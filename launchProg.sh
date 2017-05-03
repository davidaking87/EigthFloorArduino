#!/bin/bash
# ^^^ Says this file = bash script
# Is called via: /etc/rc.local
#
# Idea: If the SD card is plugged in when the robot turns on, start the program.
# 
# Sites:
#     if/else example: https://www.systemcodegeeks.com/shell-scripting/bash/bash-else-example/
#     variables: http://tldp.org/HOWTO/Bash-Prog-Intro-HOWTO-5.html
#     Calling scripts: http://stackoverflow.com/questions/8352851/how-to-call-shell-script-from-another-shell-script#8352939

folderpath="/media/0403-0201" # If sd card's folder exists...
script="/home/ubuntu/autoRun.sh" # Run this script

if [ -d $folderpath ]; then
	source $script
fi
