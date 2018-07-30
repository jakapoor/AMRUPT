#!/bin/bash

rtl_test -d1 -s 2300000 &
rtl_test -d2 -s 2300000 &
rtl_test -d3 -s 2300000 &
rtl_test -d4 -s 2300000;
exit;