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


#ifndef INCLUDED_XCORR_HOLD_II_H
#define INCLUDED_XCORR_HOLD_II_H

#include <xcorr/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace xcorr {

    /*!
     * \brief <+description of block+>
     * \ingroup xcorr
     *
     */
    class XCORR_API hold_ii : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<hold_ii> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of xcorr::hold_ii.
       *
       * To avoid accidental use of raw pointers, xcorr::hold_ii's
       * constructor is in a private implementation
       * class. xcorr::hold_ii::make is the public interface for
       * creating new instances.
       */
      static sptr make(bool hold);
      virtual void set_hold(bool new_hold) = 0;
      virtual bool get_hold() const = 0;
    };

  } // namespace xcorr
} // namespace gr

#endif /* INCLUDED_XCORR_HOLD_II_H */

