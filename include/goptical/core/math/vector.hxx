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


#ifndef GOPTICAL_MATH_VECTORBASE_HXX_
#define GOPTICAL_MATH_VECTORBASE_HXX_

#include <iostream>
#include <cassert>

#include "goptical/core/math/matrix.hpp"

namespace _goptical {

  namespace math {

    template <int N, typename T>
    void VectorBase<N, T>::set(T value)
    {
      for (unsigned int i = 0; i < N; i++)
        _val[i] = value;
    }

    template <int N, typename T>
    T & VectorBase<N, T>::operator[](int n)
    {
      return _val[n];
    }

    template <int N, typename T>
    T VectorBase<N, T>::operator[](int n) const
    {
      return _val[n];
    }

    template <int N, typename T>
    VectorBase<N, T> VectorBase<N, T>::operator+(const VectorBase<N, T> &v) const
    {
      math::VectorBase<N, T> r;

      for (unsigned int i = 0; i < N; i++)
        r._val[i] = _val[i] + v._val[i];

      return r;
    }

    template <int N, typename T>
    VectorBase<N, T> VectorBase<N, T>::operator-(const VectorBase<N, T> &v) const
    {
      math::VectorBase<N, T> r;

      for (unsigned int i = 0; i < N; i++)
        r._val[i] = _val[i] - v._val[i];

      return r;
    }

    template <int N, typename T>
    VectorBase<N, T> VectorBase<N, T>::operator-() const
    {
      math::VectorBase<N, T> r;

      for (unsigned int i = 0; i < N; i++)
        r._val[i] = -_val[i];

      return r;
    }

    template <int N, typename T>
    VectorBase<N, T> & VectorBase<N, T>::neg()
    {
      for (unsigned int i = 0; i < N; i++)
        _val[i] = -_val[i];

      return *this;
    }

    template <int N, typename T>
    const VectorBase<N, T> & VectorBase<N, T>::operator+=(const VectorBase<N, T> &v)
    {
      for (unsigned int i = 0; i < N; i++)
        _val[i] += v._val[i];

      return *this;
    }

    template <int N, typename T>
    const VectorBase<N, T> & VectorBase<N, T>::operator-=(const VectorBase<N, T> &v)
    {
      for (unsigned int i = 0; i < N; i++)
        _val[i] -= v._val[i];

      return *this;
    }

    template <int N, typename T>
    T VectorBase<N, T>::operator*(const VectorBase<N, T> &v) const
    {
      T r = 0;

      for (unsigned int i = 0; i < N; i++)
        r += _val[i] * v._val[i];

      return r;
    }

    template <int N, typename T>
    VectorBase<N, T> VectorBase<N, T>::mul(const VectorBase &v) const
    {
      math::VectorBase<N, T> r;

      for (unsigned int i = 0; i < N; i++)
        r._val[i] = _val[i] * v._val[i];

      return r;
    }

    template <int N, typename T>
    VectorBase<N, T> VectorBase<N, T>::operator/(const VectorBase &v) const
    {
      math::VectorBase<N, T> r;

      for (unsigned int i = 0; i < N; i++)
        r._val[i] = _val[i] / v._val[i];

      return r;
    }

    template <int N, typename T>
    VectorBase<N, T> VectorBase<N, T>::operator*(T scale) const
    {
      math::VectorBase<N, T> r;

      for (unsigned int i = 0; i < N; i++)
        r._val[i] = scale * _val[i];

      return r;
    }

    template <int N, typename T>
    VectorBase<N, T> VectorBase<N, T>::operator/(T scale) const
    {
      math::VectorBase<N, T> r;

      for (unsigned int i = 0; i < N; i++)
        r._val[i] = _val[i] / scale;

      return r;
    }

    template <int N, typename T>
    const VectorBase<N, T> & VectorBase<N, T>::operator*=(T scale)
    {
      for (unsigned int i = 0; i < N; i++)
        _val[i] *= scale;

      return *this;
    }

    template <int N, typename T>
    const VectorBase<N, T> & VectorBase<N, T>::operator/=(T scale)
    {
      for (unsigned int i = 0; i < N; i++)
        _val[i] /= scale;

      return *this;
    }

    template <int N, typename T>
    T VectorBase<N, T>::len() const
    {
      T r = 0;

      for (unsigned int i = 0; i < N; i++)
        r += square(_val[i]);

      return sqrt(r);
    }

    template <int N, typename T>
    const VectorBase<N, T> & VectorBase<N, T>::normalize()
    {
      return *this /= len();
    }

    template <int N, typename T>
    VectorBase<N, T> VectorBase<N, T>::normalized() const
    {
      VectorBase<N, T> r = *this / len();
      return r;
    }

    template <int N, typename T>
    VectorBase<N, T> VectorBase<N, T>::magnitude(T newlen) const
    {
      return *this * (newlen / len());
    }

    template <int N, typename T>
    template <int M>
    VectorBase<M, T> VectorBase<N, T>::select(unsigned int mask, const VectorBase<M, T> &v) const
    {
      math::VectorBase<M, T> r;

      for (unsigned int i = 0; i < M; i++)
        r._val[i] = (mask & (1 << i)) ? _val[i] : v._val[i];

      return r;
    }

