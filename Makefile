default:
	nvcc process.cpp main.cpp -o run.elf -L /usr/lib -l opencv_core -l opencv_imgproc -l opencv_highgui -l opencv_tegra --cudart=shared -O2 -arch=sm_50
