#!/bin/bash

./testing | ./circularbuffer.py &

#compare current time values to file's time values
echo $(date +"%s")
sleep 5
echo $(date +"%s")
sleep 5
echo $(date +"%s")
sleep 5
echo $(date +"%s")
sleep 5

exit;