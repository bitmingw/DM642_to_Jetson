// Copyright 2015 bitmingw
// Process and display the video stream via three-difference-frame method

#include "main.hpp"

using namespace cv;
using namespace std;

int three_diff_frame(VideoCapture * stream_ptr, int delay_ms) {
	Mat frame; // frame to be displayed
	namedWindow("camera");

	bool stop = false;
	while (!stop) {
		// if there is no next frame, stop the process
		if (!stream_ptr->read(frame)) {
			break;
		}
		
		// convert the frame into Gray
		cvtColor(frame, frame, CV_BGR2GRAY);

		imshow("camera", frame);

		// delay for next frame
		// and press any key to stop
		if (waitKey(delay_ms) >= 0) {
			stop = true;
		}
	}
	return 0;
}
