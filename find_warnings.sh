#!/bin/bash
gcc -Wall $1 -o test 2> warning.txt
grep warning:  warning.txt | awk -F warning:\  '{print $1$2}' > result.txt
echo $?
if [ $? -eq 0 ]
	then 
	a=1
	while [ $a -le $2 ] 
	do
		echo $a 1|./test >> result.txt
		let a++
	done
fi
pwd >> result.txt
