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

#include <goptical/core/curve/Grid>
#include <goptical/core/data/Grid>

namespace _goptical {

  namespace curve {

    Grid::Grid(unsigned int n, double r)
      : _data(n, n, math::Vector2(-r, -r),
              math::Vector2(2.0 * r / (double)(n - 1), 2.0 * r / (double)(n - 1)))
    {
      _data.set_interpolation(data::Bicubic);
    }

    Grid::~Grid()
    {
    }

    void Grid::fit(const Base &c)
    {
      for (unsigned int x = 0; x < _data.get_count(0); x++)
        for (unsigned int y = 0; y < _data.get_count(1); y++)
          {
            math::Vector2 v = _data.get_x_value_i(x, y);

            _data.get_y_value(x, y) = c.sagitta(v);

            if (_data.get_interpolation() == data::BicubicDeriv)
              c.derivative(v, _data.get_d_value(x, y));
          }
    }

    double Grid::sagitta(const math::Vector2 & xy) const
    {
      return _data.interpolate(xy);
    }

    void Grid::derivative(const math::Vector2 & xy, math::Vector2 & dxdy) const
    {
      dxdy = _data.interpolate_deriv(xy);
    }

  }
}
