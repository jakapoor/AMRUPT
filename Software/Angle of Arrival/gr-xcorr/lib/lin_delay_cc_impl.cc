/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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
#include "lin_delay_cc_impl.h"

namespace gr {
  namespace xcorr {

    lin_delay_cc::sptr
    lin_delay_cc::make(int samp_rate, float delay)
    {
      return gnuradio::get_initial_sptr
        (new lin_delay_cc_impl(samp_rate, delay));
    }

    /*
     * The private constructor
     */
    lin_delay_cc_impl::lin_delay_cc_impl(int samp_rate, float delay)
      : gr::sync_block("lin_delay_cc",
              gr::io_signature::make(2, 2, sizeof(gr_complex)),
              gr::io_signature::make(2, 2, sizeof(gr_complex))),
        d_samp_rate(samp_rate),
        buffer(),
        points()
    {
        set_dly(delay);
        curr_offset = 0;
    }

    /*
     * Our virtual destructor.
     */
    lin_delay_cc_impl::~lin_delay_cc_impl()
    {
    }

    gr_complex lin_delay_cc_impl::interpolate(const gr_complex &p1, const gr_complex &p2, float ratio) {
        //return p1 + (p2 - p1)*ratio;

        // unwrap the angle
        float ang1 = arg(p1);
        float ang2 = arg(p2);
        if (abs(ang2-ang1) > 3.141593) {
            if (ang2 > ang1) {
                ang1 += 6.283185;
            } else {
                ang2 += 6.283185;
            }
        }
        float mag = abs(p1) + (abs(p2) - abs(p1))*ratio;
        float ang = ang1 + (ang2 - ang1)*ratio;
        return std::polar(mag, ang);
    }

    void lin_delay_cc_impl::set_dly(float new_delay) {
        if (new_delay != d_delay) {
            gr::thread::scoped_lock l(d_mutex_delay);
            d_delay = new_delay;
            offset = fabs(d_delay) / (double) d_samp_rate;
            //printf("\n<><><><><><><><><><><><>\n");
            //printf("new delay: %f\n", new_delay);
            //printf("new offset: %f\n", offset);
        }
    }

    int
    lin_delay_cc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr::thread::scoped_lock l(d_mutex_delay);

      const gr_complex *ref_in;
      const gr_complex *in;
      gr_complex *ref_out;
      gr_complex *out;

      if (d_delay >= 0) {
          ref_in = (const gr_complex *) input_items[1];
          in = (const gr_complex *) input_items[0];
          ref_out = (gr_complex *) output_items[1];
          out = (gr_complex *) output_items[0];
      } else {
          ref_in = (const gr_complex *) input_items[0];
          in = (const gr_complex *) input_items[1];
          ref_out = (gr_complex *) output_items[0];
          out = (gr_complex *) output_items[1];
      }

      if (points.empty()) {
          points.push(gr_complex(0,0));
          points.push(gr_complex(0,0));
      }

      for (int i=0; i<noutput_items; ++i) {
          buffer.push(in[i]);
          ref_out[i] = ref_in[i];
          if (curr_offset > 1) {
              curr_offset--;
              //printf("<><><><><><><><><><>\nlinear delay wrapped around\n");
              //printf("offset: %f\n", offset);
              //printf("noutput_items: %d\n", noutput_items);
              //printf("buffer size: %lu\n", buffer.size());
          } else {
              points.pop();
              points.push(buffer.front());
              buffer.pop();
          }
          out[i] = interpolate(points.front(), points.back(), 1-curr_offset);
          curr_offset += offset;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace xcorr */
} /* namespace gr */

