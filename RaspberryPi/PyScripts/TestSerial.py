#!/usr/bin/python3
from influxdb import InfluxDBClient
import serial
from time import sleep as delay
import re

ifuser = "grafana"
ifhost = "127.0.0.1"
ifport = 8086


s = serial.Serial('/dev/ttyACM0', 115200)
s.isOpen()

try:
	while True:
		response = str(s.readline())
		if "Wind" in response:
			print("Direction " + re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
			direction = re.findall(r"[-+]?\d*\.\d+|\d+",response)[0]
			ifdb = "Weather_Wind"
			name = "Direction"
			body = [
    			{
        			"measurement": name,
        
        			"fields": {
						"value":direction,
						"String":None,
						"Unit": "°"
					}
				}
			]
			ifclient = InfluxDBClient(ifhost,ifport,ifuser,password=None,database=ifdb)
			ifclient.write_points(body)
			ifclient.close()
		elif "Speed" in response:
			print("Windspeed " + re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
			speed = re.findall(r"[-+]?\d*\.\d+|\d+",response)[0]
			ifdb = "Weather_Wind"
			name = "Speed"
			body = [
    			{
        			"measurement": name,
        
        			"fields": {
						"value":speed,
						"unit":"m/s"
					}
				}
			]
			ifclient = InfluxDBClient(ifhost,ifport,ifuser,password=None,database=ifdb)
			ifclient.write_points(body)
			ifclient.close()
		elif "Temp" in response:
			print("Temperature " + re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
			temp = re.findall(r"[-+]?\d*\.\d+|\d+",response)[0]
			ifdb = "Weather_Air"
			name = "Temperature"
			body = [
    			{
        			"measurement": name,
        
        			"fields": {
						"value":temp,
						"Unit":"°C"
					}
				}
			]
			ifclient = InfluxDBClient(ifhost,ifport,ifuser,password=None,database=ifdb)
			ifclient.write_points(body)
			ifclient.close()
		elif "Humi" in response:
			print("Humidity " + re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
			humi = re.findall(r"[-+]?\d*\.\d+|\d+",response)[0]
			ifdb = "Weather_Air"
			name = "Humidity"
			body = [
    			{
        			"measurement": name,
        
        			"fields": {
						"value":humi,
						"Unit":"%"
					}
				}
			]
			ifclient = InfluxDBClient(ifhost,ifport,ifuser,password=None,database=ifdb)
			ifclient.write_points(body)
			ifclient.close()
		elif "Lux" in response:
			print("Lux " + re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
			lux = re.findall(r"[-+]?\d*\.\d+|\d+",response)[0]
			ifdb = "Weather_Sun"
			name = "Light Intensity"
			body = [
    			{
        			"measurement": name,
        
        			"fields": {
						"value":lux,
						"Unit":"lux"
					}
				}
			]
			ifclient = InfluxDBClient(ifhost,ifport,ifuser,password=None,database=ifdb)
			ifclient.write_points(body)
			ifclient.close()
except KeyboardInterrupt:
	s.close()
	exit(0)
 
