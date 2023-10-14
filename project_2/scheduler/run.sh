./scheduler BATCH homogeneous.txt
./scheduler SJF homogeneous.txt
./scheduler RR 1000 homogeneous.txt

./scheduler BATCH reverse.txt
./scheduler SJF reverse.txt
./scheduler RR 1000 reverse.txt

./scheduler PRIO 500 mixed.txt
