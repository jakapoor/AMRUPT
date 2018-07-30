#!/bin/bash

set -o pipefail

#Clear Files
> rtla.dat
> rtlb.dat
> rtlc.dat
> rtld.dat

mkfifo pipe1
mkfifo pipe2
mkfifo pipe3
mkfifo pipe4
mkfifo pipe5
mkfifo pipe6
mkfifo pipe7
mkfifo pipe8

trap "" SIGPIPE

#Switch to noise source
LD_LIBRARY_PATH=/usr/local/lib rtl_biast -d0 -a 0x40 -v 0x1f;
sleep 0.2

#Collect data streams from each receiver
LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d1 -f 93500000 -g 35 -s 2300000 -N - | (cat > pipe1 || cat > pipe5) &
LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d2 -f 93500000 -g 35 -s 2300000 -N - | (cat > pipe2 || cat > pipe6) &
LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d3 -f 93500000 -g 35 -s 2300000 -N - | (cat > pipe3 || cat > pipe7) &
LD_LIBRARY_PATH=/usr/local/lib taskset -c 3 rtl_sdr -d4 -f 93500000 -g 35 -s 2300000 -N - | (cat > pipe4 || cat > pipe8) &

sleep 1

echo "mark1"

#Acquire data streams
cat < pipe1 | head -c 2000000 > rtla.dat &
cat < pipe2 | head -c 2000000 > rtlb.dat &
cat < pipe3 | head -c 2000000 > rtlc.dat &
cat < pipe4 | head -c 2000000 > rtld.dat &

sleep 1

echo "mark2"

#Cross-correlate data streams
peaks=()
while read line
do
	peaks+=($line)
done < <(taskset -c 2 python cross-correlate.py 100000 500000 100000 500000 100000 500000 100000 500000)
echo ${peaks[@]}

#Data Stream(s) x, Boundary(b) x
s1b1=1000000;
s1b2=1500000;
s2b1=$((1000000-${peaks[0]}-${peaks[0]}+50000));
s2b2=$((1500000-${peaks[0]}-${peaks[0]}+50000));
s3b1=$((1000000-${peaks[1]}-${peaks[1]}+50000));
s3b2=$((1500000-${peaks[1]}-${peaks[1]}+50000));
s4b1=$((1000000-${peaks[2]}-${peaks[2]}+50000));
s4b2=$((1500000-${peaks[2]}-${peaks[2]}+50000));

#Noise Switching for Testing Purposes
LD_LIBRARY_PATH=/usr/local/lib rtl_biast -d0 -a 0x40 -v 0x00;
sleep 10

echo "here1"

#Acquire data streams
cat < pipe5 | head -c 2000000 > rtla.dat &
cat < pipe6 | head -c 2000000 > rtlb.dat &
cat < pipe7 | head -c 2000000 > rtlc.dat &
cat < pipe8 | head -c 2000000 > rtld.dat &

echo "here2"

taskset -c 2 python cross-correlate.py ${s1b1} ${s1b2} ${s2b1} ${s2b2} ${s3b1} ${s3b2} ${s4b1} ${s4b2}

#Switch back to antennas
LD_LIBRARY_PATH=/usr/local/lib rtl_biast -d0 -a 0x40 -v 0x00;

sleep 2

killall rtl_sdr

#Clear Files
> rtla.dat
> rtlb.dat
> rtlc.dat
> rtld.dat

rm pipe1
rm pipe2
rm pipe3
rm pipe4

exit;