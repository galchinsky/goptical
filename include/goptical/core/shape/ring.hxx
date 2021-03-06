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


#ifndef GOPTICAL_SHAPE_RING_HXX_
#define GOPTICAL_SHAPE_RING_HXX_

#include <cassert>

#include "base.hxx"

namespace _goptical {

  namespace shape {

    Ring::Ring(double radius, double hole_radius)
    {
      set_radius(radius, hole_radius);
    }

    void RingBase::set_radius(double radius, double hole_radius)
    {
      assert(radius > hole_radius);

      _radius = radius;
      _hole_radius = hole_radius;
    }

    double RingBase::get_radius(void) const
    {
      return _radius;
    }

    double RingBase::get_hole_radius(void) const
    {
      return _hole_radius;
    }

    double RingBase::max_radius() const
    {
      return _radius;
    }

    double RingBase::min_radius() const
    {
      return _radius;
    }

    double RingBase::get_external_xradius() const
    {
      return _radius;
    }

    double RingBase::get_internal_xradius() const
    {
      return _hole_radius;
    }

    double RingBase::get_xy_ratio() const
    {
      return 1.0;
    }

  }
}

#endif

