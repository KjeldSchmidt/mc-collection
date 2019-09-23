import serial;
import time

arduino = serial.Serial('/dev/ttyUSB0', timeout = 0.5)
i = 33
while(True):
	time.sleep(2)
	arduino.write(b"CxxBxCxBCxBxCBCB")
	
