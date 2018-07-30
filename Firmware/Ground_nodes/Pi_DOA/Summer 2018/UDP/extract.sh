#!/bin/bash

#Clear Files
> rtla.dat
> rtlb.dat
> rtlc.dat
> rtld.dat

#Clear Files
> rtl1.dat
> rtl2.dat
> rtl3.dat
> rtl4.dat

neg=-1
#Collect data streams from each receiver
LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d1 -f 93500000 -g 35 -s 2300000 -N - | nc -q 100 -b -u localhost 12346 &
LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d2 -f 93500000 -g 35 -s 2300000 -N - | nc -q 100 -b -u localhost 12347 &
LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d3 -f 93500000 -g 35 -s 2300000 -N - | nc -q 100 -b -u localhost 12348 &
LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d4 -f 93500000 -g 35 -s 2300000 -N - | nc -q 100 -b -u localhost 12349;

exit;