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


#ifndef GOPTICAL_DATA_SAMPLEGRID_HXX_
#define GOPTICAL_DATA_SAMPLEGRID_HXX_

#include "goptical/core/data/set.hxx"
#include "goptical/core/math/vector.hxx"

#include <cassert>

namespace _goptical {

  namespace data {

//     double & Grid::get_nearest_y_value(double x1, double x2)
//     {
//       assert(!_data.empty());

//       return data((unsigned int)lp_round((x1 - _origin[0]) / _step[0]),
//                (unsigned int)lp_round((x2 - _origin[1]) / _step[1]));
//     }

    math::Vector2 Grid::get_x_value_i(unsigned int n1, unsigned int n2) const
    {
      return _origin + _step.mul( math::Vector2((double)n1, (double)n2) );
    }

    double Grid::get_y_value(unsigned int n1, unsigned int n2) const
    {
      assert(n1 < _size[0]);
      assert(n2 < _size[1]);

      return _y_data[n1 + n2 * _size[0]];
    }

    double & Grid::get_y_value(unsigned int n1, unsigned int n2)
    {
      assert(n1 < _size[0]);
      assert(n2 < _size[1]);

      invalidate();

      return _y_data[n1 + n2 * _size[0]];
    }

    const math::Vector2 & Grid::get_d_value(unsigned int n1, unsigned int n2) const
    {
      assert(_interpolation == BicubicDeriv);
      assert(n1 < _size[0]);
      assert(n2 < _size[1]);

      return _d_data[n1 + n2 * _size[0]];
    }

    math::Vector2 & Grid::get_d_value(unsigned int n1, unsigned int n2)
    {
      assert(_interpolation == BicubicDeriv);
      assert(n1 < _size[0]);
      assert(n2 < _size[1]);

      invalidate();

      return _d_data[n1 + n2 * _size[0]];
    }

    double Grid::get_nearest_y(const math::Vector2 & v) const
    {
      unsigned int x[2];

      lookup_nearest(x, v);
      return _y_data[x[0] + x[1] * _size[0]];
    }

    const math::Vector2 & Grid::get_nearest_d(const math::Vector2 & v) const
    {
      assert(_interpolation == BicubicDeriv);

      unsigned int x[2];

      lookup_nearest(x, v);
      return _d_data[x[0] + x[1] * _size[0]];
    }

    double & Grid::get_nearest_y(const math::Vector2 & v)
    {
      unsigned int x[2];

      invalidate();
      lookup_nearest(x, v);
      return _y_data[x[0] + x[1] * _size[0]];
    }

    math::Vector2 & Grid::get_nearest_d(const math::Vector2 & v)
    {
      assert(_interpolation == BicubicDeriv);

      unsigned int x[2];

      invalidate();
      lookup_nearest(x, v);
      return _d_data[x[0] + x[1] * _size[0]];
    }

    double Grid::interpolate(const math::Vector2 & v) const
    {      
      unsigned int x[2];
      (this->*_lookup)(x, v);

      return (this->*_interpolate_y)(x, v);
    }

    math::Vector2 Grid::interpolate_deriv(const math::Vector2 & v) const
    {
      math::Vector2 res;
      unsigned int x[2];
      (this->*_lookup)(x, v);

      (this->*_interpolate_d)(x, res, v);

      return res;
    }

    void Grid::set_metrics(const math::Vector2 & origin, const math::Vector2 & step)
    {
      invalidate();
      _origin = origin;
      _step = step;
    }

    const math::Vector2 & Grid::get_origin() const
    {
      return _origin;
    }

    const math::Vector2 & Grid::get_step() const
    {
      return _step;
    }

    void Grid::resize(unsigned int n1, unsigned int n2)
    {
      (this->*_resize)(n1, n2);
      _size[0] = n1;
      _size[1] = n2;
      invalidate();
    }

    unsigned int Grid::get_dimensions() const
    {
      return 2;
    }

    unsigned int Grid::get_count(unsigned int dimension) const
    {
      return _size[dimension];
    }

    void Grid::invalidate()
    {
      _lookup = _update;
    }

  }

}

#endif

