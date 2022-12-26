#!/bin/sh
make all

./L1 > a.log
./L2 > b.log 
./LLC > c.log

./a.py a.log 'L1 cache hit' 1 0.9999
./a.py b.log 'L1 cache miss' 2 0.99
./a.py c.log 'LLC miss' 3 0.95
