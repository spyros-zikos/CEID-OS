PROJECT TITLE: Scheduler

PURPOSE OF PROJECT: Implement a scheduler which takes as input the applications that it has to execute and schedules their execution using appropriate data structures and according to certain algorithms.

DATE: Jan 2023

AUTHORS: Spyros, Kallinikos, Orestis

USER INSTRUCTIONS:
Go to "scheduler" directory.
Run from linux terminal:
-> sudo su
-> gcc scheduler.c -o scheduler -lm
  For single file tests:
  -> ./scheduler FCFS reverse.txt
  -> ./scheduler SJF reverse.txt
  -> ./scheduler RR 1000 reverse.txt
  -> ./scheduler PRIO 500 reverse.txt
  For multiple tests:
  -> chmod +x run.sh
  -> ./run.sh


Note: If the documentation is not displayed clearly, try opening it with a web browser.