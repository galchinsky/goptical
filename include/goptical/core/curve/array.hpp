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


#ifndef GOPTICAL_CURVE_ARRAY_HH_
#define GOPTICAL_CURVE_ARRAY_HH_

#include "goptical/core/common.hpp"

#include "base.hpp"

namespace _goptical {

  namespace curve {

    /**
       @short Enable definition of curve as square and hexagonal array of an other curve
       @header <goptical/core/curve/Array
       @module {Core}
       @main

       This class provides a way to build an array of an other
       curve. It can be used to design lenses array.
     */
    class Array : public Base
    {
    public:

      /** Specify tessellation pattern used by @ref Array class */
      enum pattern_e
        {
          Square,
          SquareCenter,
          Hexagonal,
        };

      Array(const const_ref<Base> &curve, double pitch, enum pattern_e p = Square);

      /** @override */
      double sagitta(const math::Vector2 & xy) const;
      /** @override */
      void derivative(const math::Vector2 & xy, math::Vector2 & dxdy) const;

    private:

      typedef math::Vector2 (Array::*transform_t)(const math::Vector2 &v) const;

      math::Vector2 transform_square(const math::Vector2 &v) const;
      math::Vector2 transform_square_center(const math::Vector2 &v) const;

      math::Vector2 transform_hexagonal(const math::Vector2 &v) const;
      math::Vector2 transform_hexagonal_center(const math::Vector2 &v) const;

      const_ref<Base> _curve;
      double _pitch;
      transform_t _transform;
    };

  }
}

#endif

