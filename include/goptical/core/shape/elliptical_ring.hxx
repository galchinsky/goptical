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


#ifndef GOPTICAL_SHAPE_ELLIPTICALRING_HXX_
#define GOPTICAL_SHAPE_ELLIPTICALRING_HXX_

#include "base.hxx"

#include <cassert>

namespace _goptical {

  namespace shape {

    double EllipticalRingBase::get_x_radius(void) const
    {
      return _xr;
    }

    double EllipticalRingBase::get_y_radius(void) const
    {
      return _yr;
    }

    double EllipticalRingBase::get_x_hole_radius(void) const
    {
      return _xhr;
    }

    double EllipticalRingBase::get_y_hole_radius(void) const
    {
      return _xhr / _xy_ratio;
    }

    double EllipticalRingBase::max_radius() const
    {
      return std::max(_yr, _xr);
    }

    double EllipticalRingBase::min_radius() const
    {
      return std::min(_yr, _xr);
    }

    double EllipticalRingBase::get_external_xradius() const
    {
      return _xr;
    }

    double EllipticalRingBase::get_internal_xradius() const
    {
      return _xhr;
    }

    double EllipticalRingBase::get_xy_ratio() const
    {
      return _xy_ratio;
    }

  }
}

#endif

