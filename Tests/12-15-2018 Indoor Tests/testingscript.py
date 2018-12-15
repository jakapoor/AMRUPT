#!/usr/bin/python
# Script to automatically collect AoA, Phase Difference, and IQ Data from GNU Radio generated binary files during AoA Testing
# The number of samples sent to the GNU Radio file sink blocks is 5,000,000 samples to accomodate the highest possible data sampling rate (2.4 Msps * 2 + 1000)
# 2.4 Msps -> Highest ADC Sampling from RTL SDR with no dropped samples
# The head blocks in GNU Radio are used to set the sampling size of each file sink block
#
# Example arguments for program -> python testingscript.py "testoutput.txt" 1000000 200 90 "testing this program" 
# 200 phasediff/AoA sample rate chosen for 1000000 Msps ADC sampling rate and Keep 1 in 50 Samples and 100 Vector Length (1000000/50/100)
#
# Data acquisition occurs for samples at the 15 second mark or above.
# AoA, phasediff, and I/Q values do not necessarily correspond to each other to the exact sample
#
# Each sample set is defined by the number of samples accrued in 2.5 seconds by the lowest data acquisition samples rate
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
    adc_sample_rate = int(sys.argv[2]) #can be modified to include downsampled I/Q measurements
    sample_rate = int(sys.argv[3])
    actual_AoA = str(sys.argv[4])

    num_samples = int(sample_rate * 2.5)

    starta = 15
    startb = 20
    startc = 25

    timestamp = datetime.datetime.now()
    formatted_timestamp = timestamp.strftime('%Y-%m-%d %H:%M:%S')
    
    #Creates a file with the given filename and formats the heading
    f = open(filename, "w+")
    f.write(filename + "\n")
    f.write(formatted_timestamp + "\n")
    f.write("Sampling Rate of ADC for IQ Data Acquisition: " + str(adc_sample_rate) + "\n")
    f.write("Sampling Rate of Phase Difference/AoA Calculations: " + str(sample_rate) + "\n") 
    f.write("Observed Angle of Arrival: " + actual_AoA + "\n\n")
    
    f.write ("=================================\n")
    f.write ("AoA Data\n")
    f.write ("=================================\n")

    f.write ("\n=================================\n")
    f.write (str(starta) + " Seconds\n")
    f.write ("=================================\n\n")

    f.close()

    # Extract AoA data from file
    a = np.fromfile("AoA_data.txt", dtype=np.dtype("f"))
    a = a[(starta*sample_rate):(starta*sample_rate + num_samples)]  
    # AoA data is float
    a = a.astype(np.float32).view(np.float32)
    # Write data to file
    f = open(filename, 'a')
    np.savetxt(f, a, "%s")
    f.close()

    f = open(filename, "a")

    f.write ("\n\n=================================\n")
    f.write (str(startb) + " Seconds\n")
    f.write ("=================================\n\n")

    f.close()

    # Extract AoA data from file
    a = np.fromfile("AoA_data.txt", dtype=np.dtype("f"))
    a = a[(startb*sample_rate):(startb*sample_rate + num_samples)]   
    # AoA data is float
    a = a.astype(np.float32).view(np.float32)

    # Write data to file
    f = open(filename, 'a')
    np.savetxt(f, a, "%s")
    f.close()

    f = open(filename, "a")

    f.write ("\n\n=================================\n")
    f.write (str(startc) + " Seconds\n")
    f.write ("=================================\n\n")

    f.close()

    # Extract AoA data from file
    a = np.fromfile("AoA_data.txt", dtype=np.dtype("f"))
    a = a[(startc*sample_rate):(startc*sample_rate + num_samples)]  
    # AoA data is float
    a = a.astype(np.float32).view(np.float32)

    # Write data to file
    f = open(filename, 'a')
    np.savetxt(f, a, "%s")
    f.close()

    f = open(filename, "a")

    f.write ("\n\n==========================\n")
    f.write ("Phase Difference Data\n")
    f.write ("==========================\n")

    f.write ("\n=================================\n")
    f.write (str(starta) + " Seconds\n")
    f.write ("=================================\n\n")

    f.close()

    # Extract Phase Difference data from file
    a = np.fromfile("Phasediff_data.txt", dtype=np.dtype("f"))
    a = a[(starta*sample_rate):(starta*sample_rate + num_samples)] 
    # Phase difference data is float
    a = a.astype(np.float32).view(np.float32)

    # Write data to file
    f = open(filename, 'a')
    np.savetxt(f, a, "%s")
    f.close()

    f = open(filename, "a")

    f.write ("\n\n=================================\n")
    f.write (str(startb) + " Seconds\n")
    f.write ("=================================\n\n")

    f.close()

    # Extract Phase Difference data from file
    a = np.fromfile("Phasediff_data.txt", dtype=np.dtype("f"))
    a = a[(startb*sample_rate):(startb*sample_rate + num_samples)] 
    # Phase difference data is float
    a = a.astype(np.float32).view(np.float32)

    # Write data to file
    f = open(filename, 'a')
    np.savetxt(f, a, "%s")
    f.close()

    f = open(filename, "a")

    f.write ("\n\n=================================\n")
    f.write (str(startc) + " Seconds\n")
    f.write ("=================================\n\n")

    f.close()

    # Extract Phase Difference data from file
    a = np.fromfile("Phasediff_data.txt", dtype=np.dtype("f"))
    a = a[(startc*sample_rate):(startc*sample_rate + num_samples)]    
    # Phase difference data is float
    a = a.astype(np.float32).view(np.float32)

    # Write data to file
    f = open(filename, 'a')
    np.savetxt(f, a, "%s")
    f.close()

    f = open(filename, "a")

    f.write ("\n\n==========================\n")
    f.write ("IQ Data\n")
    f.write ("==========================\n")

    f.write ("\n=================================\n")
    f.write (str(starta) + " Seconds\n")
    f.write ("=================================\n\n")

    f.close()

    #Offset in samples must be even to acquire correct IQ Data
    if (adc_sample_rate % 2 == 1):
    	adc_sample_rate = adc_sample_rate + 1

    # Extract IQ data from file
    a = np.fromfile("IQ_data.txt", dtype=np.dtype("u1"))
    a = a[(starta*adc_sample_rate):(starta*adc_sample_rate + (num_samples * 2))] 
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
    f.write (str(startb) + " Seconds\n")
    f.write ("=================================\n\n")

    f.close()

    # Extract IQ data from file
    a = np.fromfile("IQ_data.txt", dtype=np.dtype("u1"))
    a = a[(startb*adc_sample_rate):(startb*adc_sample_rate + (num_samples * 2))] 
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
    f.write (str(startc) + " Seconds\n")
    f.write ("=================================\n\n")
    f.close()

    # Extract IQ data from file
    a = np.fromfile("IQ_data.txt", dtype=np.dtype("u1"))
    a = a[(startc*adc_sample_rate):(startc*adc_sample_rate + (num_samples * 2))] 
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
