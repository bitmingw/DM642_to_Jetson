// Copyright 2015 bitmingw
// Process and display the video stream via three-difference-frame method

#include "main.hpp"

using namespace cv;
using namespace std;

int three_diff_frame(VideoCapture *in_stream_ptr, int delay_ms,
	VideoWriter *out_stream_ptr) 
{
	Mat frame_disp; 
	Mat frame1, frame2, frame3;	// frames to store original images
	Mat diff_frame1, diff_frame2;	// frames of difference
	namedWindow("camera");

	int cur_frame_position = 3;	// The position of last frame
	int frame_read_status = true;	// if next frame exists

	// on the first run, read 3 frames and calculate differences
	read_3_gray_frames(in_stream_ptr, &frame1, &frame2, &frame3);
	gen_2_gray_diff_frames(cur_frame_position, &diff_frame1, &diff_frame2, 
		&frame1, &frame2, &frame3);
	combine_diff_frames(&frame_disp, &diff_frame1, &diff_frame2);

	imshow("camera", frame_disp);
	if (out_stream_ptr) {
		(*out_stream_ptr).write(frame_disp);
	}

	bool stop = false;
	while (!stop) {

		// in the loop, read 1 frame and calculate differences
		cur_frame_position = get_next_frame_position(cur_frame_position);

		switch (cur_frame_position) {
			case 1:
				frame_read_status = read_1_gray_frame(in_stream_ptr, &frame1);
				break;
			case 2:
				frame_read_status = read_1_gray_frame(in_stream_ptr, &frame2);
				break;	
			case 3:
				frame_read_status = read_1_gray_frame(in_stream_ptr, &frame3);
				break;
		}

		// break when reach the end of file
		if (!frame_read_status) {
			break;
		}

		// calculate differences
		gen_2_gray_diff_frames(cur_frame_position, &diff_frame1, &diff_frame2, 
			&frame1, &frame2, &frame3);
		combine_diff_frames(&frame_disp, &diff_frame1, &diff_frame2);

		imshow("camera", frame_disp);
		if (out_stream_ptr) {
			(*out_stream_ptr).write(frame_disp);
		}

		// delay for next frame
		// and press any key to stop
		if (waitKey(delay_ms) >= 0) {
			stop = true;
		}
	}
	return 0;
}

int get_next_frame_position(int current_position) {
	if (current_position == 1 || current_position == 2) {
		return current_position++;
	} else {
		return 1;
	}
}

int read_1_gray_frame(VideoCapture *in_stream_ptr, Mat *frame) {
	if (!in_stream_ptr->read(*frame)) {
		return false;
	}
	cvtColor(*frame, *frame, CV_BGR2GRAY);
	return true;
}

int read_3_gray_frames(VideoCapture *in_stream_ptr, 
Mat *frame1, Mat *frame2, Mat *frame3) {
	in_stream_ptr->read(*frame1);
	in_stream_ptr->read(*frame2);
	in_stream_ptr->read(*frame3);
	cvtColor(*frame1, *frame1, CV_BGR2GRAY);
	cvtColor(*frame2, *frame2, CV_BGR2GRAY);
	cvtColor(*frame3, *frame3, CV_BGR2GRAY);
	return 0;
}

int gen_2_gray_diff_frames(int start_position, Mat *diff1, Mat *diff2, 
Mat *frame1, Mat *frame2, Mat *frame3) {
	switch (start_position) {
		case 3:
			(*diff1) = (*frame2) - (*frame1);
			(*diff2) = (*frame3) - (*frame2);
			break;
		case 1:
			(*diff1) = (*frame3) - (*frame2);
			(*diff2) = (*frame1) - (*frame3);
			break;
		case 2:
			(*diff1) = (*frame1) - (*frame3);
			(*diff2) = (*frame2) - (*frame1);
			break;
	}
	return 0;
}

int combine_diff_frames(Mat *result, Mat *diff1, Mat *diff2) {
	(*result) = (*diff2) + (*diff1);
	return 0;
}

