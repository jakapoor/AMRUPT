## Conventional Beamforming Program:

The two-antenna element flowchart computes an angle of arrival from the phase difference of two received signals (originating from a common, continuous, and non phase fluctuating signal source) at antenna elements with a normalized pairwise distance denoted by "alpha". The in-phase and quadrature values of the two received signals at RTL SDRs are extracted in the program by using an RTL-SDR source block for each RTL SDRs. The in-phase and quadrature values from the two RTL-SDR sources are sent through virtual sinks which sends the raw I/Q to a cross correlation calculation for timing offset corrections followed by phase difference correction/analysis. The cross correlation is performed in the sample offset blocks which computes a delay between the data channels based on a 3 iteration FFT convolution in the frequency domain. The delay is then applied by the delay blocks. An important note is that the number of iterations specified in the sample offset blocks cannot be set too high, or the processing demands of this block will cause a loss of timing synchronicity especially when comparing more than 2 data streams. 

Once timing offsets are corrected for, the two RTL SDR datastreams are interpolated to include 1 out of every 50 samples for further DSP analysis. 100 samples are placed in a vector after this interpolation and sent to a block called “PCA XCORR.” The “PCA XCORR” block code here uses Capon Beamforming to compute a phase difference between the signals received by two RTL SDRs. This block was found to produce a very stable phase difference between two RTL SDRs when a common sinusoidal signal was supplied to the receivers (less than 0.1 radian variation). 

Once a phase difference is reliably computed, a phase offset can be corrected by pushing a button named “phase_calibration” on a GNU Radio GUI during runtime. The phase_calibration button initiates the phase offset calculation which computes the numerical separation of the observed phase difference from an expected value of zero (e.g. a 90 degree beacon is expected to yield a phase difference of zero). Afterwards, the phase difference registered by “PCA XCORR” during calibration will be sent in a message format to the “Multiply Exp” block, which continuously adjusts the phasors of one of the two datastreams according to the phase offset calculation. After the synchronization procedure is complete, the Angle of Arrival is computed by the “Phase to XCORR” block using the calculation DOA = arccos(phase/(2*pi*alpha)) where alpha is determined by the antenna spacing and the wavelength of the received signal. 

File Extension Key -

.grc -> Flowchart File

.png -> Flowchart Image

.py or .cc -> Custom Block Code
