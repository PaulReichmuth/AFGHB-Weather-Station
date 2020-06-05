#!/usr/bin/python3
import paho.mqtt.publish as pub
import serial
from time import sleep as delay
import re

s = serial.Serial('/dev/ttyACM0', 115200)
s.isOpen()

try:
	while True:
		response = str(s.readline())
		if "Wind" in response:
			print("Direction " + re.findall(r"[-+]?\d*\.\d+|\d+",response))
			pub.single("sensors/data/wind/direction", re.findall(r"[-+]?\d*\.\d+|\d+",response))
		elif "Speed" in response:
			print("Windspeed " + re.findall(r"[-+]?\d*\.\d+|\d+",response))
			pub.single("sensors/data/wind/speed", re.findall(r"[-+]?\d*\.\d+|\d+",response))
		elif "Temp" in response:
			print("Temperature " + re.findall(r"[-+]?\d*\.\d+|\d+",response))
			pub.single("sensors/data/atmo/temp", re.findall(r"[-+]?\d*\.\d+|\d+",response))
		elif "Humi" in response:
			print("Humidity " + re.findall(r"[-+]?\d*\.\d+|\d+",response))
			pub.single("sensors/data/atmo/humi", re.findall(r"[-+]?\d*\.\d+|\d+",response))
except KeyboardInterrupt:
	s.close()
	exit(0)
 
