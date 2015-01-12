default:
	nvcc process.cpp display.cpp help.cpp main.cpp -o run.elf -L /usr/lib -l opencv_core -l opencv_imgproc -l opencv_highgui -l opencv_tegra --cudart=shared -O2 -arch=sm_50

clean:
	rm *.elf

serial_test:
	gcc rs232.c serial_test.c -o serial_test.elf -Wall -O2

motion_test:
	g++ rs232.c motion.cpp motion_test.cpp -o motion_test.elf -Wall -O2
