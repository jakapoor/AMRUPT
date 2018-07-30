#!/bin/bash

nc -u -l -dk localhost 12346 | head -c 4000000 > test.dat &
sleep 3
echo "hello" 

exit;