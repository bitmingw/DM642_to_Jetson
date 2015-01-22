// Copyright 2015 bitmingw
// Process and display the video stream via three-difference-frame method

#include "../include/main.hpp"

using namespace cv;
using namespace std;

int three_diff_frame(VideoCapture *in_stream_ptr, int delay_ms,
	VideoWriter *out_stream_ptr, bool tracking) 
{
	Mat frame_disp; // should be CV_8UC1
	Mat frame1, frame2, frame3;	// frames to store original images
	Mat diff_frame1, diff_frame2;	// frames of difference
	namedWindow("camera");
	int num_frames = 0;

	int cur_frame_position = 3;	// The position of last frame
	int frame_read_status = true;	// if next frame exists

	// on the first run, read 3 frames and calculate differences
	read_3_gray_frames(in_stream_ptr, &frame1, &frame2, &frame3);
#ifdef USING_GPU
	cout << "Processing via GPU..." << endl;
	gpu_gen_2_gray_diff_frames(cur_frame_position, &diff_frame1, &diff_frame2, 
		&frame1, &frame2, &frame3);
	gpu_combine_diff_frames(&frame_disp, &diff_frame1, &diff_frame2);
#else
	gen_2_gray_diff_frames(cur_frame_position, &diff_frame1, &diff_frame2, 
		&frame1, &frame2, &frame3);
	combine_diff_frames(&frame_disp, &diff_frame1, &diff_frame2);
#endif

	// median filter
	medianBlur(frame_disp, frame_disp, 5);
	// binaryzation
	threshold(frame_disp, frame_disp, 20, 255, THRESH_BINARY);
	// setup two-sided histogram
	vector<int> x_axis(frame_disp.cols);
	vector<int> y_axis(frame_disp.rows);
	two_histogram(&frame_disp, &x_axis, &y_axis);
	Mat_<float> obj_pos_range = hist_analysis(&x_axis, &y_axis, 0.05, 0.25);
	// setup matrix used in Kalman filter
	Mat_<float> measurement(2,1);
	Mat_<float> control(2,1);
	Mat_<float> estimated(4,1);
	measurement.setTo(Scalar(0));
	control.setTo(Scalar(0));

	// initialize kalman filter
	KalmanFilter kf(4, 2, 2); // 4 order, 2 measure, 2 control
	kf.transitionMatrix = *(Mat_<float>(4, 4) << \
		1,0,1,0, 0,1,0,1, 0,0,1,0, 0,0,0,1);
	kf.statePre.at<float>(0) = obj_pos_range(0);
	kf.statePre.at<float>(1) = obj_pos_range(1);
	kf.statePre.at<float>(2) = 0; 
	kf.statePre.at<float>(0) = 0;
	setIdentity(kf.measurementMatrix); // set to I
	setIdentity(kf.processNoiseCov, Scalar::all(1e-4));
	setIdentity(kf.measurementNoiseCov, Scalar::all(10));
	setIdentity(kf.errorCovPost, Scalar::all(0.1));
	
	// display the first frame
	imshow("camera", frame_disp);
	if (out_stream_ptr) {
		cout << "Saving video to file..." << endl;
		out_stream_ptr->write(frame_disp);
	}
	num_frames += 3;


	// setup the loop
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
#ifdef USING_GPU
		gpu_gen_2_gray_diff_frames(cur_frame_position,
			&diff_frame1, &diff_frame2, 
			&frame1, &frame2, &frame3);
		gpu_combine_diff_frames(&frame_disp, &diff_frame1, &diff_frame2);
#else
		gen_2_gray_diff_frames(cur_frame_position, &diff_frame1, &diff_frame2, 
			&frame1, &frame2, &frame3);
		combine_diff_frames(&frame_disp, &diff_frame1, &diff_frame2);
#endif

		// median filter
		medianBlur(frame_disp, frame_disp, 5);
		// binaryzation
		threshold(frame_disp, frame_disp, 20, 255, THRESH_BINARY);

		if (tracking) {
			two_histogram(&frame_disp, &x_axis, &y_axis);
			obj_pos_range = hist_analysis(&x_axis, &y_axis, 0.05, 0.25);

			// predict & update
			kf.predict(control);
			measurement(0) = obj_pos_range(0);
			measurement(1) = obj_pos_range(1);	
			estimated = kf.correct(measurement);
			cout << estimated(0) << " " << estimated(1) << endl;
		}

		// display video
		imshow("camera", frame_disp);
		if (out_stream_ptr) {
			out_stream_ptr->write(frame_disp);
		}
		num_frames++;

		// delay for next frame
		// and press any key to stop
		if (waitKey(delay_ms) >= 0) {
			stop = true;
		}
	}
	return num_frames;
}

