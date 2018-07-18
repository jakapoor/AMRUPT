Angle of arrival measurements from multiple Raspberry Pi basestations are transmitted to a central computing unit for radio tag triangulation 

The Triangulation Algorithm folder contains psuedocode that will be very similar to the triangulation algorithm used in the final design which will be used to track two dimensional positions of radio tags once accurate angle of arrivals have been achieved.

The following is an overview of the functionality of the scripts in the Triangulation folder:
[From Silva 7/17/2018]
"cc-tests performs two cross correlations at different windows (the window values were changed during testing) in order to test the synchronicity of data streams after cross correlation. cc-tests only does post-processing for a finite sampling period. main.sh contains the naive approach towards continuous sampling with cross correlation. main.sh calls extract.sh which continuously extracts samples (extract.sh has a dedicated core). main.sh also calls fileclean.sh which copies each file to a separate file (in which post processing will be performed). After copying the files, fileclean.sh clears all four files at the same time. main.sh then performs cross correlation twice on the copied files (just like cc-tests)."
