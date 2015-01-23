// Copyright 2015 bitmingw
// Test the motion of PTZ

#include "../include/motion.hpp"

using namespace std;

int main()
{
	int port_num = 0;
	int bandrate = 9600;

	UART_open(port_num, bandrate);
	sleep(1); // wait signal to be stable
	for (int i = 0; i < 6; i++) {
		if (i % 3 == 0) {
			PTZ_turn_left(port_num);
			usleep(100000); // wait for 0.1s
			PTZ_turn_left(port_num); // hope it receives the message
			sleep(10); // turn left for 10 seconds
		}
		else if (i % 3 == 1) {
			PTZ_stop(port_num);
			usleep(100000);
			PTZ_stop(port_num);
			sleep(1); // stop 1 second
		}
		else if (i % 3 == 2) {
			PTZ_turn_right(port_num);
			usleep(100000);
			PTZ_turn_right(port_num);
			sleep(10);
		}
	}	
	UART_close(port_num);
	return 0;
}

