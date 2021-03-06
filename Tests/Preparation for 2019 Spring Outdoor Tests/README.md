## Preparation for 2019 Outdoor Tests

#### Introduction
It was concluded that conventional beamforming is more prone to inaccuracy at AoAs parallel to the antenna array (e.g. 0 and 180 degrees) from the april simulation tests. To combat this source of inaccuracy, a two-antenna pair system has been proposed. Each antenna pair is perpendicular to each other, so when one antenna pair collects inaccurate AoAs at more parallel angle (e.g. 0 degrees), the other antenna pair collects more accurate AoAs at a more perpendicular angle (e.g. 90 degrees) to the transmitter. Of course there are caveats to this proposal including an increased power consumption and a lower TDMA switching rate (from the requirement to listen to each tagged individual for a longer time to determine the more optimal antenna pair). For now, I think this is the best approach to demonstrate a proof of concept system which can determine which antenna pair is more accurate. Furthermore, the efficiency and accuracy of this system could be improved in a 3-antenna triangular array. The triangular array is not currently proposed due to the need for triangulation (each antenna pair has a different angular reference). 

#### Testing Scenario
Professor Skovira and I are planning to conduct the outdoor tests next week. Four primary tests have been proposed for this date - a whip antenna pair test (2 antennas), a rubber ducky antenna pair test (2 antennas), a two-antenna pair test with the optimal antenna type (4 antennas), and a movement test (4 antennas). The movement test will consist of one tester running around the transmitter to simulate multipath interference. The specific testing procedure will be similar to the one [here](https://github.com/jakapoor/AMRUPT/tree/master/Tests/12-20-2018%20Outdoor%20Tests). Angles 0, 15, 45, and 90 will be tested. Some other logistics are still being worked out for the tests including whether a sinusoid or PRBS-15 noise signal should be transmitted from the CC1310 and whether to seperate the two antenna pairs by height to avoid possible interference.

#### Updated Data Extraction Python Code
The updated data extraction python code has been uploaded to this folder. The code generates 9 sections of data: Optimal AoA values chosen by the system, AoA values from antenna pair 1, AoA values from antenna pair 2, phase difference values from antenna pair 1, phase difference values from antenna pair 2, IQ data from receiver 1-antenna pair 1, IQ data from receiver 2-antenna pair 1, IQ data from receiver 1-antenna pair 2, and IQ data from receiver 2-antenna pair 2. The program also lists the expected angle of arrival from each antenna pair.

The code listed below is used in order to determine the optimal AoA from each antenna pair where data segment a is extracted from antenna pair 1 and data segment b is extracted from antenna pair 2. The code computes the optimal AoA from the mean and variance values of 10-sample segments of AoAs taken from each antenna pair. From the observations of the simulated tests, low simulated angles could cause a high degree of imprecision that could result in a high mean of AoA values. This condition is checked first before checking the means to determine the optimal AoA. If the variance between the antenna pairs is not drastically different, the mean is used to choose the antenna pair with the  AoA values closer to 90 degrees.
```
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
```
