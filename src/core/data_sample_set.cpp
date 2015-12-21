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


#include <goptical/core/Error>
#include <goptical/core/data/SampleSet>

#include "data_interpolate_1d_.hxx"

namespace _goptical
{

  namespace data
  {

    SampleSetBase::SampleSetBase()
      : Set1d(),
        _origin(0),
        _step(1.0),
        _data()
    {
    }

    SampleSetBase::~SampleSetBase()
    {
    }

    math::range_t SampleSetBase::get_x_range() const
    {
      if (_data.empty())
        throw Error("data set contains no data");

      return math::range_t(_origin, _origin + _step * (_data.size() - 1));
    }

    inline unsigned int SampleSetBase::get_interval(double x) const
    {
      int n = (int)floor((x - _origin) / _step);

      if (n < 0)
        return 0;
      else if (n >= (int)_data.size())
        return _data.size();
      else
        return n + 1;
    }

    inline unsigned int SampleSetBase::get_nearest(double x) const
    {
      int n = (int)round((x - _origin) / _step);

      if (n < 0)
        return 0;
      else if (n >= (int)_data.size())
        return _data.size() - 1;
      else
        return n;
    }

    void SampleSetBase::set_value(unsigned int x, double y, double d)
    {
      struct entry_s ze = { 0.0, 0.0 };

      if (x >= _data.size())
        _data.resize(x + 1, ze);

      struct entry_s e = { y, d };

      _data[x] = e;
      invalidate();
   }

    void SampleSetBase::set_value_near(double x, double y, double d)
    {
      assert(x >= _origin);

      struct entry_s ze = { 0.0, 0.0 };

      int n = (int)round((x - _origin) / _step);

      if (n >= (int)_data.size())
        _data.resize(n + 1, ze);

      struct entry_s e = { y, d };

      _data[n] = e;
      invalidate();
    }

    void SampleSetBase::clear()
    {
      _data.clear();
      invalidate();
    }

    void SampleSetBase::resize(unsigned int n)
    {
      struct entry_s ze = { 0.0, 0.0 };

      _data.resize(n, ze);
      invalidate();
    }

    template class Interpolate1d<SampleSetBase>;

  }

}

