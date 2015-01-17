// Copyright 2015 bitmingw
// Control the motion of PTZ via RS232->RS485 signal

#include "../include/motion.hpp"

using namespace std;

int UART_open(int port_num, int bandrate) {
	// setup serial mode: 8bit, no parity, 1 stop bit
	char mode[] = {'8', 'N', '1', 0};
	if (RS232_OpenComport(port_num, bandrate, mode) != 0) {
		cerr << "Can\'t open RS232 port " << port_num << "." << endl;
		return 1;
	}
	return 0;
}

void UART_close(int port_num) {
	RS232_CloseComport(port_num);
}

int PTZ_turn_left(int port_num) {
	unsigned char left_code[] = {0xFF, 0x01, 0x00, 0x02, 0x3F, 0x00, 0x42};
	int length = 7;
	if (RS232_SendBuf(port_num, left_code, length) == -1) {
		cerr << "Code [turing left] send failure!" << endl;
		return 1;
	}
	return 0;
}

int PTZ_turn_right(int port_num) {
	unsigned char right_code[] = {0xFF, 0x01, 0x00, 0x04, 0x3F, 0x00, 0x44};
	int length = 7;
	if (RS232_SendBuf(port_num, right_code, length) == -1) {
		cerr << "Code [turing right] send failure!" << endl;
		return 1;
	}
	return 0;

}

int PTZ_stop(int port_num) {
	unsigned char stop_code[] = {0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01};
	int length = 7;
	if (RS232_SendBuf(port_num, stop_code, length) == -1) {
		cerr << "Code [stop motion] send failure!" << endl;
		return 1;
	}
	return 0;

}

