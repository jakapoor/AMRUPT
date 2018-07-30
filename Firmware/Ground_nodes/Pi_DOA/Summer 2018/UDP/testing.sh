#!/bin/bash

LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d1 -f 93500000 -g 35 -s 2300000 -N - | nc -q 100 -b -u localhost 12346 

exit;