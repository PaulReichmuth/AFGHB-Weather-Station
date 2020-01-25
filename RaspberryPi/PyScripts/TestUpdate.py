from sh import git
from time import sleep as delay


git.fetch()
status = git.status()

try:
	while True:
		print(status)
		if not "auf demselben Stand" in status:
			print(git.pull())
			print("updated")
			print("\n"+"\n"+"\n"+"\n"+"\n"+"\n")
			delay(10)
		else:
			print("no updates available")
			delay(10)
			git.fetch()
			status = git.status()
except KeyboardInterrupt:
	exit()
