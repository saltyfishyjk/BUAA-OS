#!/bin/bash
# assignment for lab0
sed -n '8p' $1 1>$2
sed -n '32p' $1 1>>$2
sed -n '128p' $1 1>>$2
sed -n '512p' $1 1>>$2
sed -n '1024p' $1 1>>$2
