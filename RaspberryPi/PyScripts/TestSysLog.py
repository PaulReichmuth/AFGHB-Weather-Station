import psutil
load = psutil.cpu_percent(interval=1)
diskc = psutil.disk_usage("C:")
diskd = psutil.disk_usage("D:")


print("CPU: " + str(load))
print("Disk C: " + str(diskc.percent))
print("Disk D: " + str(diskd.percent))