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
	index = 1;
	for filename in files:
		a = np.fromfile(filename, dtype=np.dtype("u1"))
		a = a[int(sys.argv[index]):int(sys.argv[index + 1])]   # take short block
		a = a.astype(np.float32).view(np.complex64)
		a -= (127.4 + 127.4j)
		signals.append(a)
		index = index + 2
	
	for i in range(3):
		sig1 = signals[0]
		sig2 = signals[1+i]
	
		corr = signal.fftconvolve(sig1, np.conj(sig2[::-1]))
		
		peakpos = np.argmax(np.abs(corr))
		
		print str(peakpos) 
		
		#plt.subplot(3,1,1+i)
		#plt.plot(np.abs(corr))
	
	#plt.show()

if __name__ == '__main__':
	main()
