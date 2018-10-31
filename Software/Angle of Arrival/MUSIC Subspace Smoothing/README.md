## MUSIC Subspace Smoothing Program:

MUSIC is a subspace technique that estimates an angle of arrival based on the roots of a polynomial determined by the eigenvector analysis of a sensor array correlation matrix. 

In this flowchart, the same synchronization procedure performed for two RTL SDRs in the conventional beamforming program is applied to three RTL SDR pairs. Tming offsets are computed using three sample offset blocks which send delay values respective to the RTL SDR combination (delay1, delay2, and delay3). Variables listed as “Function Probe” hold these values. After timing offsets are corrected between the RTL SDR channels, phase offsets are corrected using a similar procedure to the Conventional Beamforming program.  

During root MUSIC testing the following protocol would be performed on the user end for timing and phase offset correction. The RF signal generator was turned off and the noise source was turned on by a separate terminal before the start of the program. On startup, the program computes and applies timing delays between the channels, which would cause the phase difference display on the GUI with the noise source on to steady to a phase difference with +/- 0.2 radian jittering. Afterwards, the noise source was turned off, and a common sinusoidal signal source was fed into the first two receivers on the coherent receivers. The phase_calibration1 button was pressed to correct for a phase offset when this common sinusoid is sent to RTL SDR 1&2. The same is then done for RTL SDR 1&3 and RTL SDR 1&4. Finally, the RF signal generator is turned off and the four antennas are connected to each receiver accordingly. The GUI Compass/float sink can then be monitored to record AoA values. 

File Extension Key -

.grc -> Flowchart File

.png -> Flowchart Image

.py or .cc -> Custom Block Code
