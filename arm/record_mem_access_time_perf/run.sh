#!/bin/sh
make all

sudo setcap cap_sys_admin+ep L1
sudo setcap cap_sys_admin+ep L2

./L1 > a.log
./L2 > b.log 

