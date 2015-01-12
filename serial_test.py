import time
import serial

# ttyS0 is wired to RS232 port
# ttyS1 to ttyS3 are not configured
# ttyTHS0 to ttyTHS2 are wired to expansion ports
ser = serial.Serial("/dev/ttyS0", 9600)

while True:
	ser.write("a")
	time.sleep(1)

