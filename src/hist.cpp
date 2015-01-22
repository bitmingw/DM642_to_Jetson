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
			if (static_cast<unsigned char>(frame->at<char>(i,j)) == 0xFF) {
				x_axis->at(j)++;
				y_axis->at(i)++;
			}
		}
	}
}

Mat_<float> hist_analysis(vector<int> *x_axis, vector<int> *y_axis,
	float threshold_ratio)
{
	assert(threshold_ratio > 0 && threshold_ratio < 1);

	int max_x = x_axis->size();
	int max_y = y_axis->size();

    // find all the positions that satisfy threshold ratio
    int threshold_x = static_cast<int>(max_x * threshold_ratio);
    int threshold_y = static_cast<int>(max_y * threshold_ratio);
    vector<int> x_coord(0);
    vector<int> y_coord(0);
    for (int i = 0; i < max_x; i++) {
        if (x_axis->at(i) > threshold_x) {
            x_coord.push_back(i);
        }
    }
    for (int i = 0; i < max_y; i++) {
        if (y_axis->at(i) > threshold_y) {
            y_coord.push_back(i);
        }
    }

	// check if moving object exists
	if (x_coord.size() == 0 || y_coord.size() == 0) {
		// nothing is found
		return Mat_<float>(4,1) << 0,0,0,0;
	}
	else {
		int x1 = 0, x2 = max_x-1, y1 = 0, y2 = max_y-1;
        int start_x = x_coord[0], len_x = 0, maxlen_x = 0;
        int start_y = y_coord[0], len_y = 0, maxlen_y = 0;

		// search for the longest continuous segment
		for (unsigned int i = 1; i < x_coord.size(); i++) {
            // when continuous and not the end
            if (x_coord[i] == x_coord[i-1] + 1 && i != x_coord.size() - 1) {
                len_x++;
            }
            // when break
            else {
                if (len_x > maxlen_x) {
                    maxlen_x = len_x; // record max length
                    x1 = start_x; // record the position
                }
                len_x = 0; // clear counter
                start_x = x_coord[i]; // try new segment
            }
		}
		x2 = x1 + maxlen_x;

		for (unsigned int i = 1; i < y_coord.size(); i++) {
            // when continuous and not the end
            if (y_coord[i] == y_coord[i-1] + 1 && i != y_coord.size() - 1) {
                len_y++;
            }
            // when break
            else {
                if (len_y > maxlen_y) {
                    maxlen_y = len_y; // record max length
                    y1 = start_y; // record the position
                }
                len_y = 0; // clear counter
                start_y = y_coord[i]; // try new segment
            }
		}
		y2 = y1 + maxlen_y;

		return Mat_<float>(4,1) << (x1+x2)/2, (y1+y2)/2, (x2-x1)/2, (y2-y1)/2;
	}
}

