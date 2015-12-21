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


#ifndef GOPTICAL_SHAPE_BASE_HH_
#define GOPTICAL_SHAPE_BASE_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/math/vector.hpp"
#include "goptical/core/math/triangle.hpp"

namespace _goptical {

  namespace shape {

    /**
       @short Base class for contour 2d shapes
       @header <goptical/core/shape/Base
       @module {Core}
       @main

       This class defines an interface for all 2d shape
       implementations. It is mainly used to describe 2d contours of
       optical surfaces and provides distribution pattern for ray
       tracing.
     */
    class Base : public ref_base<Base>
    {
    public:
      inline Base();

      virtual ~Base() {}

      /** Check if the (x,y) 2d point is inside 2d shape area */
      virtual bool inside(const math::Vector2 &point) const = 0;

      /** Get points distributed on shape area with given pattern */
      virtual void get_pattern(const math::Vector2::put_delegate_t &f,
                               const trace::Distribution &d,
                               bool unobstructed = false) const;

      /** Get distance between origin and farthest shape edge */
      virtual double max_radius() const = 0;

      /** Get distance between origin and nearest shape outter edge */
      virtual double min_radius() const = 0;

      /** Get distance between origin and farthest shape edge in specified direction */
      virtual double get_outter_radius(const math::Vector2 &dir) const = 0;

      /** Get distance between origin and nearest shape outter edge in specified direction */
      virtual double get_hole_radius(const math::Vector2 &dir) const;

      /** Get shape bounding box */
      virtual math::VectorPair2 get_bounding_box() const = 0;

      /** Get number of contours polygones. This function returns
          value is greater than 1 if shape has hole(s). @see get_contour */
      virtual unsigned int get_contour_count() const = 0;

      /** Get contour polygone points for specified contour id. First
          contour is always outter edge. @see get_contour_count */
      virtual void get_contour(unsigned int contour,
                               const math::Vector2::put_delegate_t &f,
                               double resolution) const = 0;

      /** Get shape teselation triangles */
      virtual void get_triangles(const math::Triangle<2>::put_delegate_t &f,
                                 double resolution) const = 0;
    };

  }
}

#endif

