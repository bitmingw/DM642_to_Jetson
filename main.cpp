// Copyright 2015 bitmingw
// Read video stream and display in a window

#include "main.hpp"

using namespace cv;
using namespace std;

int main()
{
	string stream_name = "camera2.avi";
	
	cv::VideoCapture stream(stream_name);
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

	// delay between each frame in ms
	int delay = 1000 / frame_rate;

	// call the algorithm to process and display the video
	three_diff_frame(&stream, delay);

	// close the stream
	stream.release();

	return 0;
}
