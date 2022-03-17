#!/bin/bash
make all
touch hello_os
mkdir hello_os_dir
cp os_hello hello_os_dir/
cp os_hello hello_os_dir/hello_os
rm os_hello
grep -n -i os_hello hello_os.c > hello_os.txt
