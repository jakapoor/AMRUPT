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

#Collect data streams from each receiver
LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d1 -f 93500000 -g 35 -s 2300000 -N rtla.dat &
LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d2 -f 93500000 -g 35 -s 2300000 -N rtlb.dat &
LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d3 -f 93500000 -g 35 -s 2300000 -N rtlc.dat &
LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d4 -f 93500000 -g 35 -s 2300000 -N rtld.dat;

exit;