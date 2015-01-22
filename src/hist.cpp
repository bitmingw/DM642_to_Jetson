// Copyright 2015 bitmingw
// Calculate and analyze light distribution of a frame

#include "../include/main.hpp"

using namespace cv;
using namespace std;

void two_histogram(Mat *frame, vector<int> *x_axis, vector<int> *y_axis) { 
	assert(static_cast<unsigned int>(frame->rows) == y_axis->size() && \
		static_cast<unsigned int>(frame->cols) == x_axis->size());

	int max_row = frame->rows;
	int max_col = frame->cols;
	
	// clean former info
	for (vector<int>::iterator it = x_axis->begin(); it != x_axis->end(); it++) {
		*it = 0;
	}
	for (vector<int>::iterator it = y_axis->begin(); it != y_axis->end(); it++) {
		*it = 0;
	}

	// calculate two-side histogram at the same time
	for (int i = 0; i < max_row; i++) {
		for (int j = 0; j < max_col; j++) {
			if (frame->at<char>(i,j) == 0xFF) {
				x_axis->at(j)++;
				y_axis->at(i)++;
			}
		}
	}
}

Mat_<float> hist_analysis(vector<int> *x_axis, vector<int> *y_axis,
	float threshold_ratio, float boundary_ratio)
{
	assert(threshold_ratio > 0 && threshold_ratio < 1);
	assert(boundary_ratio> 0 && boundary_ratio< 1);

	// find peak at x and y axis
	int peak_val_x = 0, peak_val_y = 0;
	int peak_pos_x = 0, peak_pos_y = 0;
	int max_x = x_axis->size();
	int max_y = y_axis->size();

	for (int i = 0; i < max_x; i++) {
		if (x_axis->at(i) > peak_val_x) {
			peak_val_x = x_axis->at(i);
			peak_pos_x = i;
		}
	}

	for (int i = 0; i < max_y; i++) {
		if (y_axis->at(i) > peak_val_y) {
			peak_val_y = y_axis->at(i);
			peak_pos_y = i;
		}
	}

	// check if moving object exists
	if (peak_val_x < max_x*threshold_ratio || peak_val_y < max_y*threshold_ratio) {
		// nothing is found
		return Mat_<float>(4,1) << 0,0,0,0;
	}
	else {
		int x1 = 0, x2 = max_x-1, y1 = 0, y2 = max_y-1;
		int edge_val_x = static_cast<int>(peak_val_x * boundary_ratio);
		int edge_val_y = static_cast<int>(peak_val_y * boundary_ratio);

		for (int i = peak_pos_x; i >= 0; i--) {
			if (x_axis->at(i) < edge_val_x) {
				x1 = i;
				break;
			}
		}
		for (int i = peak_pos_x; i < max_x; i++) {
			if (x_axis->at(i) < edge_val_x) {
				x2 = i;
				break;
			}
		}
		for (int i = peak_pos_y; i >= 0; i--) {
			if (y_axis->at(i) < edge_val_y) {
				y1 = i;
				break;
			}
		}
		for (int i = peak_pos_y; i < max_y; i++) {
			if (y_axis->at(i) < edge_val_y) {
				y2 = i;
				break;
			}
		}
		return Mat_<float>(4,1) << (x1+x2)/2, (y1+y2)/2, (x2-x1)/2, (y2-y1)/2;
	}
}

