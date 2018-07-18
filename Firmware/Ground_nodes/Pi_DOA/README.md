The Datalogging folder contains an overview of SPI, an embedded bus protocol commonly used to send data between microcontrollers and small peripheral devices. We intend to use SPI in the datalogging of timestamped radio tag positions.

The Demodulated IQ Sampling folder contains commands that are run on Ubuntu Terminal in order to extract the real and imaginary values of an incoming radio signal in parallel among coherent receivers, and write these values to a file.

The GNU Radio folder contains all files relevant to GNU Radio. GNU Radio is a software that utilizes signal processing blocks in order to perform DSP routines (e.g. cross correlation) on received radio signals. 

The following is an overview of the functionality of the scripts used for cross-correlation of IQ samples from different channels to achieve channel synchronization:

[From Silva 7/17/2018]
"cc-tests performs two cross correlations at different windows (the window values were changed during testing) in order to test the synchronicity of data streams after cross correlation. cc-tests only does post-processing for a finite sampling period. main.sh contains the naive approach towards continuous sampling with cross correlation. main.sh calls extract.sh which continuously extracts samples (extract.sh has a dedicated core). main.sh also calls fileclean.sh which copies each file to a separate file (in which post processing will be performed). After copying the files, fileclean.sh clears all four files at the same time. main.sh then performs cross correlation twice on the copied files (just like cc-tests)."
