display:
	nvcc display.cpp -o display.elf -L /usr/lib -l opencv_core -l opencv_highgui -l opencv_tegra --cudart=shared -O2 -arch=sm_50