int get_next_frame_position(int current_position) {
	if (current_position == 1 || current_position == 2) {
		return current_position + 1;
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
	Mat *frame1, Mat *frame2, Mat *frame3)
{
	in_stream_ptr->read(*frame1);
	in_stream_ptr->read(*frame2);
	in_stream_ptr->read(*frame3);
	cvtColor(*frame1, *frame1, CV_BGR2GRAY);
	cvtColor(*frame2, *frame2, CV_BGR2GRAY);
	cvtColor(*frame3, *frame3, CV_BGR2GRAY);
	return 0;
}

int gen_2_gray_diff_frames(int start_position, Mat *diff1, Mat *diff2, 
	Mat *frame1, Mat *frame2, Mat *frame3)
{
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

#ifdef USING_GPU
int gpu_gen_2_gray_diff_frames(int start_position, Mat *diff1, Mat *diff2, 
	Mat *frame1, Mat *frame2, Mat *frame3)
{
	gpu::GpuMat gpu_frame1, gpu_frame2, gpu_frame3, gpu_diff1, gpu_diff2;
	
	switch (start_position) {
		case 3:
			gpu_frame2.upload(*frame2);
			gpu_frame1.upload(*frame1);
			gpu::subtract(gpu_frame2, gpu_frame1, gpu_diff1);
			gpu_diff1.download(*diff1);
			
			gpu_frame3.upload(*frame3);
			gpu::subtract(gpu_frame3, gpu_frame2, gpu_diff2);
			gpu_diff1.download(*diff2);
			
			break;
		case 1:
			gpu_frame3.upload(*frame3);
			gpu_frame2.upload(*frame2);
			gpu::subtract(gpu_frame3, gpu_frame2, gpu_diff1);
			gpu_diff1.download(*diff1);
			
			gpu_frame1.upload(*frame1);
			gpu::subtract(gpu_frame1, gpu_frame3, gpu_diff2);
			gpu_diff1.download(*diff2);
			
			break;
		case 2:
			gpu_frame1.upload(*frame1);
			gpu_frame3.upload(*frame3);
			gpu::subtract(gpu_frame1, gpu_frame3, gpu_diff1);
			gpu_diff1.download(*diff1);
			
			gpu_frame2.upload(*frame2);
			gpu::subtract(gpu_frame2, gpu_frame1, gpu_diff2);
			gpu_diff1.download(*diff2);
			
			break;
	}

	return 0;
}
#endif

int combine_diff_frames(Mat *result, Mat *diff1, Mat *diff2) {
	(*result) = (*diff2) + (*diff1);
	return 0;
}

#ifdef USING_GPU
int gpu_combine_diff_frames(Mat *result, Mat *diff1, Mat *diff2) {
	gpu::GpuMat gpu_result, gpu_diff1, gpu_diff2;
	gpu_diff1.upload(*diff1);
	gpu_diff2.upload(*diff2);
	gpu::add(gpu_diff1, gpu_diff2, gpu_result);
	gpu_result.download(*result);
	return 0;
}
#endif

