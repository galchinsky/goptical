/*

      This file is part of the <goptical/core Core library.
  
      The <goptical/core library is free software; you can redistribute it
      and/or modify it under the terms of the GNU General Public
      License as published by the Free Software Foundation; either
      version 3 of the License, or (at your option) any later version.
  
      The <goptical/core library is distributed in the hope that it will be
      useful, but WITHOUT ANY WARRANTY; without even the implied
      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
      See the GNU General Public License for more details.
  
      You should have received a copy of the GNU General Public
      License along with the <goptical/core library; if not, write to the
      Free Software Foundation, Inc., 59 Temple Place, Suite 330,
      Boston, MA 02111-1307 USA
  
      Copyright (C) 2010-2011 Free Software Foundation, Inc
      Author: Alexandre Becoulet

*/


#ifndef GOPTICAL_DATA_SET1D_HXX_
#define GOPTICAL_DATA_SET1D_HXX_

#include "goptical/core/data/set.hxx"

namespace _goptical {

  namespace data {

    Set1d::Set1d()
      : Set()
    {
    }

    Set1d::~Set1d()
    {
    }

    unsigned int Set1d::get_dimensions() const
    {
      return 1;
    }

    unsigned int Set1d::get_count(unsigned int dimension) const
    {
      assert(dimension == 0);
      return get_count();
    }

    double Set1d::get_x_value(unsigned int x, unsigned int dimension) const
    {
      assert(dimension == 0);
      return get_x_value(x);
    }

    double Set1d::get_y_value(const unsigned int x[]) const
    {
      return get_y_value(x[0]);
    }

    math::range_t Set1d::get_x_range(unsigned int dimension) const
    {
      assert(dimension == 0);     
      return get_x_range();
    }

    double Set1d::interpolate(const double x[]) const
    {
      return interpolate(x[0]);
    }

    double Set1d::interpolate(const double x[], unsigned int deriv, unsigned int dimension) const
    {
      assert(dimension == 0);     
      return interpolate(x[0], deriv);
    }


  }
}

#endif

