## Outdoor Testing 12/19/2018

Unfortunately outdoor testing could not be performed due to two issues:

1. My new bluetooth mouse turned on my laptop without notification a couple of hours before testing, and my laptop was drained at the time of testing. Because of this, the Back-UPS power supply drained quickly from supplying power to my laptop in addition to the coherent receiver/raspberry pi.

2. I needed to program the CC1310 to send noise and then a continuous wave (it was only sending a continuous wave at the time of testing). The night before, I created a shortcut to a large portion of the files on my laptop. This inadvertently moved all of these files (including the CC1310 files) to a unrecognizable location. This has been fixed by restoring all of the files to their original locations.

Preventative measures that will be taken to avoid these problems in the future:

1. Completely shutdown the laptop when it is charged to full battery, so that no vampire power is consumed before outdoor testing. Also, bring an extra portable power supply for the Raspberry Pi so the main power unit can focus on supplying power to the coherent receiver.

2. Make sure the CC1310 is programmed correctly before outdoor testing (along with any equipment other than the Raspberry Pi that requires a specific program). Also, make sure all of the necessary files are in place right before conducting testing. The CC1310 files have also been backed up on GitHub [here](https://github.com/jakapoor/AMRUPT/tree/master/Software/Angle%20of%20Arrival/Transmitter%20Testing%20Protocol%20Code).
