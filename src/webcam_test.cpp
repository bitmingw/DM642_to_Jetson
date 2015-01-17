// Copyright 2015 bitmingw
// Test program of reading video sequence from web

#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
	string video_url = "http://127.0.0.1:8080/?action=stream?dummy=.mjpg";

	VideoCapture ipCam(video_url);
	if (!ipCam.isOpened()) {
		cerr << "Fatal error: can\'t open web camera!" << endl;
		return 1;
	}		
	
	Mat frame;
	namedWindow("webcam");
	
	cout << "Now start webcam!" << endl;
	while (1) {
		ipCam.read(frame);
		imshow("webcam", frame);
		
		// delay to achieve 25fps, press any key to stop
		if (waitKey(40) >= 0) {
			break;
		}
	}
	return 0;	
}
