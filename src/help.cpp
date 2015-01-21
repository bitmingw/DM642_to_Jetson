// Copyright 2015 bitmingw
// Help info of this program

#include "../include/main.hpp"

using namespace std;

void print_usage() {
	cout << endl << "Usage:" << endl;
	cout << "$ program_name [-h][-w][-p video_port_number]" << endl;
	cout << "[-i read_file_name][-o write_file_name][-f fps][-k][-d][-t]" << endl;
	cout << "To show this help info, use -h." << endl;
	cout << "To specify a source of video: " << endl;
	cout << "Use -w to read video sequence from localhost webcam." << endl;
	cout << "Use -p + port number to read video from /dev/video*." << endl;
	cout << "Use -i + file name to read video from *.avi file." << endl;
	cout << "Use -o + file name to save the results to file." << endl;
	cout << "Use -k to turn on Kalman filter to tracking moving objects." << endl;
	cout << "Use -d to display the video without processing." << endl;
	cout << "Use -t to enable performance test." << endl << endl;
}
