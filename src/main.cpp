// Copyright 2015 bitmingw
// Read video stream and display in a window

#include "../include/main.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	// read command line arguments 
	char *in_stream_name = NULL;
	char *in_stream_port_str = NULL;
	int in_stream_port = -1;
	char *out_stream_name = NULL;
	char *frame_rate_str = NULL;
	double frame_rate = 25.0;
	bool show_help = false;
	bool display_only = false;
	bool performance_test = false;

	int op;
	while ((op = getopt(argc, argv, "i:p:o:f:hdt")) != -1) {
		switch (op) {
			case 'i':
				in_stream_name = optarg;
				break;
			case 'p':
				in_stream_port_str = optarg;
				break;
			case 'o':
				out_stream_name = optarg;
				break;
			case 'f':
				frame_rate_str = optarg;
				break;
			case 'h':
				show_help = true;
				break;
			case 'd':
				display_only = true;
				break;
			case 't':
				performance_test = true;
				break;
			default:
				cerr << "Wrong arguments!" << endl;
				print_usage();
				return 1;
		}
	}

	// report usage of program when no argument is given
	if (optind == 1) {
		print_usage();	
		return 1;
	}

	// show help info if -h is given
	if (show_help) {
		print_usage();
		return 0;
	}

	// open a default video stream for reading
	cv::VideoCapture in_stream(0);

	// try to open video port if -p is set
	if (in_stream_port_str) {
		in_stream_port = atoi(in_stream_port_str);
		in_stream = VideoCapture(in_stream_port);
		// if can't open port and no -i is given, terminate with error
		if (!in_stream.isOpened()) {
			in_stream_port_str = NULL; // port is invalid
			if (!in_stream_name) {
				cerr << "Fatal error: can\'t read from video port " \
				<< in_stream_port << '.' << endl;
				return 1;
			}
		}
	} 
	
	// try to read video file if -i is set
	if (in_stream_name && !in_stream_port_str) {
		in_stream = VideoCapture(in_stream_name);
		// if can't open video file, terminate with error
		if (!in_stream.isOpened()) {
			cerr << "Fatal error: can\'t read from video file " \
			<< in_stream_name << '.' << endl;
			return 1;
		}
	}

	// get the frame size
	int frame_w = static_cast<int>(in_stream.get(CV_CAP_PROP_FRAME_WIDTH));
	int frame_h = static_cast<int>(in_stream.get(CV_CAP_PROP_FRAME_HEIGHT));
	cv::Size frame_size = Size(frame_w, frame_h);
	int fourcc = CV_FOURCC('X', 'V', 'I', 'D');

	// get the frame rate 
	if (frame_rate_str) {
		frame_rate = atof(frame_rate_str);
	}
	cout << "Source video FPS: " << frame_rate << endl;

	// delay between each frame in ms
	int delay = 1000 / frame_rate;

	// set timer for performance test
	double start_time = 0, stop_time = 0, real_time_difference = 0;
	int num_frames;
	double tick_frequency = static_cast<double>(getTickFrequency());

	// try to write video file if -o is set
	if (out_stream_name) {
		cv::VideoWriter out_stream(out_stream_name, fourcc, 
			frame_rate, frame_size); 
		// if can't write to video file, terminate with error
		if (!out_stream.isOpened()) {
			cerr << "Fatal error: can\'t create write stream " \
			<< out_stream_name << '.' << endl;
			return 1;
		}

		if (performance_test) {
			start_time = static_cast<double>(getTickCount());		
		}

		if (display_only) {
			num_frames = display_video(&in_stream, delay, &out_stream);
		}
		else {
			num_frames = three_diff_frame(&in_stream, delay, &out_stream);
		}

		if (performance_test) {
			stop_time = static_cast<double>(getTickCount());
		}

		// close I/O streams
		in_stream.release();
		out_stream.release();
	}

	// don't write video to file
	else {
		if (performance_test) {
			start_time = static_cast<double>(getTickCount());		
		}

		if (display_only) {
			num_frames = display_video(&in_stream, delay);
		}
		else {
			num_frames = three_diff_frame(&in_stream, delay);
		}

		if (performance_test) {
			stop_time = static_cast<double>(getTickCount());
		}

		// close input stream
		in_stream.release();
	}
	
	// report actual FPS if enable performance test
	if (performance_test) {
		real_time_difference = (stop_time - start_time) / tick_frequency;
		cout << "Time to process: " << real_time_difference << " s" << endl;
		if (num_frames > 0) {
			cout << "Number of frames: " << num_frames << endl;
			cout << "Actual FPS: " << num_frames / real_time_difference << endl;
		}
	}

	return 0;
}

