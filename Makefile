# Copyright 2015 bitmingw

# set -D USING_GPU to accelerate image processing
default:
	cd src; \
	nvcc hist.cpp process.cpp display.cpp help.cpp main.cpp \
	-o ../run.elf -D USING_GPU -L /usr/lib \
	-l opencv_core -l opencv_imgproc \
	-l opencv_highgui -l opencv_video \
	-l opencv_gpu -l opencv_tegra \
	--cudart=shared -O2 -arch=sm_50

# 'normal' means using g++ so it is portable
normal:
	cd src; \
	g++ hist.cpp process.cpp display.cpp help.cpp main.cpp \
	-o ../run.elf -L /usr/lib -l opencv_core \
	-l opencv_imgproc -l opencv_highgui -l opencv_video \
	-Wall -O2

clean:
	rm *.elf

# test the UART communication module
serial_test:
	g++ src/rs232.c test/serial_test.cpp \
	-o serial_test.elf -Wall -O2

# test the PTZ motion controlled by UART module
motion_test:
	g++ src/rs232.c src/motion.cpp test/motion_test.cpp \
	-o motion_test.elf -Wall -O2

# test the video capture by HTTP
webcam_test:
	g++ test/webcam_test.cpp -o webcam_test.elf \
	-L /usr/lib -l opencv_core -l opencv_highgui -Wall -O2

