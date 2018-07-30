#!/bin/bash

#Switch to noise source
LD_LIBRARY_PATH=/usr/local/lib rtl_biast -d0 -a 0x40 -v 0x1f;
./extract.sh & 
taskset -c 1 ./fileclean.sh &
sleep 6

#Cross-correlate data streams
peaks=()
while read line
do
	peaks+=($line)
done < <(taskset -c 2 python cross-correlate.py 100000 500000 100000 500000 100000 500000 100000 500000)
echo ${peaks[@]}

sleep 6

#Data Stream(s) x, Boundary(b) x
s1b1=100000;
s1b2=500000;
s2b1=$((100000-${peaks[0]}-${peaks[0]}+50000));
s2b2=$((500000-${peaks[0]}-${peaks[0]}+50000));
s3b1=$((100000-${peaks[1]}-${peaks[1]}+50000));
s3b2=$((500000-${peaks[1]}-${peaks[1]}+50000));
s4b1=$((100000-${peaks[2]}-${peaks[2]}+50000));
s4b2=$((500000-${peaks[2]}-${peaks[2]}+50000));

python cross-correlate.py ${s1b1} ${s1b2} ${s2b1} ${s2b2} ${s3b1} ${s3b2} ${s4b1} ${s4b2}

#Switch back to antennas
LD_LIBRARY_PATH=/usr/local/lib rtl_biast -d0 -a 0x40 -v 0x00;

sleep 6
killall extract.sh
killall rtl_sdr
killall fileclean.sh

#Clear Files
> rtl1.dat
> rtl2.dat
> rtl3.dat
> rtl4.dat

exit;