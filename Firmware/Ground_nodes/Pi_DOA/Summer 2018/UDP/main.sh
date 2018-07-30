#!/bin/bash

#Switch to noise source
LD_LIBRARY_PATH=/usr/local/lib rtl_biast -d0 -a 0x40 -v 0x1f;
#./tcp.sh &
sleep 0.1
./extract.sh & 
#./fileclean.sh &
sleep 4

echo "alpha"

#Acquire data streams
nc -u -l localhost 12346 | head -c 4000000 > rtla.dat &
nc -u -l localhost 12347 | head -c 4000000 > rtlb.dat &
nc -u -l localhost 12348 | head -c 4000000 > rtlc.dat &
nc -u -l localhost 12349 | head -c 4000000 > rtld.dat &

echo "bravo"

#Cross-correlate data streams
peaks=()
while read line
do
	peaks+=($line)
done < <(taskset -c 2 python cross-correlate.py 100000 500000 100000 500000 100000 500000 100000 500000)
echo ${peaks[@]}

#Cut files down
#filesize=$(wc -c < rtla.dat)
#portion=$(bc -l <<< "$filesize * 0.8")
#portion=${portion%.*}
#portion=$((${filesize}-4000000))
#dd if=rtla.dat of=rtlatemp.dat ibs=$portion skip=1
#dd if=rtlb.dat of=rtlbtemp.dat ibs=$portion skip=1
#dd if=rtlc.dat of=rtlctemp.dat ibs=$portion skip=1
#dd if=rtld.dat of=rtldtemp.dat ibs=$portion skip=1
#> rtla.dat
#dd if=rtlatemp.dat of=rtla.dat ibs=1 skip=1
#> rtlb.dat
#dd if=rtlbtemp.dat of=rtlb.dat ibs=1 skip=1
#> rtlc.dat
#dd if=rtlctemp.dat of=rtlc.dat ibs=1 skip=1
#> rtld.dat
#dd if=rtldtemp.dat of=rtld.dat ibs=1 skip=1

#Data Stream(s) x, Boundary(b) x
s1b1=1000000;
s1b2=1500000;
s2b1=$((1000000-${peaks[0]}-${peaks[0]}+100000));
s2b2=$((1500000-${peaks[0]}-${peaks[0]}+100000));
s3b1=$((1000000-${peaks[1]}-${peaks[1]}+100000));
s3b2=$((1500000-${peaks[1]}-${peaks[1]}+100000));
s4b1=$((1000000-${peaks[2]}-${peaks[2]}+100000));
s4b2=$((1500000-${peaks[2]}-${peaks[2]}+100000));

#Acquire data streams
nc -u -l localhost 12346 | head -c 4000000 > rtla.dat &
nc -u -l localhost 12347 | head -c 4000000 > rtlb.dat &
nc -u -l localhost 12348 | head -c 4000000 > rtlc.dat &
nc -u -l localhost 12349 | head -c 4000000 > rtld.dat &
echo $(wc -c < rtla.dat)
echo "hello"

taskset -c 2 python cross-correlate.py ${s1b1} ${s1b2} ${s2b1} ${s2b2} ${s3b1} ${s3b2} ${s4b1} ${s4b2}

#Switch back to antennas
LD_LIBRARY_PATH=/usr/local/lib rtl_biast -d0 -a 0x40 -v 0x00;

sleep 1

killall extract.sh
killall rtl_sdr
killall fileclean.sh
killall tcp.sh

#Clear Files
> rtla.dat
> rtlb.dat
> rtlc.dat
> rtld.dat

exit;