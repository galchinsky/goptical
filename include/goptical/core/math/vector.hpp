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


#ifndef GOPTICAL_MATH_VECTORBASE_HH_
#define GOPTICAL_MATH_VECTORBASE_HH_

#include <ostream>

#include "goptical/core/common.hpp"

namespace _goptical {

  namespace math {

    /**
       @short Vector base class
       @header <goptical/core/math/Vector
       @module {Core}
       @internal

       This class is the base class for N dimensions vector.
     */
    template <int N, typename T> struct VectorBase
    {
      template <int, typename> friend class VectorBase;

      /** Set the whole vector to the specified value */
      inline void set(T value);
      /** Get value at specified index */
      inline T operator[](int n) const;
      /** Get reference to value at specified index */
      inline T & operator[](int n);
      /** Add two vectors */
      inline VectorBase operator+(const VectorBase &v) const;
      /** Subtract two vectors */
      inline VectorBase operator-(const VectorBase &v) const;
      /** Get negated vector */
      inline VectorBase operator-() const;
      /** Negate vector */
      inline VectorBase & neg();
      /** Add a vector */
      inline const VectorBase & operator+=(const VectorBase &v);
      /** Sutract a vector */
      inline const VectorBase & operator-=(const VectorBase &v);
      /** Vector dot product */
      inline T operator*(const VectorBase &v) const;
      /** Vector values multiply */
      inline VectorBase mul(const VectorBase &v) const;
      /** Scale vector */
      inline VectorBase operator*(T scale) const;
      /** Scale vector */
      inline VectorBase operator/(T scale) const;
      /** Vector division */
      inline VectorBase operator/(const VectorBase &v) const;
      /** Multiply by a vector */
      inline const VectorBase & operator*=(T scale);
      /** Divide by a vector */
      inline const VectorBase & operator/=(T scale);
      /** Compute vector length */
      inline T len() const;
      /** Normalize vector length @see normalized */
      inline const VectorBase & normalize();
      /** Get normalized vector @see normalize */
      inline VectorBase normalized() const;
      /** Adjust vector length */
      inline VectorBase magnitude(T newlen) const;
      /** Select components from two vectors. Components which have
          their corresponding bit set in the mask are extracted from
          this vector and other components are taken from passed
          vector. */
      template <int M>
      inline VectorBase<M, T> select(unsigned int bitmask, const VectorBase<M, T> &v) const;
      /** Multiply vector with matrix. See Vector class for
          matrix/vector multiplication. */
      inline VectorBase operator*(const Matrix<N> &m);
      /** compare two vectors for equality */
      inline bool operator==(const VectorBase &m) const;
      /** compare two almost equal vectors */
      inline bool close_to(const VectorBase &m, T error = 1e-8);

    protected:
      T _val[N];
    };

    /** 
       @short N dimension vector class
       @header <goptical/core/math/Vector
       @module {Core}

       This class is the general purpose N dimensions vector class.
     */
    template <int N, typename T> struct Vector : public VectorBase<N, T>
    {
      inline Vector();
      inline Vector(const VectorBase<N, T> &v);
      /** Create a 2d vector with same value for all components */
      inline Vector(T v);
    };

    /** 
       @short 2d vector class
       @header <goptical/core/math/Vector
       @module {Core}
       @alias Vector2

       This class implements 2d vectors.

       The @ref math namespace contains some commonly used constant vector objects.
     */
    template <typename T> struct Vector<2, T> : public VectorBase<2, T>
    {
      typedef VectorBase<2, T> base;
      typedef delegate<void (const math::Vector2 &)> put_delegate_t;

      inline Vector();

      inline Vector(const VectorBase<2, T> &v);

      template <int M>
      inline Vector(const VectorBase<M, T> &v, T p);

      /** Create a 2d vector with same value for all components */
      inline Vector(T v);

      /** Create a 2d vector from x and y z values */
      inline Vector(T x, T y);

      /** Create a 2d vector and initialize from specified components of an other vector */
      template <int N>
      inline Vector(const VectorBase<N, T> &v, unsigned int c0, unsigned int c1);

      inline T cross_product(const Vector<2, T> &v) const;

      /** Get reference to vector x value */
      inline T & x();
      /** Get reference to vector y value */
      inline T & y();
      /** Get vector x value */
      inline T x() const;
      /** Get vector y value */
      inline T y() const;
    };

    /**
       @short 3d vector class
       @header <goptical/core/math/Vector
       @module {Core}
       @alias Vector3

       This class implements 3d vectors.

       The @ref math namespace contains some commonly used constant vector objects.
     */
    template <typename T> struct Vector<3, T> : public VectorBase<3, T>
    {
      typedef VectorBase<3, T> base;
      typedef delegate<void (const Vector<3, T> &)> put_delegate_t;

      inline Vector();

      /** Copy constructor */
      inline Vector(const VectorBase<3, T> &v);

      /** Create a 3d vector from 2d vector and given z value. */
      template <int M>
      inline Vector(const VectorBase<M, T> &v, T p);

      /** Create a 2d vector with same value for all components */
      inline Vector(T v);
      /** Create a 3d vector from x, y and z values */
      inline Vector(T x, T y, T z);
      /** Get a 2d vector formed x and y values. */
      inline Vector<2, T> project_xy() const;
      /** Get a 2d vector formed z and y values. */
      inline Vector<2, T> project_zy() const;
      /** Compute vectors cross product */
      inline Vector cross_product(const Vector<3, T> &v) const;

      /** Get reference to vector x value */
      inline T & x();
      /** Get reference to vector y value */
      inline T & y();
      /** Get reference to vector z value */
      inline T & z();

      /** Get vector x value */
      inline T x() const;
      /** Get vector y value */
      inline T y() const;
      /** Get vector z value */
      inline T z() const;
    };

    template <int N, typename T>
    std::ostream & operator<<(std::ostream &o, const VectorBase<N, T> &v);

    /** @multiple @showvalue Commonly used vector constant */
    static const Vector2 vector2_0 = Vector2(0.0, 0.0);
    static const Vector2 vector2_1 = Vector2(1.0, 1.0);

    static const Vector2 vector2_01 = Vector2(0.0, 1.0);
    static const Vector2 vector2_10 = Vector2(1.0, 0.0);

    static const Vector3 vector3_0 = Vector3(0.0, 0.0, 0.0);
    static const Vector3 vector3_1 = Vector3(1.0, 1.0, 1.0);

    static const Vector3 vector3_001 = Vector3(0.0, 0.0, 1.0);
    static const Vector3 vector3_010 = Vector3(0.0, 1.0, 0.0);
    static const Vector3 vector3_100 = Vector3(1.0, 0.0, 0.0);
    /** */
  }
}

#endif

