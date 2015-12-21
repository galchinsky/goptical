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


#ifndef GOPTICAL_DATA_SAMPLESET_1D_HXX_
#define GOPTICAL_DATA_SAMPLESET_1D_HXX_

#include <cassert>

#include "goptical/core/data/set1d.hxx"
#include "data_interpolate_1d.hxx"

namespace _goptical {

  namespace data {

    void SampleSetBase::set_metrics(double origin, double step)
    {
      _origin = origin;
      _step = step;
      invalidate();
    }

    unsigned int SampleSetBase::get_count() const
    {
      return _data.size();
    }

    double SampleSetBase::get_x_value(unsigned int n) const
    {
      return _origin + (double)n * _step;
    }

    double SampleSetBase::get_y_value(unsigned int n) const
    {
      return _data[n].y;
    }

    double & SampleSetBase::get_y_value(unsigned int n)
    {
      invalidate();
      return _data[n].y;
    }

    double SampleSetBase::get_d_value(unsigned int n) const
    {
      return _data[n].d;
    }

    double & SampleSetBase::get_d_value(unsigned int n)
    {
      invalidate();
      return _data[n].d;
    }

    double SampleSetBase::get_step() const
    {
      return _step;
    }

    double SampleSetBase::get_origin() const
    {
      return _origin;
    }

    double SampleSetBase::get_x_interval(unsigned int x) const
    {
      return _step;
    }

    double SampleSetBase::get_x_interval(unsigned int x1, unsigned int x2) const
    {
      return _step * (double)(x2 - x1);
    }

  }
}

#endif

