// Copyright 2015 bitmingw
// Read video stream and display in a window

#include "main.hpp"

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

	int op;
	while ((op = getopt(argc, argv, "i:p:o:f:")) != -1) {
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

	// open a default video stream for reading
	cv::VideoCapture in_stream(0);

	// try to open video port if -p is set
	if (in_stream_port_str) {
		in_stream_port = atoi(in_stream_port_str);
		in_stream = VideoCapture(in_stream_port);
		// if can't open port and no -i is given, terminate with error
		if (!in_stream.isOpened()) {
			in_stream_port_str = NULL;
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
	cout << "FPS: " << frame_rate << endl;

	// delay between each frame in ms
	int delay = 1000 / frame_rate;


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

		// process, display and record the video
		three_diff_frame(&in_stream, delay, &out_stream);

		// close I/O streams
		in_stream.release();
		out_stream.release();
	}
	// don't write video to file
	else {
		// process and display the video
		three_diff_frame(&in_stream, delay);

		// close the streams
		in_stream.release();
	}

	return 0;
}
