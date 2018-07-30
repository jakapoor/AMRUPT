#!/usr/bin/python
# script to test cross correlating saved blocks containing only coherent noise

import sys
import math
import cmath
import matplotlib.pyplot as plt
import numpy as np
from scipy import signal

def main():
	# files having only one received block
	files = ["rtl1.dat", "rtl2.dat", "rtl3.dat", "rtl4.dat"]
	signals = []
	for filename in files:
		a = np.fromfile(filename, dtype=np.dtype("u1"))
		a = a[100000:600000]   # take short block
		a = a.astype(np.float32).view(np.complex64)
		a -= (127.4 + 127.4j)
		signals.append(a)
	
if __name__ == '__main__':
	main()
