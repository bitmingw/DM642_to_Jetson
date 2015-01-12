// Copyright 2015 bitmingw
// Header of motion.cpp

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include "rs232.h"

// Note: PTZ = pan/tilt/zoom

// Open a RS232 port, 0 -> success, 1 -> failure
int UART_open(int port_num, int bandrate);
void UART_close(int port_num);

// Control the motion of PTZ by PELCO-D protocol
// 0 -> success, 1 -> failure
int PTZ_turn_left(int port_num);
int PTZ_turn_right(int port_num);
int PTZ_stop(int port_num);