    template <int N, typename T>
    VectorBase<N, T> VectorBase<N, T>::operator*(const Matrix<N> &m)
    {
      VectorBase<N, T> r;

      for (int j = 0; j < N; j++)
        {
          T s = 0;

          for (int k = 0; k < N; k++)
            s += _val[k][j] * m._val[k];

          r[j] = s;
        }

      return r;
    }

    template <int N, typename T>
    inline bool VectorBase<N, T>::operator==(const VectorBase &m) const
    {
      for (int j = 0; j < N; j++)
        if (_val[j] != m._val[j])
          return false;

      return true;
    }

    template <int N, typename T>
    bool VectorBase<N, T>::close_to(const VectorBase &m, T error)
    {
      for (int j = 0; j < N; j++)
        if (fabs(_val[j] - m._val[j]) > error)
          return false;

      return true;      
    }

    template <int N, typename T>
    std::ostream & operator<<(std::ostream &o, const VectorBase<N, T> &v)
    {
      o << "[";

      for (unsigned int i = 0; i < N; i++)
        {
          o << v[i];
          if (i + 1 < N)
            o << ", ";
        }

      o << "]";

      return o;
    }

    // **********************************************************************

    template <int N, typename T>
    Vector<N, T>::Vector()
    {
    }

    template <int N, typename T>
    Vector<N, T>::Vector(T value)
    {
      for (unsigned int i = 0; i < N; i++)
        VectorBase<N, T>::_val[i] = value;
    }

    template <int N, typename T>
    Vector<N, T>::Vector(const VectorBase<N, T> &v)
      : VectorBase<N, T>(v)
    {
    }

    // **********************************************************************

    template <typename T>
    Vector<3, T>::Vector()
    {
    }

    template <typename T>
    Vector<3, T>::Vector(const VectorBase<3, T> &v)
      : VectorBase<3, T>(v)
    {
    }

    template <typename T>
    template <int M>
    Vector<3, T>::Vector(const VectorBase<M, T> &v, T p)
    {
      int i;
      for (i = 0; i < std::min(3, M); i++)
        base::_val[i] = v[i];
      for (; i < 3; i++)
        base::_val[i] = p;
    }

    template <typename T>
    Vector<3, T>::Vector(T value)
    {
      base::_val[0] = base::_val[1] = base::_val[2] = value;
    }

    template <typename T>
    Vector<3, T>::Vector(T x, T y, T z)
    {
      base::_val[0] = x;
      base::_val[1] = y;
      base::_val[2] = z;
    }

    template <typename T>
    Vector<2, T> Vector<3, T>::project_xy() const
    {
      Vector<2, T> v;

      v[0] = x();
      v[1] = y();

      return v;
    }

    template <typename T>
    Vector<2, T> Vector<3, T>::project_zy() const
    {
      Vector<2, T> v;

      v[0] = z();
      v[1] = y();

      return v;
    }

    template <typename T>
    Vector<3, T> Vector<3, T>::cross_product(const Vector<3, T> &v) const
    {
      return Vector<3, T>(y() * v.z() - z() * v.y(),
                       z() * v.x() - x() * v.z(),
                       x() * v.y() - y() * v.x());
    }

    template <typename T>
    T & Vector<3, T>::x()
    {
      return base::_val[0];
    }

    template <typename T>
    T & Vector<3, T>::y()
    {
      return base::_val[1];
    }

    template <typename T>
    T & Vector<3, T>::z()
    {
      return base::_val[2];
    }

    template <typename T>
    T Vector<3, T>::x() const
    {
      return base::_val[0];
    }

    template <typename T>
    T Vector<3, T>::y() const
    {
      return base::_val[1];
    }

    template <typename T>
    T Vector<3, T>::z() const
    {
      return base::_val[2];
    }

    // **********************************************************************

    template <typename T>
    Vector<2, T>::Vector()
    {
    }

    template <typename T>
    Vector<2, T>::Vector(const VectorBase<2, T> &v)
      : VectorBase<2, T>(v)
    {
    }

    template <typename T>
    template <int M>
    Vector<2, T>::Vector(const VectorBase<M, T> &v, T p)
    {
      int i;
      for (i = 0; i < std::min(2, M); i++)
        base::_val[i] = v[i];
      for (; i < 2; i++)
        base::_val[i] = p;
    }

    template <typename T>
    Vector<2, T>::Vector(T value)
    {
      base::_val[0] = base::_val[1] = value;
    }

    template <typename T>
    Vector<2, T>::Vector(T x, T y)
    {
      base::_val[0] = x;
      base::_val[1] = y;
    }

    template <typename T>
    template <int N>
    Vector<2, T>::Vector(const VectorBase<N, T> &v, unsigned int a, unsigned int b)
    {
      base::_val[0] = v[a];
      base::_val[1] = v[b];
    }

    template <typename T>
    T Vector<2, T>::cross_product(const Vector<2, T> &v) const
    {
      return base::_val[0] * v.base::_val[1] - base::_val[1] * v.base::_val[0];
    }

    template <typename T>
    T & Vector<2, T>::x()
    {
      return base::_val[0];
    }

    template <typename T>
    T & Vector<2, T>::y()
    {
      return base::_val[1];
    }

    template <typename T>
    T Vector<2, T>::x() const
    {
      return base::_val[0];
    }

    template <typename T>
    T Vector<2, T>::y() const
    {
      return base::_val[1];
    }

  }
}

#endif

