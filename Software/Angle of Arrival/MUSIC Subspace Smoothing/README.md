## MUSIC Subspace Smoothing Program:

MUSIC is a subspace technique that estimates an angle of arrival based on the roots of a polynomial determined by the eigenvector analysis of a sensor array correlation matrix. 

In this flowchart, the same synchronization procedure performed for two RTL SDRs in the conventional beamforming program is applied to three RTL SDR pairs. Tming offsets are computed using three sample offset blocks which send delay values respective to the RTL SDR combination (delay1, delay2, and delay3). Variables listed as “Function Probe” hold these values. After timing offsets are corrected between the RTL SDR channels, phase offsets are corrected using a similar procedure to the Conventional Beamforming program.  

File Extension Key -

.grc -> Flowchart File

.png -> Flowchart Image

.py or .cc -> Custom Block Code
