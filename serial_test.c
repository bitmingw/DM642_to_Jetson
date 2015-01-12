// Copyright 2015 bitmingw
// Test program of RS232 output in C interface

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rs232.h"

int main()
{
	// Define UART arguments
	int port_num = 0; /* /dev/ttyS0 */
	int bandrate = 9600;
	char mode[] = {'8', 'N', '1', 0}; /* 8bit no parity 1 stop */

	// data to be sent
	char data[512];
	strcpy(data, "The quick brown fox jumped over the lazy grey dog.\n");

	// open serial port
	if (RS232_OpenComport(port_num, bandrate, mode)) {
		fprintf(stderr, "Can\'t open serial port %d.\n", port_num);
		return 1;
	}
	while (1) {
		RS232_cputs(port_num, data);
		printf("Sent data: %s\n", data);
		usleep(100000); /* sleep 100 millisecond */
	}
	return 0;
}
