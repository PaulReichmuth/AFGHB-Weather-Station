#!/usr/bin/python3
# Code originally made by John Grant
# https://medium.com/@johngrant/updating-distributed-raspberrypis-with-automatic-code-updates-3336aa0bcca1
import sh
from sh import git
import time
import os, sys
import argparse
import requests
import logging
import datetime

logger = logging.getLogger()
logger.setLevel(logging.DEBUG)

ch = logging.StreamHandler()
ch.setLevel(logging.INFO)
logger.addHandler(ch)

fh = logging.FileHandler('logs/Updater.log')
fh.setLevel(logging.WARNING)
logger.addHandler(fh)



parser = argparse.ArgumentParser(description='Update a git directory from a selected branch')
parser.add_argument('branch', metavar='branch', type=str ,help='branch to update from')
parser.add_argument('dir', metavar='directory', type=str ,help='directory to update', default="/WeatherStation/")



aggregated = ""
branch = args.branch
gitDir = args.dir

def telegram_notify(bot_message):
    bot_token = '1198116152:AAGnSQK62eVTi1Lz1D5_8SHXkhJ_q0tF_4U'
    bot_chatID = '413144530'
    send_text = 'https://api.telegram.org/bot' + bot_token + '/sendMessage?chat_id=' + bot_chatID + '&parse_mode=Markdown&text=' + bot_message

    response = requests.get(send_text)

    return response.json()

def CheckForUpdate(workingDir):
    logger.info("Fetching most recent code from source..." + workingDir)

    # Fetch most up to date version of code.
    p = git("--git-dir=" + workingDir + ".git/", "--work-tree=" + workingDir, "fetch", "origin", branch, _out=ProcessFetch, _out_bufsize=0, _tty_in=True)
    logger.info("Fetch complete.")
    time.sleep(2)
    logger.info("Checking status for " + workingDir + "...")
    statusCheck = git("--git-dir=" + workingDir + ".git/", "--work-tree=" + workingDir, "status")

    if "auf demselben Stand" in statusCheck:
        logger.info("Status check passes.")
        logger.info("Code up to date.")
        return False
    else:
        logger.warning(str(datetime.datetime.now()) + ": Code update available.")
        return True

def ProcessFetch(char, stdin):
    global aggregated

    sys.stdout.flush()
    aggregated += char
    if aggregated.endswith("Password for 'https://yourrepo@bitbucket.org':"):
        print(mainLogger, "Entering password...", True)
        stdin.put("yourpassword\n")

if __name__ == "__main__":
    logger.info("*********** Checking for code update **************")
    if CheckForUpdate(gitDir):
        logger.info("Resetting code...")
        resetCheck = git("--git-dir=" + gitDir + ".git/", "--work-tree=" + gitDir, "reset", "--hard", "origin/" + branch)
        logger.warning(str(datetime.datetime.now()) + " : " +str(resetCheck))
        message = "Update applied:" + "\n" + str(resetCheck).strip("HEAD ist jetzt bei")
        telegram_notify(message)
    logger.info("Check complete.)
