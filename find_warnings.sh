#!/bin/bash
gcc -Wall $1 -o test 2> warning.txt
grep warning:  warning.txt | awk -F warning:\  '{print $1$2}' > result.txt
if [ $? -eq 0 ]
then 
a=1
while [ $a -ne $2 ]
do
a=$[$a+1]
done
fi
pwd >> result.txt
