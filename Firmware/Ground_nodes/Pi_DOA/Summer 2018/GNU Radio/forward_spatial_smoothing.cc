/* -*- c++ -*- */
/*
 * Copyright 2016
 * Travis F. Collins <travisfcollins@gmail.com>
 * Srikanth Pagadarai <srikanth.pagadarai@gmail.com>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "forward_spatial_smoothing_impl.h"

#include <armadillo>
#define COPY_MEM false  // Do not copy matrices into separate memory
#define FIX_SIZE true   // Keep dimensions of matrices constant

namespace gr {
  namespace custom {

    forward_spatial_smoothing::sptr
    forward_spatial_smoothing::make(int inputs, int subarray_size)
    {
      return gnuradio::get_initial_sptr
        (new forward_spatial_smoothing_impl(inputs, subarray_size));
    } 

    /*
     * The private constructor
     */
    forward_spatial_smoothing_impl::forward_spatial_smoothing_impl(int inputs, int subarray_size)
      : gr::block("forward_spatial_smoothing",
              gr::io_signature::make(1, 1, sizeof(gr_complex)*subarray_size*subarray_size)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*inputs*inputs)),
      d_num_inputs(inputs),
      d_subarray_size(subarray_size),
    {
      num_outputs = inputs - subarray_size + 1;

	  if (num_outputs > inputs || num_outputs < 0)
		  throw std::invalid_argument("Invalid subarray size.");
    }

    /*
     * Our virtual destructor.
     */
    forward_spatial_smoothing_impl::~forward_spatial_smoothing_impl()
    {
    }

    int
    forward_spatial_smoothing_impl::general_work (int output_matrices,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      // Cast pointers
	  const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Create each output matrix
      for (int i=0; i<output_matrices; i++)
      {

		// make input pointer into matrix pointer
	    arma::cx_fmat in_matrix(in+i*d_subarray_size*d_subarray_size, d_subarray_size, d_subarray_size);

        // Make output pointer into matrix pointer
        arma::cx_fmat out_matrix(out+i*d_num_inputs*d_num_inputs*i,d_num_inputs,d_num_inputs,COPY_MEM,FIX_SIZE);

		out_matrix = in_matrix[0];
		// Form output matrix
		for (int k = 1; k<d_num_inputs; k++)
		{
			out_matrix = out_matrix + in_matrix[k]
		}
		out_matrix = (1.0 / d_num_inputs)*out_matrix
      }	

      // Tell runtime system how many output items we produced.
      return (output_matrices);
    }

  } /* namespace custom */
} /* namespace gr */
