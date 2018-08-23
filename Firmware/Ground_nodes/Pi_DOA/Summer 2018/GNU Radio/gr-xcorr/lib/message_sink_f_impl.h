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

#ifndef INCLUDED_XCORR_MESSAGE_SINK_F_IMPL_H
#define INCLUDED_XCORR_MESSAGE_SINK_F_IMPL_H

#include <xcorr/message_sink_f.h>

namespace gr {
  namespace xcorr {

    class message_sink_f_impl : public message_sink_f
    {
     private:
      // Nothing to declare in this block.

     public:
      message_sink_f_impl();
      ~message_sink_f_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace xcorr
} // namespace gr

#endif /* INCLUDED_XCORR_MESSAGE_SINK_F_IMPL_H */

