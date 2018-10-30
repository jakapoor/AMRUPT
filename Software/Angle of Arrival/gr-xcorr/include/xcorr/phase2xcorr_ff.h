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


#ifndef INCLUDED_XCORR_PHASE2XCORR_FF_H
#define INCLUDED_XCORR_PHASE2XCORR_FF_H

#include <xcorr/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace xcorr {

    /*!
     * \brief <+description of block+>
     * \ingroup xcorr
     *
     */
    class XCORR_API phase2xcorr_ff : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<phase2xcorr_ff> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of xcorr::phase2xcorr_ff.
       *
       * To avoid accidental use of raw pointers, xcorr::phase2xcorr_ff's
       * constructor is in a private implementation
       * class. xcorr::phase2xcorr_ff::make is the public interface for
       * creating new instances.
       */
      static sptr make(float alpha);
    };

  } // namespace xcorr
} // namespace gr

#endif /* INCLUDED_XCORR_PHASE2XCORR_FF_H */

