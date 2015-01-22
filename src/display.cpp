// Copyright 2015 bitmingw
// Display video stream

#include "../include/main.hpp"

using namespace cv;
using namespace std;

int display_video(VideoCapture *in_stream_ptr, int delay_ms) {
	Mat disp;
	namedWindow("camera");
	int num_frames = 0;
	
	cout << "Width: " << in_stream_ptr->get(CV_CAP_PROP_FRAME_WIDTH) << endl;
	cout << "Height: " << in_stream_ptr->get(CV_CAP_PROP_FRAME_HEIGHT) << endl;

	bool stop = false;
	while (!stop) {
		if (!in_stream_ptr->read(disp)) {
			break;
		}

		imshow("camera", disp);
		num_frames++;

		if (waitKey(delay_ms) >= 0) {
			stop = true;
		}	
	}	
	return num_frames;
}
