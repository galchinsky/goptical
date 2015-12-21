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


#ifndef GOPTICAL_SHAPE_INFINITE_HH_
#define GOPTICAL_SHAPE_INFINITE_HH_

#include "goptical/core/common.hpp"

#include "base.hpp"

namespace _goptical {

  namespace shape {

    /**
       @short Borderless shape
       @header <goptical/core/shape/Infinite
       @module {Core}
       @main

       This class implements an infinite shape with no edge. Any 2d
       points is located inside the shape. It is useful to describe
       unbounded image plane for instance.

       The global variable @ref infinite provides an instance of this
       class.
     */

    class Infinite : public Base
    {
    public:
      /** Create an infinite shape */
      inline Infinite();

      /** @override */
      inline double max_radius() const;
      /** @override */
      inline double min_radius() const;
      /** @override */
      double get_outter_radius(const math::Vector2 &dir) const;
      /** @override */
      math::VectorPair2 get_bounding_box() const;
      /** @override */
      bool inside(const math::Vector2 &point) const;
      /** @override */
      void get_pattern(const math::Vector2::put_delegate_t  &v, const trace::Distribution &d, bool unobstructed) const;
      /** @override */
      unsigned int get_contour_count() const;
      /** @override */
      void get_contour(unsigned int contour, const math::Vector2::put_delegate_t  &f, double resolution) const;
      /** @override */
      void get_triangles(const math::Triangle<2>::put_delegate_t  &f, double resolution) const;
    };

    extern Infinite infinite;

  }
}

#endif

