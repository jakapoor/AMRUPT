## Simulation Tests 4/8/2019

#### System Improvements
1. When conducting preliminary simulated tests with the coherent receiver, only some of the bias tees could be activated on the rtl sdrs and the noise source could not be deactivated when powered by the usb hub. These and other new issues never observed before occured during the simulated tests. Fortunately, all of these observed issues were solved by replacing the existing usb multiport hub with a [higher power anker multiport usb hub](https://www.amazon.com/Anker-PowerIQ-Charging-Macbook-Surface/dp/B00VDVCQ84). 

2. The conventional beamforming GNU Radio programs and python data extraction program (testingscriptnew.py) were updated to include file sinks to both RTL SDR channels involved, so that AoA values can be recalculated using raw I/Q samples.

#### Testing Setup and Protocol
The goal of the simulated tests was to test the functionality of the conventional beamforming software to correctly detect angles corresponding to phase differences of simulated radio waves. The simulated testing setup from [3-1-2019](https://github.com/jakapoor/AMRUPT/tree/master/Tests/3-1-2019%20Simulation%20Tests) was extended to include timing/phase offset correction and compatibility to the coherent receiver setup. 

A PRBS-15 signal acting as a noise source was sent from a CC1310 into two RTL SDRs on the coherent receiver for timing offset correction. After 20 seconds, the CC1310 would then send a sinusoidal wave to both RTL SDRs for 90 degree simulation following phase offset correction. After the 90 degree simulation, the program would test AoA calculations for 60, 45, 30, and 0 degree simulated angles of arrival. Each simulated angle of arrival would be tested for 20 seconds in order for the python data extraction program to have enough data for each angle.

In order to create a phase offset between the split data channels, the Multiply EXP GNU Radio block was used to continuously adjust the phasor of one of the data channels to have a specific phase difference. Phase offset arguments of 3.14159, 2.72070, 2.22144, 1.57080, and 0.00000 radians were used corresponding to 0, 30, 45, 60, and 90 degree angles of arrival respectively. Phase offset arguments were calculated from the AoA formula arccos(phase_difference/(2*pi*alpha)) where alpha is equal to wavelength/antenna_seperation.

#### Results
Five tests with three trials were performed for each AoA using the sinusoidal signal from the CC1310. In these tests, alpha values used in calculating AoA were adjusted to 0.45, 0.48, 0.5, 0.52, and 0.55 to see how the program would react to inaccuracy in antenna seperation. The results of each test were processed automatically into txt files with names denoting the ExpectedAoAValue (e.g."angle0") and the alpha value (e.g."alpha0_45" for 0.45). In addition, 0.5 alpha tests were performed for each AoA using the PRBS-15 signal from the CC1310. These tests are denoted by "noise" at the end of the test name. Because of the promising results of these tests, the PRBS-15 signal and sinusoidal signal were tested again for a 15 degree angle of arrival for an alpha value of 0.5.
	
#### Discussion
These results show similar levels of accuracy to the 3-1-2019 simulated tests for angles >= 30 degrees, even when alpha values were changed to simulate inexact antenna spacings. However, the 0 degree results show much greater error and imprecision to the 3-1-2019 simulated tests with all alpha values. A possible solution to this in the future would be to discard lower angles of arrival in a triangular array. 

Another observation is that angle of arrival differences of +/- 10 degrees occured when moving the wires connecting the CC1310 to the coherent receiver. The will probably be less problematic outdoors since the wires connecting the antennas to the coherent receiver in this scenario are spaced further apart and are more taut.

Simulation tests with the PRBS-15 signal yielded unexpected high accuracy/high precision angle of arrival results. Comparisons between these tests and the sinusoidal tests reveal that the PRBS-15 had higher precision, but AoA accuracy tapers off sooner as the simulated AoA reduces. Because the sinusoidal signal is less precise, phase offset correction can yield starting AoAs that are +/- 1-2 degrees off from 90 degrees. This adversely effects lower angle of arrival calculations which are more sensitive than 90 degrees. A moving average can help fix this problem by averaging phase difference during phase offset correction. 
