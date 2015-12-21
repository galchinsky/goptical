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


#include <goptical/core/sys/Surface>
#include <goptical/core/shape/Infinite>
#include <goptical/core/math/Vector>
#include <goptical/core/Error>

namespace _goptical {

  namespace shape {

    bool Infinite::inside(const math::Vector2 &point) const
    {
      return true;
    }

    math::VectorPair2 Infinite::get_bounding_box() const
    {
      return math::vector2_pair_00;
    }

    void Infinite::get_pattern(const math::Vector2::put_delegate_t  &f, const trace::Distribution &d, bool unobstructed) const
    {
      throw Error("can not distribute rays accross an infinite surface shape");
    }

    unsigned int Infinite::get_contour_count() const
    {
      return 0;
    }

    void Infinite::get_contour(unsigned int contour, const math::Vector2::put_delegate_t  &f, double resolution) const
    {
    }

    void Infinite::get_triangles(const math::Triangle<2>::put_delegate_t  &t, double resolution) const
    {
    }

    double Infinite::get_outter_radius(const math::Vector2 &dir) const
    {
      return 0;
    }

    Infinite infinite;

  }
}

