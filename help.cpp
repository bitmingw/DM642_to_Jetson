// Copyright 2015 bitmingw
// Help info of this program

#include "main.hpp"

using namespace std;

void print_usage() {
	cout << "Usage:" << endl;
	cout << "$ program_name [-h][-i read_file_name][-p video_port_number]" \
	<< "[-o write_file_name][-f fps][-d][-t]" << endl;
	cout << "To show this help info, use -h." << endl;
	cout << "To specify a source of video, " \
	<< "either -i or -p should be given, " \
	<< "and -p is tried prior to -i." << endl;
	cout << "To display the video without processing, use -d." << endl;
	cout << "To enable performance test, use -t." << endl;
}
