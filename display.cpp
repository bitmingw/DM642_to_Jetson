// Copyright 2015 bitmingw
// Read video stream and display in a window

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main()
{
	string stream_name = "camera2.avi";
	
	VideoCapture stream(stream_name);
	if (!stream.isOpened()) {
		cout << "Fatal error: can\'t open video stream.\n" << endl;
		return 1;
	}

	// get the frame rate
	double frame_rate = stream.get(CV_CAP_PROP_FPS);
	// cout << "FPS: " << frame_rate << endl;
	if (isnan(frame_rate)) {
		frame_rate = 30;
	}
	cout << "FPS: " << frame_rate << endl;
	bool stop = false;

	// the current video frame
	Mat frame;
	namedWindow("camera");

	// delay between each frame in ms
	int delay = 1000 / frame_rate;

	// display the video
	while (!stop) {
		if (!stream.read(frame)) {
			break;
		}		
		
		imshow("camera", frame);

		// delay to display video, and press a key to stop
		if (waitKey(delay) >= 0) {
			stop = true;
		}
	}

	// close the stream
	stream.release();

	return 0;
}
