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

#include <goptical/core/shape/Disk>
#include <goptical/core/math/VectorPair>

#include "shape_round_.hxx"

namespace _goptical {

  namespace shape {

    bool DiskBase::inside(const math::Vector2 &point) const
    {
      return (math::square(point.x()) + math::square(point.y()) <= math::square(_radius));
    }

    math::VectorPair2 DiskBase::get_bounding_box() const
    {
      math::Vector2 hs(_radius, _radius);

      return math::VectorPair2(-hs, hs);
    }

    double DiskBase::get_outter_radius(const math::Vector2 &dir) const
    {
      return _radius;
    }

    template class Round<DiskBase, false>;

  }

}

