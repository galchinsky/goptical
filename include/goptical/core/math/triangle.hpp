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


#ifndef GOPTICAL_MATH_TRIANGLE_HH_
#define GOPTICAL_MATH_TRIANGLE_HH_

#include <ostream>

#include "goptical/core/common.hpp"

#include "goptical/core/math/vector.hpp"

namespace _goptical {

  namespace math {

    /**
       @short Triangle base class
       @header <goptical/core/math/Triangle
       @module {Core}
       @internal

       This class is the base class for triangles in N dimensions.
     */
    template <int N> struct TriangleBase
    {
      inline TriangleBase();

      /** Create a triangle from 3 points */
      inline TriangleBase(const Vector<N> &a, const Vector<N> &b, const Vector<N> &c);
      /** Get point n of the triangle */
      inline const Vector<N> & operator[](int n) const;
      /** Get reference to point n of the triangle */
      inline Vector<N> & operator[](int n);
      /** Get triangle centroid */
      inline Vector<N> get_centroid() const;

    protected:
      Vector<N> _v[3];
    };

    /** 
       @short N dimension triangle class
       @header <goptical/core/math/Triangle
       @module {Core}

       This class is the general purpose N dimensions triangles class.
     */

    template <int N> struct Triangle : public TriangleBase<N>
    {
      typedef delegate<void (const math::Triangle<N> &)> put_delegate_t;

      inline Triangle<N>();

      /** Create a triangle from 3 points */
      inline Triangle<N>(const Vector<N> &a, const Vector<N> &b, const Vector<N> &c);
    };

    /**
       @short 3d triangle class
       @header <goptical/core/math/Triangle
       @module {Core}

       This class is the 3d triangles class.
     */
    template <> struct Triangle<3> : public TriangleBase<3>
    {
      typedef delegate<void (const math::Triangle<3> &)> put_delegate_t;

      inline Triangle<3>();

      /** Create a triangle from 3 points */
      inline Triangle<3>(const Vector3 &a, const Vector3 &b, const Vector3 &c);

      /** Get triangle normal */
      inline Vector3 normal() const;
    };

    template <int N>
    inline std::ostream & operator<<(std::ostream &o, const Triangle<N> &l);

  }
}

#endif

