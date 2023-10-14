# Zikos Spyridon 1084581
# Kyriakoulopoulos Kallinikos 1084583
# Makris Orestis-Antonis 1084516

#!/bin/bash

filename=$1

mining_username () {
		awk '{ print $3 }' $filename | sort | uniq -c > user_counter.txt
		}

case $# in

0) echo "1084581|1084583|1084516"
   echo "1084581|1084583|1084516" > AM.txt;;
   
1) awk '{print NR,$0}'  $1
   awk '{print $0}'  $1 > access_copy.log
   ;;
   
2)  if [ "$2" = "--servprot" ]; then
		echo "Wrong Network Protocol" > server_protocol.txt
		echo "Wrong Network Protocol" 
	fi
	
	if [ "$2" = "--method" ]; then
		echo "Wrong Method Name" > server_protocol.txt
		echo "Wrong Method Name" 
	fi
	
	if [ "$2" = "--datum" ]; then
		echo "Wrong Date" > date.txt
		echo "Wrong Date" 
	fi
	
	if [ "$2" = "--usrid" ]; then
		mining_username
		awk '{print $0}' user_counter.txt
	fi
	
	if [ "$2" = "--browsers" ]; then
		count_browsers () {
	
			awk -F'"' '{print $6}' $1 > output1.txt
			awk '{
        			for(i=1; i<=NF; i++) {
                			temp=match($i, /[a-zA-Z]+[/]/)
                			if(temp) {
                        			print $i
                			}
        			}
			}' output1.txt > output2.txt
			awk -F '/' '{print $1}' output2.txt | sort | uniq -c > output1.txt
			awk '{print $2, $1}' output1.txt > output2.txt
			awk '{print $2, $1}' output1.txt 
}
	count_browsers $filename
	fi
	;;

3) 	
	if [ "$2" = "--usrid" ]; then
		mining_username
		if grep -Ewq "$3" user_counter.txt; then
			awk '/'$3' -/ {print $0 }' $1 > with_username.txt
			awk '/'$3' -/ {print $0 }' $1 
		else 
			echo "Username Not Found" > with_username.txt 
			echo "Username Not Found" 
		fi
	fi
	
	if [ "$2" = "--servprot" ]; then
		if [ "$3" = "IPv4" ]; then
			awk '/^1/ {print $0 }' $1 > server_protocol.txt
			awk '/^1/ {print $0 }' $1 
		elif [ "$3" = "IPv6" ]; then
			awk '/::/ {print $0 }' $1 > server_protocol.txt
			awk '/::/ {print $0 }' $1
		else 
			echo "Wrong Network Protocol"
			echo "Wrong Network Protocol" > server_protocol.txt
		fi
	fi
	
	if [ "$2" = "--method" ]; then
		if [[ "$3" == "GET" ]]; then
			awk '/GET/ {print $0 }' $1 > method_name.txt
			awk '/GET/ {print $0 }' $1
		elif [[ "$3" == "POST" ]]; then
			awk '/POST/ {print $0 }' $1 > method_name.txt
			awk '/POST/ {print $0 }' $1
		else 
			echo "Wrong Method Name"
			echo "Wrong Method Name" > method_name.txt
		fi
    fi
    
    if [ "$2" = "--datum" ]; then
		if [ "$3" = "Jan" ] || 
		   [ "$3" = "Feb" ] ||
		   [ "$3" = "Mar" ] ||
		   [ "$3" = "Apr" ] ||
		   [ "$3" = "May" ] ||
		   [ "$3" = "Jun" ] ||
		   [ "$3" = "Jul" ] ||
		   [ "$3" = "Aug" ] ||
		   [ "$3" = "Sep" ] ||
		   [ "$3" = "Oct" ] ||
	   	   [ "$3" = "Nov" ] ||
		   [ "$3" = "Dec" ]; then
			awk '/'$3'/ {print $0 }' $1 > date.txt
			awk '/'$3'/ {print $0 }' $1 
		else
			echo "Wrong Date" > date.txt
			echo "Wrong Date"
		fi
    fi ;;
esac