#!/bin/sh

if [ $# -lt 1 ]
then
    echo "Missing target executable"
    exit 1
fi

if [ $# -eq 2 ]
then
    gcc -g3 -static $1 -o $2
else
    gcc -static $1
fi
