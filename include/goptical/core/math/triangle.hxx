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


#ifndef GOPTICAL_MATH_TRIANGLE_HXX_
#define GOPTICAL_MATH_TRIANGLE_HXX_

#include <ostream>

#include "goptical/core/math/vector.hxx"

namespace _goptical {

  namespace math {

    // **********************************************************************

    template <int N>
    TriangleBase<N>::TriangleBase()
    {
    }

    template <int N>
    TriangleBase<N>::TriangleBase(const Vector<N> &a, const Vector<N> &b, const Vector<N> &c)
    {
      _v[0] = a;
      _v[1] = b;
      _v[2] = c;
    }

    template <int N>
    const Vector<N> & TriangleBase<N>::operator[](int n) const
    {
      assert(n < 3);
      return _v[n];
    }

    template <int N>
    Vector<N> & TriangleBase<N>::operator[](int n)
    {
      assert(n < 3);
      return _v[n];
    }

    template <int N>
    std::ostream & operator<<(std::ostream &o, const TriangleBase<N> &l)
    {
      o << "[" << l.vect(0) << ", " << l.vect(1) << ", " << l.vect(2) << "]";

      return o;
    }

    // **********************************************************************

    template <int N>
    Triangle<N>::Triangle()
    {
    }

    template <int N>
    Triangle<N>::Triangle(const Vector<N> &a, const Vector<N> &b, const Vector<N> &c)
      : TriangleBase<N>(a, b, c)
    {
    }

    template <int N>
    Vector<N> TriangleBase<N>::get_centroid() const
    {
      return (_v[0] + _v[1] + _v[2]) / 3.;
    }

    // **********************************************************************

    Triangle<3>::Triangle()
    {
    }

    Triangle<3>::Triangle(const Vector3 &a, const Vector3 &b, const Vector3 &c)
      : TriangleBase<3>(a, b, c)
    {
    }

    Vector3 Triangle<3>::normal() const
    {
      return Vector3(_v[0] - _v[1]).cross_product(_v[0] - _v[2]).normalized();
    }

  }
}

#endif

