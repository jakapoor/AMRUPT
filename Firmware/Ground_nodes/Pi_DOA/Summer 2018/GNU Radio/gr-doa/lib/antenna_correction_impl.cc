/* -*- c++ -*- */
/*
 * Copyright 2016
 * Srikanth Pagadarai <srikanth.pagadarai@gmail.com>
 * Travis F. Collins <travisfcollins@gmail.com>
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
#include "antenna_correction_impl.h"

#include <fstream>

#define COPY_MEM false  // Do not copy matrices into separate memory
#define FIX_SIZE true   // Keep dimensions of matrices constant

namespace gr {
  namespace doa {

    antenna_correction::sptr
    antenna_correction::make(int num_ant_ele, char* config_filename)
    {
      return gnuradio::get_initial_sptr
        (new antenna_correction_impl(num_ant_ele, config_filename));
    }

    /*
     * The private constructor
     */
    antenna_correction_impl::antenna_correction_impl(int num_ant_ele, char* config_filename)
      : gr::sync_block("antenna_correction",
              gr::io_signature::make(num_ant_ele, num_ant_ele, sizeof(gr_complex)),
              gr::io_signature::make(num_ant_ele, num_ant_ele, sizeof(gr_complex))),
        d_num_ant_ele(num_ant_ele)
    {
        
        start = 0;
        filename = config_filename;
        num_elements = d_num_ant_ele;

        // Read configuration file
        // We assume each line has 2 values gain and phase for a given input
        std::ifstream infile(config_filename);
        // Check exists
        if (!infile.good())
            throw std::invalid_argument("Cannot find configuration file.");

        // Do checks as we import gains
        float GainEst, PhaseEst;
        d_ComplexGain = cx_fcolvec(d_num_ant_ele, fill::zeros);
        i = 0;
        while (infile >> GainEst >> PhaseEst)
        {
            if (i>=d_num_ant_ele)
                throw std::invalid_argument("Configuration file has too many inputs.");
            d_ComplexGain(i) = gr_complex(1.0/GainEst, 0)*exp(gr_complex(0,-PhaseEst));
            i++;
        }
        if (i!=d_num_ant_ele)
            cout << "antenna correction file not ready " << endl;	
	
        message_port_register_in(pmt::mp("recalc"));
        set_msg_handler(pmt::mp("recalc"), boost::bind(&antenna_correction_impl::recalc_msg, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    antenna_correction_impl::~antenna_correction_impl()
    {
    }

    void antenna_correction_impl::recalc_msg(pmt::pmt_t msg) {
        if (pmt::is_pair(msg)) {
            pmt::pmt_t key = pmt::car(msg);
            pmt::pmt_t val = pmt::cdr(msg);
            if (pmt::eq(key, pmt::intern("recalc"))) {
                if (pmt::is_integer(val)) {
                    if (pmt::to_long(val) == 10) {
                        start = 0;
                        cout << "Stopping antenna correction!" << endl;
                    }
                    if (pmt::to_long(val) == 20) {
                        // Read configuration file
		        // We assume each line has 2 values gain and phase for a given input
		        std::ifstream infile(filename);
		        // Check exists
		        if (!infile.good())
		            throw std::invalid_argument("Cannot find configuration file.");
		
		        // Do checks as we import gains
		        float GainEst, PhaseEst;
		        d_ComplexGain = cx_fcolvec(num_elements, fill::zeros);
		        i = 0;
		        while (infile >> GainEst >> PhaseEst)
		        {
		            if (i>=num_elements)
		                throw std::invalid_argument("Configuration file has too many inputs.");
		            d_ComplexGain(i) = gr_complex(1.0/GainEst, 0)*exp(gr_complex(0,-PhaseEst));
		            i++;
		        }
		        if (i!=num_elements)
            		    throw std::invalid_argument("Configuration file does not have enough inputs.");	
                        start = 1;
                        cout << "Starting antenna correction!" << endl;
                    }
                }
            } else {
                GR_LOG_WARN(d_logger, boost::format("recalc message must have the key = 'recalc'; got '%1%'.") % pmt::write_string(key));
            }
        } else {
            GR_LOG_WARN(d_logger, "recalc message must be a key:value pair where the key is 'recalc'.");
        }
    }

    int
    antenna_correction_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
	  for(int k=0; k<d_num_ant_ele; k++)
	  {
		  const gr_complex *in_k = (const gr_complex*) input_items[k];
		  gr_complex *out_k = (gr_complex*) output_items[k];
		  for (int i=0; i < noutput_items; i++)
                    if (start == 1) 
		      out_k[i] = d_ComplexGain(k)*in_k[i];
                    if (start == 0)
                      out_k[i] = in_k[i];    //don't do anything		  
	  }
	  
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace doa */
} /* namespace gr */
