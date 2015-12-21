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


#include <goptical/core/shape/EllipticalRing>
#include <goptical/core/math/VectorPair>

#include "shape_round_.hxx"

namespace _goptical {

  namespace shape {

    EllipticalRing::EllipticalRing(double x_radius, double y_radius, double x_hole_radius)
    {
      set_radius(x_radius, y_radius, x_hole_radius);
    }

    void EllipticalRingBase::set_radius(double x_radius, double y_radius, double x_hole_radius)
    {
      assert(x_radius > x_hole_radius);

      _xr = x_radius;
      _xhr = x_hole_radius;
      _yr = y_radius;
      _xy_ratio = x_radius / y_radius;
      _e2 = math::square(sqrt(fabs(_xr * _xr - _yr * _yr)) / std::max(_xr, _yr));
    }

    bool EllipticalRingBase::inside(const math::Vector2 &point) const
    {
      double d = sqrt(math::square(point.x()) + math::square(point.y() * _xy_ratio));

      return (d <= _xr && d >= _xhr);
    }

    math::VectorPair2 EllipticalRingBase::get_bounding_box() const
    {
      math::Vector2 hs(_xr, _yr);

      return math::VectorPair2(-hs, hs);
    }

    double EllipticalRingBase::get_outter_radius(const math::Vector2 &dir) const
    {
      return _xr > _yr
        ? sqrt(math::square(_yr) / (1. - _e2 * math::square(dir.x())))
        : sqrt(math::square(_xr) / (1. - _e2 * math::square(dir.y())));
    }

    double EllipticalRingBase::get_hole_radius(const math::Vector2 &dir) const
    {
      return _xr > _yr
        ? sqrt(math::square(_xhr / _xy_ratio) / (1. - _e2 * math::square(dir.x())))
        : sqrt(math::square(_xhr) / (1. - _e2 * math::square(dir.y())));
    }

    template class Round<EllipticalRingBase, true>;

  }

}
