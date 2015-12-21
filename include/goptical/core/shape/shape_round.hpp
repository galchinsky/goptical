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


#ifndef GOPTICAL_SHAPE_ROUND_HH_
#define GOPTICAL_SHAPE_ROUND_HH_

#include "goptical/core/common.hpp"

namespace _goptical {

  namespace shape {

    /**
       @short Base class for Disk, Ring, Ellipse and EllipticalRing
       @header <goptical/core/shapeRound
       @module {Core}
       @internal

       This class contains common code for round shaped classes.
     */

    template <class X, bool hole> class Round : public X
    {
    public:
      /** @override */
      unsigned int get_contour_count() const;
      /** @override */
      void get_pattern(const math::Vector2::put_delegate_t  &v, const trace::Distribution &d, bool unobstructed) const;
      /** @override */
      void get_contour(unsigned int contour, const math::Vector2::put_delegate_t  &f, double resolution) const;
      /** @override */
      void get_triangles(const math::Triangle<2>::put_delegate_t  &f, double resolution) const;

    private:
      double get_radial_step(double resolution) const;
    };
  }

}

#endif

