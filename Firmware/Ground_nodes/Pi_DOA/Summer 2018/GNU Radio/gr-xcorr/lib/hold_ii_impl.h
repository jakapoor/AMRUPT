/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_XCORR_HOLD_II_IMPL_H
#define INCLUDED_XCORR_HOLD_II_IMPL_H

#include <xcorr/hold_ii.h>

namespace gr {
  namespace xcorr {

    class hold_ii_impl : public hold_ii
    {
     private:
      bool d_hold;
      int d_value;
      gr::thread::mutex d_mutex_delay;

     public:
      hold_ii_impl(bool hold);
      ~hold_ii_impl();

      void set_hold(bool new_hold);
      bool get_hold() const {return d_hold;}

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace xcorr
} // namespace gr

#endif /* INCLUDED_XCORR_HOLD_II_IMPL_H */

