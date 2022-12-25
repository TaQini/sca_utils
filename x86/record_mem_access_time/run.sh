#!/bin/sh
gcc a.c -o a
./a > a.log
./a.py a.log
