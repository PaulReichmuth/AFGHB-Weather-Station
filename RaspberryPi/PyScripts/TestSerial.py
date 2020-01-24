##!/usr/bin/python3

import serial
from time import sleep as delay

s = serial.Serial('/dev/ttyACM0', 115200)
s.isOpen()

try:
	while True:
		response = str(s.readline())
		if "Wind" in response:
			print("Direction " + response.strip("Wind "))
		elif "Speed" in response:
			print("Windspeed " + response.strip("Speed "))
except KeyboardInterrupt:
	s.close()
	exit(0)
