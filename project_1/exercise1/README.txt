PROJECT TITLE: Shell Scripting

PURPOSE OF PROJECT: Process a log file "access.log" using bash.

DATE: Jan 2023

AUTHORS: Spyros, Kallinikos, Orestis


USER INSTRUCTIONS:
Run from linux terminal:
-> sudo su
-> chmod +x logparser

-> ./logparser.sh

-> ./logparser.sh access.log

-> ./logparser.sh access.log --usrid
-> ./logparser.sh access.log --usrid root

-> ./logparser.sh access.log --method GET
-> ./logparser.sh access.log --method POST

-> ./logparser.sh access.log --servprot IPv4
-> ./logparser.sh access.log --servprot IPv6

-> ./logparser.sh access.log --browsers

-> ./logparser.sh access.log --datum Feb
