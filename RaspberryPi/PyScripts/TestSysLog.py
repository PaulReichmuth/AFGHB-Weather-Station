import psutil
load = psutil.cpu_percent(interval=1)


print(load)