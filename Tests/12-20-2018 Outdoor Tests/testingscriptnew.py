#!/usr/bin/python
# Script to automatically collect AoA, Phase Difference, and IQ Data from GNU Radio generated binary files during AoA Testing
#
# Data acquisition occurs for samples at the 15 second mark or above.
# AoA, phasediff, and I/Q values do not necessarily correspond to each other to the exact sample
#
# The three trials for a test occur at (elapsed_time - 10, elapsed_time - 5, elapsed_time)
#
# The sample size for each trial is determined by the number of samples accrued in 2.5 seconds 
# by the lowest data acquisition sample rate
#
# Author: Russell Silva

import sys
import math
import cmath
import numpy as np
import time
import datetime

def main():
    print("Welcome to the AMRUPT Data Collection Program.")
    print("The program collects data corresponding to a fifteen second window.")
    print("The time window ends with the elapsed time from program startup to when \"start\" is entered")
    print("After starting the program for the first time, please wait for 15 seconds before the first data collection.")

    start_time = time.time()

    while 1:  
        #Prompts the user to start data collection or end the program
        while 1:
            status = raw_input('\nEnter \"start\" to begin data collection or enter \"end\" to stop the program: ')
            if (status == "start"):
                elapsed_time = time.time() - start_time
                if (elapsed_time < 15.0):
                    print("\nPlease wait until 15 seconds has passed to start data collection.")
                else:
                    break
            elif (status == "end"):
                break

        if (status == "end"):
            break
    
        print("Data Recorded")
        filename = str(raw_input('File name: '))
        adc_sample_rate = int(raw_input('ADC Sampling Rate: ')) #can be modified to include downsampled I/Q measurements
        sample_rate = int(raw_input('AoA Sample Rate: '))
        actual_AoA = str(raw_input('Expected AoA: '))
        
        num_samples = int(sample_rate * 2.5)

        starta = int(elapsed_time - 10)
        startb = int(elapsed_time - 5)
        startc = int(elapsed_time)

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
