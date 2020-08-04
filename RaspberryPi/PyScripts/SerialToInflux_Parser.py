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
			direction = float(re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
			ifdb = "Weather_Wind"
			name = "Direction"
			body = [
    			{
        			"measurement": name,
        
        			"fields": {
						"value":(360/16)*direction,
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
			speed = float(re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
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
			temp = float(re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
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
			humi = float(re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
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
			lux = float(re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
			ifdb = "Weather_Sun"
			name = "Light_Intensity"
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
		elif "Presssure" in response:
			pressure = float(re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
			print("Air Pressure " + str(pressure))

			ifdb = "Weather_Air"
			name = "Pressure"
			body = [
				{
					"measurement": name,

					"fields": {
						"value": pressure,
						"Unit": "hPa"
					}
				}
			]
			ifclient = InfluxDBClient(ifhost,ifport,ifuser,password=None,database=ifdb)
			ifclient.write_points(body)
			ifclient.close()
		elif "Reference Temp" in response:
			reftemp = float(re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
			print("Reference Temp" + str(reftemp))
			ifdb = "Weather_Air"
			name = "Temp ref."
			body = [
				{
					"measurement": name,

					"fields": {
						"value": reftemp,
						"Unit": "°C"
					}
				}
			]
			ifclient = InfluxDBClient(ifhost,ifport,ifuser,password=None,database=ifdb)
			ifclient.write_points(body)
			ifclient.close()
		elif "UV-Index" in response:
    		UVI = float(re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
			print("UV-Index " + str(UVI))
			ifdb = "Weather_Sun"
			name = "UV-Index"
			body = [
				{
					"measurement":name,

					"fields": {
						"value": UVI,
						"Unit": None
					}
				}
			]
			ifclient = InfluxDBClient(ifhost,ifport,ifuser,password=None,database=ifdb)
			ifclient.write_points(body)
			ifclient.close()
		elif "UV-raw" in response:
			UVraw = float(re.findall(r"[-+]?\d*\.\d+|\d+",response)[0])
			print("RAW UV "+str(UVraw))
			ifdb = "Weather_Sun"
			name = "UV-RAW"
			body = [
				{
					"measurement":name,

					"fields": {
						"value": UVraw,
						"Unit": None
					}
				}
			]
			ifclient = InfluxDBClient(ifhost,ifport,ifuser,password=None,database=ifdb)
			ifclient.write_points(body)
			ifclient.close()
except KeyboardInterrupt:
	s.close()
	exit(0)
 
