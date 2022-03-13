#!bin/bash
#First you can use grep (-n) to find the number of lines of string.
grep -n $2 $1 | awk -F: '{print $1}'> $3
