#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/pi/gr-xcorr/python
export PATH=/home/pi/gr-xcorr/build/python:$PATH
export LD_LIBRARY_PATH=/home/pi/gr-xcorr/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/pi/gr-xcorr/build/swig:$PYTHONPATH
/usr/bin/python2 /home/pi/gr-xcorr/python/qa_variance_ff.py 
