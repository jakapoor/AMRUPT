#!/bin/bash

#Switch to noise source
LD_LIBRARY_PATH=/usr/local/lib rtl_biast -d0 -a 0x40 -v 0x1f;

#Collect data streams from each receiver
LD_LIBRARY_PATH=/usr/local/lib rtl_sdr -d1 -f 93500000 -g 35 -s 2300000 -n 2000000 -N rtl1.dat &
LD_LIBRARY_PATH=/usr/local/lib rtl_sdr -d2 -f 93500000 -g 35 -s 2300000 -n 2000000 -N rtl2.dat &
LD_LIBRARY_PATH=/usr/local/lib rtl_sdr -d3 -f 93500000 -g 35 -s 2300000 -n 2000000 -N rtl3.dat &
LD_LIBRARY_PATH=/usr/local/lib rtl_sdr -d4 -f 93500000 -g 35 -s 2300000 -n 2000000 -N rtl4.dat;

#Cross-correlate data streams
peaks=()
while read line
do
	peaks+=($line)
done < <(python cross-correlate.py 100000 500000 100000 500000 100000 500000 100000 500000)
echo ${peaks[@]}

#Data Stream(s) x, Boundary(b) x
s1b1=1000000;
s1b2=1400000;
s2b1=$((1000000-${peaks[0]}-${peaks[0]}+50000));
s2b2=$((1400000-${peaks[0]}-${peaks[0]}+50000));
s3b1=$((1000000-${peaks[1]}-${peaks[1]}+50000));
s3b2=$((1400000-${peaks[1]}-${peaks[1]}+50000));
s4b1=$((1000000-${peaks[2]}-${peaks[2]}+50000));
s4b2=$((1400000-${peaks[2]}-${peaks[2]}+50000));

python cross-correlate.py ${s1b1} ${s1b2} ${s2b1} ${s2b2} ${s3b1} ${s3b2} ${s4b1} ${s4b2}

#Switch back to antennas
LD_LIBRARY_PATH=/usr/local/lib rtl_biast -d0 -a 0x40 -v 0x00;

#Clear Files
> rtl1.dat
> rtl2.dat
> rtl3.dat
> rtl4.dat
exit;