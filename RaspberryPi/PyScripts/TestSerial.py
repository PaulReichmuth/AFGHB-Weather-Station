#!/usr/bin/python3
import paho.mqtt.publish as pub
import serial
from time import sleep as delay


s = serial.Serial('/dev/ttyACM0', 115200)
s.isOpen()

try:
	while True:
		response = str(s.readline())
		if "Wind" in response:
			print("Direction " + response.strip("Wind "))
			pub.single("sensors/data/wind/direction", response.strip("Wind "))
		elif "Speed" in response:
			print("Windspeed " + response.strip("Speed "))
			pub.single("sensors/data/wind/speed", response.strip("Speed "))
		elif "Temp" in response:
			print("Temperature " + response.strip("Temp "))
			pub.single("sensors/data/atmo/temp", response.strip("Temp "))
		elif "Humi" in response:
			print("Humidity " + response.strip("Humi "))
			pub.single("sensors/data/atmo/humi", response.strip("Humi "))
except KeyboardInterrupt:
	s.close()
	exit(0)
 