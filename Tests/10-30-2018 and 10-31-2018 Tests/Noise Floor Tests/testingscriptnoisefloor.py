#!/usr/bin/python
# Script to automatically collect IQ Data from GNU Radio generated binary file for Noise Floor Analysis during AoA Testing
#
# Example arguments for program -> python testingscript.py "noisefloorA1B1.txt" 1000000
# 1000000 chosen because of 1000000 Msps ADC sampling rate 
#
# Author: Russell Silva

import sys
import math
import cmath
import numpy as np
import time
import datetime

def main():
    # files having only one received block
    filename = str(sys.argv[1])
    adc_sample_rate = int(sys.argv[2])

    timestamp = datetime.datetime.now()
    formatted_timestamp = timestamp.strftime('%Y-%m-%d %H:%M:%S')
    
    #Creates a file with the given filename and formats the heading
    f = open(filename, "w+")
    f.write("NOISE FLOOR AMPLITUDE\n")
    f.write(filename + "\n")
    f.write(formatted_timestamp + "\n")
    f.write("Sampling Rate of ADC for IQ Data Acquisition: " + str(adc_sample_rate) + "\n")

    f.write ("\n\n==========================\n")
    f.write ("IQ Data\n")
    f.write ("==========================\n")

    f.write ("\n=================================\n")
    f.write ("1st 1000 samples of second 1\n")
    f.write ("=================================\n\n")

    f.close()

    # Extract IQ data from file
    a = np.fromfile("IQ_data.txt", dtype=np.dtype("u1"))
    # First 0 - 1000 Samples of Second 1
    a = a[0:2000] 
    # IQ data is complex
    a = a.astype(np.float32).view(np.complex64)
    # Allow for negative I/Q
    a -= (127.4 + 127.4j)

    # Write data to file
    f = open(filename, 'a')
    np.savetxt(f, a, "%s")
    f.close()

    f = open(filename, "a")

    f.write ("\n\n=================================\n")
    f.write ("1st 1000 samples of second 2\n")
    f.write ("=================================\n\n")

    f.close()

    #Offset in samples must be even to acquire correct IQ Data
    if (adc_sample_rate % 2 == 1):
    	adc_sample_rate = adc_sample_rate + 1

    # Extract IQ data from file
    a = np.fromfile("IQ_data.txt", dtype=np.dtype("u1"))
    # First 0 - 1000 Samples of Second 2
    a = a[adc_sample_rate:(adc_sample_rate + 2000)]  
    # IQ data is complex
    a = a.astype(np.float32).view(np.complex64)
    # Allow for negative I/Q
    a -= (127.4 + 127.4j)

    # Write data to file
    f = open(filename, 'a')
    np.savetxt(f, a, "%s")
    f.close()

    f = open(filename, 'a')

    f.write ("\n\n=================================\n")
    f.write ("1st 1000 samples of second 3\n")
    f.write ("=================================\n\n")

    f.close()

    # Extract IQ data from file
    a = np.fromfile("IQ_data.txt", dtype=np.dtype("u1"))
    # First 0 - 1000 Samples of Second 3
    a = a[(2*adc_sample_rate):(2*adc_sample_rate + 2000)] 
    # IQ data is complex
    a = a.astype(np.float32).view(np.complex64)
    # Allow for negative I/Q
    a -= (127.4 + 127.4j)

    # Write data to file
    f = open(filename, 'a')
    np.savetxt(f, a, "%s")
    f.close()

if __name__ == '__main__':
	main()
