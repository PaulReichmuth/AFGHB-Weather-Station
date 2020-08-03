import serial

s = serial.Serial("/dev/ttyACM0",115200)

while s.isOpen():
	print(s.readline())
