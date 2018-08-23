#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/pi/gr-xcorr/lib
export PATH=/home/pi/gr-xcorr/build/lib:$PATH
export LD_LIBRARY_PATH=/home/pi/gr-xcorr/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-xcorr 
