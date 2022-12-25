#!/bin/sh
make all

./L1 > a.log
./L2 > b.log 

./a.py a.log 'L1 cache hit' 1
./a.py b.log 'L2 cache miss' 2
