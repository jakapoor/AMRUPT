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
    print("Welcome to the AMRUPT Data Collection Program for two antenna pairs.")
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
        actual_AoA1 = str(raw_input('Expected AoA for antenna pair 1: '))
	actual_AoA2 = str(raw_input('Expected AoA for antenna pair 2: '))
        
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
        f.write("Expected Angle of Arrival antenna pair 1: " + actual_AoA1 + "\n\n")
	f.write("Expected Angle of Arrival antenna pair 2: " + actual_AoA2 + "\n\n")
        
        f.write ("=================================\n")
        f.write ("Chosen AoA Data from Antenna Pairs\n")
        f.write ("=================================\n")

        f.write ("\n=================================\n")
        f.write (str(starta) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract antenna pair 1 AoA data from file
        a = np.fromfile("pair1_AoA_data.txt", dtype=np.dtype("f"))
        a = a[(starta*sample_rate):(starta*sample_rate + num_samples)]  
        # AoA data is float
        a = a.astype(np.float32).view(np.float32)

	# Extract antenna pair 2 AoA data from file
        b = np.fromfile("pair2_AoA_data.txt", dtype=np.dtype("f"))
        b = b[(starta*sample_rate):(starta*sample_rate + num_samples)]  
        # AoA data is float
        b = b.astype(np.float32).view(np.float32)
	
	#Increments through every 10 samples of AoA values from antenna
	#pairs 1 and 2.
	for i in range(len(a)/10):
		# data segments are from index numbers increment_value*10 to
		# (increment_value+1)*10
		a_segment = a[i*10:(i+1)*10]
		b_segment = b[i*10:(i+1)*10]
		#mean value from 10 sample segment
		a_mean=np.mean(a_segment)
		b_mean=np.mean(b_segment)
		#variance value from 10 sample segment
		a_var=np.var(a_segment)
		b_var=np.var(b_segment)
		#More than 200x variance indicates that one pair has much more
		#imprecision than the other. Variance is tested first, because
		#the more inaccurate antenna pair could have a mean value closer
		#to 90 if it switches from 0 to 180 degrees for example (which
		#has been observed in simulated tests)
		if (a_var > 200*b_var):
			# Write data to file
        		f = open(filename, 'a')
        		np.savetxt(f, b_segment, "%s")
			f.write ("antenna pair " + str(2) + "\n")
        		f.close()
		elif (b_var > 200*a_var):
			# Write data to file
        		f = open(filename, 'a')
        		np.savetxt(f, a_segment, "%s")
			f.write ("antenna pair " + str(1) + "\n")
        		f.close()
		#Else, decide optimal AoA value solely using the mean values. 
		#AoA values closer to 90 degrres are more likely to be accurate.
		elif (np.absolute(a_mean - 90) < np.absolute(b_mean - 90)):
			# Write data to file
        		f = open(filename, 'a')
        		np.savetxt(f, a_segment, "%s")
			f.write ("antenna pair " + str(1) + "\n")
        		f.close()
		else:
			# Write data to file
        		f = open(filename, 'a')
        		np.savetxt(f, b_segment, "%s")
			f.write ("antenna pair " + str(2) + "\n")
        		f.close()

        f = open(filename, "a")

        f.write ("\n=================================\n")
        f.write (str(startb) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract antenna pair 1 AoA data from file
        a = np.fromfile("pair1_AoA_data.txt", dtype=np.dtype("f"))
        a = a[(startb*sample_rate):(startb*sample_rate + num_samples)]  
        # AoA data is float
        a = a.astype(np.float32).view(np.float32)

	# Extract antenna pair 2 AoA data from file
        b = np.fromfile("pair2_AoA_data.txt", dtype=np.dtype("f"))
        b = b[(startb*sample_rate):(startb*sample_rate + num_samples)]  
        # AoA data is float
        b = b.astype(np.float32).view(np.float32)
	
	#Increments through every 10 samples of AoA values from antenna
	#pairs 1 and 2.
	for i in range(len(a)/10):
		# data segments are from index numbers increment_value*10 to
		# (increment_value+1)*10
		a_segment = a[i*10:(i+1)*10]
		b_segment = b[i*10:(i+1)*10]
		#mean value from 10 sample segment
		a_mean=np.mean(a_segment)
		b_mean=np.mean(b_segment)
		#variance value from 10 sample segment
		a_var=np.var(a_segment)
		b_var=np.var(b_segment)
		#More than 200x variance indicates that one pair has much more
		#imprecision than the other. Variance is tested first, because
		#the more inaccurate antenna pair could have a mean value closer
		#to 90 if it switches from 0 to 180 degrees for example (which
		#has been observed in simulated tests)
		if (a_var > 200*b_var):
			# Write data to file
        		f = open(filename, 'a')
        		np.savetxt(f, b_segment, "%s")
			f.write ("antenna pair " + str(2) + "\n")
        		f.close()
		elif (b_var > 200*a_var):
			# Write data to file
        		f = open(filename, 'a')
        		np.savetxt(f, a_segment, "%s")
			f.write ("antenna pair " + str(1) + "\n")
        		f.close()
		#Else, decide optimal AoA value solely using the mean values. 
		#AoA values closer to 90 degrres are more likely to be accurate.
		elif (np.absolute(a_mean - 90) < np.absolute(b_mean - 90)):
			# Write data to file
        		f = open(filename, 'a')
        		np.savetxt(f, a_segment, "%s")
			f.write ("antenna pair " + str(1) + "\n")
        		f.close()
		else:
			# Write data to file
        		f = open(filename, 'a')
        		np.savetxt(f, b_segment, "%s")
			f.write ("antenna pair " + str(2) + "\n")
        		f.close()

        f = open(filename, "a")

        f.write ("\n=================================\n")
        f.write (str(startc) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract antenna pair 1 AoA data from file
        a = np.fromfile("pair1_AoA_data.txt", dtype=np.dtype("f"))
        a = b[(startc*sample_rate):(startc*sample_rate + num_samples)]  
        # AoA data is float
        a = b.astype(np.float32).view(np.float32)

	# Extract antenna pair 2 AoA data from file
        b = np.fromfile("pair2_AoA_data.txt", dtype=np.dtype("f"))
        b = a[(startc*sample_rate):(startc*sample_rate + num_samples)]  
        # AoA data is float
        b = a.astype(np.float32).view(np.float32)
	
	#Increments through every 10 samples of AoA values from antenna
	#pairs 1 and 2.
	for i in range(len(a)/10):
		# data segments are from index numbers increment_value*10 to
		# (increment_value+1)*10
		a_segment = a[i*10:(i+1)*10]
		b_segment = b[i*10:(i+1)*10]
		#mean value from 10 sample segment
		a_mean=np.mean(a_segment)
		b_mean=np.mean(b_segment)
		#variance value from 10 sample segment
		a_var=np.var(a_segment)
		b_var=np.var(b_segment)
		#More than 200x variance indicates that one pair has much more
		#imprecision than the other. Variance is tested first, because
		#the more inaccurate antenna pair could have a mean value closer
		#to 90 if it switches from 0 to 180 degrees for example (which
		#has been observed in simulated tests)
		if (a_var > 200*b_var):
			# Write data to file
        		f = open(filename, 'a')
        		np.savetxt(f, b_segment, "%s")
			f.write ("antenna pair " + str(2) + "\n")
        		f.close()
		elif (b_var > 200*a_var):
			# Write data to file
        		f = open(filename, 'a')
        		np.savetxt(f, a_segment, "%s")
			f.write ("antenna pair " + str(1) + "\n")
        		f.close()
		#Else, decide optimal AoA value solely using the mean values. 
		#AoA values closer to 90 degrres are more likely to be accurate.
		elif (np.absolute(a_mean - 90) < np.absolute(b_mean - 90)):
			# Write data to file
        		f = open(filename, 'a')
        		np.savetxt(f, a_segment, "%s")
			f.write ("antenna pair " + str(1) + "\n")
        		f.close()
		else:
			# Write data to file
        		f = open(filename, 'a')
        		np.savetxt(f, b_segment, "%s")
			f.write ("antenna pair " + str(2) + "\n")
        		f.close()

        f = open(filename, "a")

	f.write ("\n=================================\n")
        f.write ("AoA Data Antenna Pair 1\n")
        f.write ("=================================\n")

        f.write ("\n=================================\n")
        f.write (str(starta) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract AoA data from file
        a = np.fromfile("pair1_AoA_data.txt", dtype=np.dtype("f"))
        a = a[(starta*sample_rate):(starta*sample_rate + num_samples)]  
        # AoA data is float
        a = a.astype(np.float32).view(np.float32)
        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

        f = open(filename, "a")

        f.write ("\n=================================\n")
        f.write (str(startb) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract AoA data from file
        a = np.fromfile("pair1_AoA_data.txt", dtype=np.dtype("f"))
        a = a[(startb*sample_rate):(startb*sample_rate + num_samples)]   
        # AoA data is float
        a = a.astype(np.float32).view(np.float32)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

        f = open(filename, "a")

        f.write ("\n=================================\n")
        f.write (str(startc) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract AoA data from file
        a = np.fromfile("pair1_AoA_data.txt", dtype=np.dtype("f"))
        a = a[(startc*sample_rate):(startc*sample_rate + num_samples)]  
        # AoA data is float
        a = a.astype(np.float32).view(np.float32)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

        f = open(filename, "a")

	f.write ("\n=================================\n")
        f.write ("AoA Data Antenna Pair 2\n")
        f.write ("=================================\n")

        f.write ("\n=================================\n")
        f.write (str(starta) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract AoA data from file
        a = np.fromfile("pair2_AoA_data.txt", dtype=np.dtype("f"))
        a = a[(starta*sample_rate):(starta*sample_rate + num_samples)]  
        # AoA data is float
        a = a.astype(np.float32).view(np.float32)
        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

        f = open(filename, "a")

        f.write ("\n=================================\n")
        f.write (str(startb) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract AoA data from file
        a = np.fromfile("pair2_AoA_data.txt", dtype=np.dtype("f"))
        a = a[(startb*sample_rate):(startb*sample_rate + num_samples)]   
        # AoA data is float
        a = a.astype(np.float32).view(np.float32)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

        f = open(filename, "a")

        f.write ("\n=================================\n")
        f.write (str(startc) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract AoA data from file
        a = np.fromfile("pair2_AoA_data.txt", dtype=np.dtype("f"))
        a = a[(startc*sample_rate):(startc*sample_rate + num_samples)]  
        # AoA data is float
        a = a.astype(np.float32).view(np.float32)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

        f = open(filename, "a")

        f.write ("\n==========================\n")
        f.write ("Phase Difference Data Antenna Pair 1\n")
        f.write ("==========================\n")

        f.write ("\n=================================\n")
        f.write (str(starta) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract Phase Difference data from file
        a = np.fromfile("pair1_Phasediff_data.txt", dtype=np.dtype("f"))
        a = a[(starta*sample_rate):(starta*sample_rate + num_samples)] 
        # Phase difference data is float
        a = a.astype(np.float32).view(np.float32)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

        f = open(filename, "a")

        f.write ("\n=================================\n")
        f.write (str(startb) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract Phase Difference data from file
        a = np.fromfile("pair1_Phasediff_data.txt", dtype=np.dtype("f"))
        a = a[(startb*sample_rate):(startb*sample_rate + num_samples)] 
        # Phase difference data is float
        a = a.astype(np.float32).view(np.float32)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

        f = open(filename, "a")

        f.write ("\n=================================\n")
        f.write (str(startc) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract Phase Difference data from file
        a = np.fromfile("pair1_Phasediff_data.txt", dtype=np.dtype("f"))
        a = a[(startc*sample_rate):(startc*sample_rate + num_samples)]    
        # Phase difference data is float
        a = a.astype(np.float32).view(np.float32)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

        f = open(filename, "a")

	f.write ("\n==========================\n")
        f.write ("Phase Difference Data Antenna Pair 2\n")
        f.write ("==========================\n")

        f.write ("\n=================================\n")
        f.write (str(starta) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract Phase Difference data from file
        a = np.fromfile("pair2_Phasediff_data.txt", dtype=np.dtype("f"))
        a = a[(starta*sample_rate):(starta*sample_rate + num_samples)] 
        # Phase difference data is float
        a = a.astype(np.float32).view(np.float32)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

        f = open(filename, "a")

        f.write ("\n=================================\n")
        f.write (str(startb) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract Phase Difference data from file
        a = np.fromfile("pair2_Phasediff_data.txt", dtype=np.dtype("f"))
        a = a[(startb*sample_rate):(startb*sample_rate + num_samples)] 
        # Phase difference data is float
        a = a.astype(np.float32).view(np.float32)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

        f = open(filename, "a")

        f.write ("\n=================================\n")
        f.write (str(startc) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract Phase Difference data from file
        a = np.fromfile("pair2_Phasediff_data.txt", dtype=np.dtype("f"))
        a = a[(startc*sample_rate):(startc*sample_rate + num_samples)]    
        # Phase difference data is float
        a = a.astype(np.float32).view(np.float32)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

        f = open(filename, "a")

        f.write ("\n==========================\n")
        f.write ("IQ Data RTL SDR 1 Antenna Pair 1\n")
        f.write ("==========================\n")

        f.write ("\n=================================\n")
        f.write (str(starta) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        #Offset in samples must be even to acquire correct IQ Data
        if (adc_sample_rate % 2 == 1):
            adc_sample_rate = adc_sample_rate + 1

        # Extract IQ data from file
        a = np.fromfile("pair1_IQ_data_rtl1.txt", dtype=np.dtype("u1"))
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

        f.write ("\n=================================\n")
        f.write (str(startb) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract IQ data from file
        a = np.fromfile("pair1_IQ_data_rtl1.txt", dtype=np.dtype("u1"))
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

        f.write ("\n=================================\n")
        f.write (str(startc) + " Seconds\n")
        f.write ("=================================\n\n")
        f.close()

        # Extract IQ data from file
        a = np.fromfile("pair1_IQ_data_rtl1.txt", dtype=np.dtype("u1"))
        a = a[(startc*adc_sample_rate):(startc*adc_sample_rate + (num_samples * 2))] 
        # IQ data is complex
        a = a.astype(np.float32).view(np.complex64)
        # Allow for negative I/Q
        a -= (127.4 + 127.4j)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

	f = open(filename, "a")

        f.write ("\n==========================\n")
        f.write ("IQ Data RTL SDR 2 Antenna Pair 1\n")
        f.write ("==========================\n")

        f.write ("\n=================================\n")
        f.write (str(starta) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        #Offset in samples must be even to acquire correct IQ Data
        if (adc_sample_rate % 2 == 1):
            adc_sample_rate = adc_sample_rate + 1

        # Extract IQ data from file
        a = np.fromfile("pair1_IQ_data_rtl2.txt", dtype=np.dtype("u1"))
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

        f.write ("\n=================================\n")
        f.write (str(startb) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract IQ data from file
        a = np.fromfile("pair1_IQ_data_rtl2.txt", dtype=np.dtype("u1"))
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

        f.write ("\n=================================\n")
        f.write (str(startc) + " Seconds\n")
        f.write ("=================================\n\n")
        f.close()

        # Extract IQ data from file
        a = np.fromfile("pair1_IQ_data_rtl2.txt", dtype=np.dtype("u1"))
        a = a[(startc*adc_sample_rate):(startc*adc_sample_rate + (num_samples * 2))] 
        # IQ data is complex
        a = a.astype(np.float32).view(np.complex64)
        # Allow for negative I/Q
        a -= (127.4 + 127.4j)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

	f = open(filename, "a")

        f.write ("\n==========================\n")
        f.write ("IQ Data RTL SDR 1 Antenna Pair 2\n")
        f.write ("==========================\n")

        f.write ("\n=================================\n")
        f.write (str(starta) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        #Offset in samples must be even to acquire correct IQ Data
        if (adc_sample_rate % 2 == 1):
            adc_sample_rate = adc_sample_rate + 1

        # Extract IQ data from file
        a = np.fromfile("pair2_IQ_data_rtl1.txt", dtype=np.dtype("u1"))
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

        f.write ("\n=================================\n")
        f.write (str(startb) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract IQ data from file
        a = np.fromfile("pair2_IQ_data_rtl1.txt", dtype=np.dtype("u1"))
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

        f.write ("\n=================================\n")
        f.write (str(startc) + " Seconds\n")
        f.write ("=================================\n\n")
        f.close()

        # Extract IQ data from file
        a = np.fromfile("pair2_IQ_data_rtl1.txt", dtype=np.dtype("u1"))
        a = a[(startc*adc_sample_rate):(startc*adc_sample_rate + (num_samples * 2))] 
        # IQ data is complex
        a = a.astype(np.float32).view(np.complex64)
        # Allow for negative I/Q
        a -= (127.4 + 127.4j)

        # Write data to file
        f = open(filename, 'a')
        np.savetxt(f, a, "%s")
        f.close()

	f = open(filename, "a")

        f.write ("\n==========================\n")
        f.write ("IQ Data RTL SDR 2 Antenna Pair 2\n")
        f.write ("==========================\n")

        f.write ("\n=================================\n")
        f.write (str(starta) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        #Offset in samples must be even to acquire correct IQ Data
        if (adc_sample_rate % 2 == 1):
            adc_sample_rate = adc_sample_rate + 1

        # Extract IQ data from file
        a = np.fromfile("pair2_IQ_data_rtl2.txt", dtype=np.dtype("u1"))
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

        f.write ("\n=================================\n")
        f.write (str(startb) + " Seconds\n")
        f.write ("=================================\n\n")

        f.close()

        # Extract IQ data from file
        a = np.fromfile("pair2_IQ_data_rtl2.txt", dtype=np.dtype("u1"))
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

        f.write ("\n=================================\n")
        f.write (str(startc) + " Seconds\n")
        f.write ("=================================\n\n")
        f.close()

        # Extract IQ data from file
        a = np.fromfile("pair2_IQ_data_rtl2.txt", dtype=np.dtype("u1"))
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
