// Copyright 2015 bitmingw
// Read video stream and display in a window

#include "main.hpp"

#define IN_STREAM_NAME "camera2.avi"
#define OUT_STREAM_NAME "disp2.avi"

using namespace cv;
using namespace std;

int main()
{
	// open input video stream
	cv::VideoCapture in_stream(IN_STREAM_NAME);
	if (!in_stream.isOpened()) {
		cout << "Fatal error: can\'t read from video stream." << endl;
		return 1;
	}

	// get the frame size
	int frame_w = static_cast<int>(in_stream.get(CV_CAP_PROP_FRAME_WIDTH));
	int frame_h = static_cast<int>(in_stream.get(CV_CAP_PROP_FRAME_HEIGHT));
	cv::Size frame_size = Size(frame_w, frame_h);
	int fourcc = CV_FOURCC('X', 'V', 'I', 'D');

	// get the frame rate
	double frame_rate = in_stream.get(CV_CAP_PROP_FPS);
	if (isnan(frame_rate)) {
		frame_rate = 25;
	}
	cout << "FPS: " << frame_rate << endl;


	// open output video stream, if necessary
#ifdef OUT_STREAM_NAME
	cv::VideoWriter out_stream(OUT_STREAM_NAME, fourcc, 
		frame_rate, frame_size); 
	if (!out_stream.isOpened()) {
		cout << "Fatal error: can\'t create write stream." << endl;
		return 1;
	}
#else
	cv::VideoWriter out_stream();
#endif

	// delay between each frame in ms
	int delay = 1000 / frame_rate;

	// call the algorithm to process and display the video
	three_diff_frame(&in_stream, delay);

	// close the streams
	in_stream.release();
	out_stream.release();

	return 0;
}
