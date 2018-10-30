#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/pi/gr-doa/python
export PATH=/home/pi/gr-doa/build/python:$PATH
export LD_LIBRARY_PATH=/home/pi/gr-doa/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/pi/gr-doa/build/swig:$PYTHONPATH
/usr/bin/python2 /home/pi/gr-doa/python/qa_calibrate_lin_array.py 
