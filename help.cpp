// Copyright 2015 bitmingw
// Help info of this program

#include "main.hpp"

using namespace std;

void print_usage() {
	cout << "Usage:" << endl;
	cout << "$ program_name [-i read_file_name][-p video_port_number]" \
	<< "[-o write_file_name][-f fps]" << endl;
	cout << "Either -i or -p should be given, and -p is tried prior to -i." \
	<< endl;
}
