#!/usr/bin/python3


import docker
from influxdb import InfluxDBClient

# influx configuration - edit these
ifuser = "grafana"
ifpass = ""
ifdb   = "systemstats"
ifhost = "127.0.0.1"
ifport = 8086
measurement_name = "docker"

client = docker.from_env()

ls_cont = client.containers.list(all=True)

names = list()
stats = list()

for item in ls_cont:
	ThisContainer = client.containers.get(item.id)
	names.append(str(ThisContainer.name))
	if str(ThisContainer.status) == "running":
		stats.append(1)
	elif str(ThisContainer.status) == "exited":
		stats.append(0)

print(names)
print(stats)


# format the data as a single measurement for influx
body = [
    {
        "measurement": measurement_name,
        
        "fields": {
            names[0]:stats[0],
            names[1]:stats[1],
            names[2]:stats[2],
            names[3]:stats[3],
            names[4]:stats[4]

        }
    }
]

# connect to influx
ifclient = InfluxDBClient(ifhost,ifport,ifuser,password=None,database=ifdb)

# write the measurement
ifclient.write_points(body)
