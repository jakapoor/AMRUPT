These are the Ettus custom block files used inside the GRC files. 

autocorrelation_impl.cc is the source code for the autocorrelation block. It takes in an integer parameters inputs (number of RTL-SDR sources), snapshot_size (directly correlated with rate limiting - see GRC readme), overlap_size (length of complex vector it takes in at each iteration), and avg_method - can be forward-backwards or forward averaging. This computes a correlation matrix using the input paramets by using the conjugate of a certain snapshot interval over other snapshot intervals. 

MUSIC_lin_array_impl.cc is the source code for the MUSIC lin array block. See the GRC readme for the parameters it takes in.

find_local_max_impl.cc is the source code for the find local max block. It takes in parameters num_max_vals (number of maximum values that should be computed), vector_len (inputted pseudo-spectrum), x_min, and x_max. This finds peak locations of the psuedo-spectrum outputted by MUSIC_lin_array by iterating overy the spectrum and finding indices at which peaks occur. The values of peak indices are sorted, so that the sorted array can be used to output how the number of maximums needed (determined by num_max_vals). For our implementation, we set num_max_vals = 1, therefore one global maximum is found which corresponds the AoA of the received RF signal.
