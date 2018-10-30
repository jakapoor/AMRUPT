# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
install(FILES
    xcorr_capon_ccf.xml
    xcorr_timestamp_ccf.xml
    xcorr_unwrap_ff.xml
    xcorr_linearslope_ff.xml
    xcorr_mode_ii.xml
    xcorr_lin_delay_cc.xml
    xcorr_peak_estimator_cif.xml
    xcorr_cub_delay_cc.xml
    xcorr_cumulative_ff.xml
    xcorr_variance_ff.xml
    xcorr_zero_pad_cc.xml
    xcorr_sample_offset_cci.xml
    xcorr_hold_ii.xml
    xcorr_hold_ff.xml
    xcorr_full_capon_ccf.xml
    xcorr_full_capon3_ccf.xml
    xcorr_multiply_exp_cc.xml
    xcorr_pi2pi_ff.xml
    xcorr_arrow_f.xml
    xcorr_delay.xml
    xcorr_message_sink_i.xml
    xcorr_message_sink_f.xml
    xcorr_add_const_ff.xml
    xcorr_phase2xcorr_ff.xml
    xcorr_hold_state_ff.xml DESTINATION share/gnuradio/grc/blocks
)
