// Copyright 2015 bitmingw
// Header of functions in the project

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// Processing algorithm
int three_diff_frame(cv::VideoCapture *stream_ptr, int delay_ms);

// Helper: determine the buffer to store the next frame
int get_next_frame_position(int current_position);

// Helper: read 1 gray frame
int read_1_gray_frame(cv::VideoCapture *stream_ptr, cv::Mat *frame);

// Helper: read 3 gray frames
int read_3_gray_frames(cv::VideoCapture *stream_ptr, 
cv::Mat *frame1, cv::Mat *frame2, cv::Mat *frame3);

// Helper: generate 2 gray difference frames
int gen_2_gray_diff_frames(int start_position, cv::Mat *diff1, cv::Mat *diff2, 
cv::Mat *frame1, cv::Mat *frame2, cv::Mat *frame3);

// Helper: combine 2 difference frames to the result
int combine_diff_frames(cv::Mat *result, cv::Mat *diff1, cv::Mat *diff2);
