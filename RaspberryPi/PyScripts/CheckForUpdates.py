#!/usr/bin/python3
# Code originally made by John Grant
# https://medium.com/@johngrant/updating-distributed-raspberrypis-with-automatic-code-updates-3336aa0bcca1
import sh
from sh import git
import time
import os, sys
import argparse

parser = argparse.ArgumentParser(description='Update a git directory from a selected branch')
parser.add_argument('branch', metavar='branch', type=str ,help='branch to update from')
parser.add_argument('dir', metavar='directory', type=str ,help='directory to update', default="/WeatherStation/")

args = parser.parse_args()
print ("branch = " + args.branch)
print ("directory = " + args.dir)

aggregated = ""
branch = args.branch
gitDir = args.dir

def CheckForUpdate(workingDir):
    print("Fetching most recent code from source..." + workingDir)

    # Fetch most up to date version of code.
    p = git("--git-dir=" + workingDir + ".git/", "--work-tree=" + workingDir, "fetch", "origin", branch, _out=ProcessFetch, _out_bufsize=0, _tty_in=True)
    print("Fetch complete.")
    time.sleep(2)
    print("Checking status for " + workingDir + "...")
    statusCheck = git("--git-dir=" + workingDir + ".git/", "--work-tree=" + workingDir, "status")

    if "auf demselben Stand" in statusCheck:
        print("Status check passes.")
        print("Code up to date.")
        return False
    else:
        print("Code update available.")
        return True

def ProcessFetch(char, stdin):
    global aggregated

    sys.stdout.flush()
    aggregated += char
    if aggregated.endswith("Password for 'https://yourrepo@bitbucket.org':"):
        print(mainLogger, "Entering password...", True)
        stdin.put("yourpassword\n")

if __name__ == "__main__":
    checkTimeSec = 60
    while True:
        print("*********** Checking for code update **************")
        if CheckForUpdate(gitDir):
            print("Resetting code...")
            resetCheck = git("--git-dir=" + gitDir + ".git/", "--work-tree=" + gitDir, "reset", "--hard", "origin/" + branch)
            print(str(resetCheck))
        print("Check complete. Waiting for " + str(checkTimeSec) + "seconds until next check...", True)
        time.sleep(checkTimeSec)
