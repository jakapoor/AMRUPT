#!/bin/bash

while true
do
	sleep 3
	cp rtla.dat rtl1.dat
	cp rtlb.dat rtl2.dat
	cp rtlc.dat rtl3.dat
	cp rtld.dat rtl4.dat
	#Clear Files
	truncate --size 0 rtla.dat rtlb.dat rtlc.dat rtld.dat
done

exit;