## Angle of Arrival Software

This folder includes a general overview of the latest software implemented for the angle of arrival direction finding system. Each subfolder refers to a distinct program/method for generating angle of arrival results at receiver basestations. Currently, the only software implementations that can generate angle of arrival results are implemented in GNU Radio.

### IMPORTANT: 
THE FILES IN THIS FOLDER SHOULD BY NO MEANS BE USED DIRECTLY. These files are here to help explain the software, they cannot be downloaded and just ran like a stand-alone python program. In order to use these programs, load the flowchart in GNU Radio from a Raspberry Pi loaded with the OS Image "AMRUPT Raspbian." This image file is included in this folder as "AMRUPT Raspbian." Instructions for loading an OS image to the Raspberry Pi can be accessed in the following link - https://www.raspberrypi.org/documentation/installation/installing-images/. 

### GNU Radio
There are many resources to get started with GNU Radio. Notable resource here include -
1. https://wiki.gnuradio.org/index.php/Guided_Tutorial_GRC
2. https://wiki.gnuradio.org/index.php/OutOfTreeModules
3. https://www.gnuradio.org/blog/buffers

Within each subfolder is a GRC file associated with the flowchart for that program. Furthermore, the primary .cc files associated with each custom block/module in the program are included with the .cc file renamed to the corresponding block. This does not include the header, background, and installation programs that are unessential in understanding the software associated with the custom blocks/modules. Such programs can be found in the libraries gr-doa (library for music, subspace-smoothing), gr-xcorr (library for cross-correlation, phase difference calculation), and gr-osmosdr (library for RTL-SDR data extraction). These libraries have also been included here as additional folders accordingly.

Code for the blocks automatically included in GNU Radio (e.g. Add, QT GUI Sink, File Sink, Keep 1 in N, Stream to Vector) can be viewed inside the lib folder in the repository https://github.com/gnuradio/gnuradio/tree/master/gr-blocks 

### Modified Repositories
gr-doa: library modified from Ettus Research gr-doa repository - https://github.com/EttusResearch/gr-doa
gr-xcorr: library modified from Sam Whiting's gr-doa repository - https://github.com/samwhiting/gnuradio-doa/tree/master/gr-doa 
gr-osmosdr: library modified from Osmocom's gr-osmosdr repository - https://github.com/osmocom/gr-osmosdr 

This software was also based on the work documented here - https://coherent-receiver.com/getting-started
