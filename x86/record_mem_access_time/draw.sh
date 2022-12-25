#!/bin/sh
./a.py a.log 'L1 cache hit' 1
./a.py b.log 'L1 cache miss' 2
./a.py c.log 'LLC miss' 3

