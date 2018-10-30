/* -*- c++ -*- */

#define XCORR_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "xcorr_swig_doc.i"

%{
#include "xcorr/capon_ccf.h"
#include "xcorr/unwrap_ff.h"
#include "xcorr/linearslope_ff.h"
#include "xcorr/mode_ii.h"
#include "xcorr/lin_delay_cc.h"
#include "xcorr/peak_estimator_cif.h"
#include "xcorr/cub_delay_cc.h"
#include "xcorr/cumulative_ff.h"
#include "xcorr/variance_ff.h"
#include "xcorr/zero_pad_cc.h"
#include "xcorr/sample_offset_cci.h"
#include "xcorr/hold_ii.h"
#include "xcorr/hold_ff.h"
#include "xcorr/full_capon_ccf.h"
#include "xcorr/full_capon3_ccf.h"
#include "xcorr/multiply_exp_cc.h"
#include "xcorr/pi2pi_ff.h"
#include "xcorr/delay.h"
#include "xcorr/message_sink_i.h"
#include "xcorr/message_sink_f.h"
#include "xcorr/add_const_ff.h"
#include "xcorr/phase2xcorr_ff.h"
#include "xcorr/hold_state_ff.h"
%}


%include "xcorr/capon_ccf.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, capon_ccf);

%include "xcorr/unwrap_ff.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, unwrap_ff);
%include "xcorr/linearslope_ff.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, linearslope_ff);

%include "xcorr/mode_ii.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, mode_ii);

%include "xcorr/lin_delay_cc.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, lin_delay_cc);
%include "xcorr/peak_estimator_cif.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, peak_estimator_cif);
%include "xcorr/cub_delay_cc.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, cub_delay_cc);

%include "xcorr/cumulative_ff.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, cumulative_ff);
%include "xcorr/variance_ff.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, variance_ff);

%include "xcorr/zero_pad_cc.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, zero_pad_cc);
%include "xcorr/sample_offset_cci.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, sample_offset_cci);
%include "xcorr/hold_ii.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, hold_ii);
%include "xcorr/hold_ff.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, hold_ff);
%include "xcorr/full_capon_ccf.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, full_capon_ccf);
%include "xcorr/full_capon3_ccf.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, full_capon3_ccf);

%include "xcorr/multiply_exp_cc.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, multiply_exp_cc);
%include "xcorr/pi2pi_ff.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, pi2pi_ff);

%include "xcorr/delay.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, delay);
%include "xcorr/message_sink_i.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, message_sink_i);
%include "xcorr/message_sink_f.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, message_sink_f);
%include "xcorr/add_const_ff.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, add_const_ff);
%include "xcorr/phase2xcorr_ff.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, phase2xcorr_ff);
%include "xcorr/hold_state_ff.h"
GR_SWIG_BLOCK_MAGIC2(xcorr, hold_state_ff);
