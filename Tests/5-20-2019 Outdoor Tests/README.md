## Outdoor Testing 12/20/2018

#### Testing Setup and Protocol
Today's equipment setup and automatic data retrieval program went smoothly as no programming or visible hardware issues occurred. The commented automatic retrieval program "testingscriptnew.py" correctly logged in angles of arrival at user specified times, without having to reset any part of the system after 90 degree beacon synchronization. The testing for both conventional beamforming and other methods went as followed (person 1 - Russell, person 2 - Professor Skovira): 

1. Person 2 places the CC1310 and its transmitter rig at 90 degrees. Person 2 presses reset on the CC1310.
2. Person 1 waits for 10 seconds ((gives time for person 2 to move far away from the CC1310). Person 1 then starts the GNU Radio Angle of Arrival program and the python data analysis program at the same time. The GNU Radio program automatically corrects for sampling offsets on startup using the noise signal sent from the CC1310.
3. Person 1 waits for the CC1310 to switch transmission to a sinusoidal signal (the CC1310 sends a noise signal for 20 seconds after reset or power cycle).
4. Once a sinusoid is seen on the GNU Radio GUI, person 1 presses the phase calibration button on the GNU Radio GUI to correct for phase offsets.
5. Person 1 waits 20 seconds and then starts data collection on the data collection program (so that the program has 15 seconds of uninterrupted data to record from).
6. Person 2 moves the transmitter rig to 45 degrees. Person 1 waits for person 2 to move away and then waits for 20 additional seconds.
7. Person 1 starts data collection for 45 degrees now.
8. Person 2 moves the transmitter rig to 0 degrees. Person 1 waits for person 2 to move away and then waits for 20 additional seconds.
9. Person 1 starts data collection for 0 degrees now.
10. End Trial

#### Results
There were two trials of conventional beamforming and they are specified by the title "2ant0trialx" where the second number in the title refers to the expected angle. The ending numbers listed in the trial titles of other methods also refer to the expected angle.

Conventional Beamforming Results -
  2ant0trial1.txt 	
	2ant0trial2.txt 	
	2ant45trial1.txt 	
	2ant45trial2.txt 	
	2ant90trial1.txt 
	2ant90trial2.txt 	
  
MUltiple Signal Classification (MUSIC) Results -
	music0.txt 
	music45.txt 
	music90.txt 	
  
MUSIC with Forward Backwards Spatial Smoothing Results -
	musicsmoothing0.txt 
	musicsmoothing45.txt 	
	musicsmoothing90.txt 	
  
Root MUSIC Results -
	rootmusic0.txt 
	rootmusic45.txt 	
	rootmusic90.txt 	
	
#### Discussion
The results from all the methods tested reflected poorly on the system's accuracy, but demonstrated precision. There were no observations of random bimodal phase shifts after phase synchronization at 90 degrees. Some variations of MUSIC; however, demonstrated large fluctuation between samples for certain angles. I believe the system would benefit from further testing that would monitor how the accuracy of angle of arrival depreciates as the transmitter moves angularly away from 90 degrees at the same receiver-transmitter distance, and whether the error occurs is logarithmic, exponential, etc. as this is happening. Then the software and hardware can be further debugged and improved based on those results.
