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

#include <iostream>

#include <goptical/core/Error>
#include <goptical/core/data/DiscreteSet>

#include "data_interpolate_1d_.hxx"

namespace _goptical
{

  namespace data
  {

    DiscreteSetBase::DiscreteSetBase()
      : Set1d(),
        _data()
    {
    }

    DiscreteSetBase::~DiscreteSetBase()
    {
    }

    inline unsigned int DiscreteSetBase::get_interval(double x) const
    {
      int min_idx = 0;
      int max_idx = _data.size() + 1;

      while (max_idx - min_idx > 1)
        {
          unsigned int p = (max_idx + min_idx) / 2;

          if (x >= _data[p - 1].x)
            min_idx = p;
          else
            max_idx = p;
        }

      return min_idx;
    }

    inline unsigned int DiscreteSetBase::get_nearest(double x) const
    {
      int min_idx = 0;
      int max_idx = _data.size();

      while (max_idx - min_idx > 1)
        {
          unsigned int p = (max_idx + min_idx) / 2;

          if (x + x >= _data[p - 1].x + _data[p].x)
            min_idx = p;
          else
            max_idx = p;
        }

      return min_idx;
    }

    void DiscreteSetBase::add_data(double x, double y, double d)
    {
      const struct entry_s e = { x, y, d };

      _version++;

      unsigned int di = get_interval(x);

      if (di && (_data[di - 1].x == x))
        _data[di - 1] = e;
      else
        _data.insert(_data.begin() + di, e);

      invalidate();
    }

    void DiscreteSetBase::clear()
    {
      _data.clear();
      _version++;
      invalidate();
    }

    math::range_t DiscreteSetBase::get_x_range() const
    {
      if (_data.empty())
        throw Error("_data set contains no _data");

      return math::range_t(_data.front().x, _data.back().x);
    }

    template class Interpolate1d<DiscreteSetBase>;

  }

}

