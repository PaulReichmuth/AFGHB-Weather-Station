#!/usr/bin/env python

import datetime
import psutil
from influxdb import InfluxDBClient

# influx configuration - edit these
ifuser = "grafana"
ifpass = ""
ifdb   = "systemstats"
ifhost = "127.0.0.1"
ifport = 8086
measurement_name = "system"

# collect some stats from psutil
disk = psutil.disk_usage('/')
mem = psutil.virtual_memory()
load = psutil.cpu_percent(interval=0.5)
freq = psutil.cpu_freq().current
cores = "Running on a {} core system".format(psutil.cpu_count(logical=False))
boottime = datetime.datetime.fromtimestamp(psutil.boot_time()).strftime("Gestartet um %H:%M:%S am %A, den %d %B %Y")
eth0addr = psutil.net_if_addrs()['Ethernet'][1].address
net_tx = psutil.net_io_counters().bytes_sent
net_rx = psutil.net_io_counters().bytes_recv
net_tx_err = psutil.net_io_counters().errout
net_rx_err = psutil.net_io_counters().errin



# format the data as a single measurement for influx
body = [
    {
        "measurement": measurement_name,
        
        "fields": {
            "booted_at":boottime,
            "cpu_load":load,
            "cpu_freq":freq,
            "cores":cores,
            "disk_percent": disk.percent,
            "disk_free": disk.free,
            "disk_used": disk.used,
            "mem_percent": mem.percent,
            "mem_free": mem.free,
            "mem_used": mem.used,
            "ip_address":eth0addr,
            "rx":net_rx,
            "tx":net_tx,
            "rx_err":net_rx_err,
            "tx_err":net_tx_err

        }
    }
]

# connect to influx
ifclient = InfluxDBClient(ifhost,ifport,ifuser,password=None,database=ifdb)

# write the measurement
ifclient.write_points(body)