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


#ifndef GOPTICAL_MATH_TRANSFORM_HXX_
#define GOPTICAL_MATH_TRANSFORM_HXX_

#include "goptical/core/math/vector.hxx"
#include "goptical/core/math/vector_pair.hxx"
#include "goptical/core/math/matrix.hxx"
#include "goptical/core/math/quaternion.hxx"

namespace _goptical {

  namespace math {

    template <int N>
    TransformBase<N>::TransformBase()
    {
    }

    template <int N>
    TransformBase<N>::~TransformBase()
    {
    }

    template <int N>
    const Vector<N> & TransformBase<N>::get_translation() const
    {
      return _translation;      
    }

    template <int N>
    Vector<N> & TransformBase<N>::get_translation()
    {
      return _translation;      
    }

    template <int N>
    void TransformBase<N>::set_translation(const Vector<N> &v)
    {
      _translation = v;
    }

    template <int N>
    TransformBase<N> & TransformBase<N>::apply_translation(const Vector<N> &v)
    {
      _translation += v;

      return *this;
    }

    template <int N>
    TransformBase<N> & TransformBase<N>::affine_scaling(const Vector<N> &v)
    {
      _translation = _translation.mul(v);

      for (unsigned int i = 0; i < N; i++)
        for (unsigned int j = 0; j < N; j++)
          _linear.value(i, j) = _linear.value(i, j) * v[i];

      _use_linear = true;

      return *this;      
    }

    template <int N>
    TransformBase<N> & TransformBase<N>::linear_scaling(const Vector<N> &v)
    {
      for (unsigned int i = 0; i < N; i++)
        for (unsigned int j = 0; j < N; j++)
          _linear.value(i, j) = _linear.value(i, j) * v[i];

      _use_linear = true;

      return *this;      
    }

    template <int N>
    TransformBase<N> & TransformBase<N>::affine_scaling(double s)
    {
      _translation = _translation * s;
      _linear = _linear * s;
      _use_linear = true;

      return *this;      
    }

    template <int N>
    TransformBase<N> & TransformBase<N>::linear_scaling(double s)
    {
      _linear = _linear * s;

      _use_linear = true;

      return *this;      
    }

    template <int N>
    TransformBase<N> & TransformBase<N>::affine_rotation(unsigned int axis, double dangle)
    {
      return affine_rotation_rad(axis, degree2rad(dangle));
    }

    template <int N>
    TransformBase<N> & TransformBase<N>::affine_rotation_rad(unsigned int axis, double rangle)
    {
      Matrix<N> r;

      get_rotation_matrix(r, axis, rangle);

      _translation = r * _translation;
      _linear = r * _linear;
      _use_linear = true;

      return *this;
    }

    template <int N>
    TransformBase<N> & TransformBase<N>::linear_rotation(unsigned int axis, double dangle)
    {
      return linear_rotation_rad(axis, degree2rad(dangle));
    }

    template <int N>
    TransformBase<N> & TransformBase<N>::linear_rotation_rad(unsigned int axis, double rangle)
    {
      Matrix<N> r;

      get_rotation_matrix(r, axis, rangle);

      _linear = r * _linear;
      _use_linear = true;

      return *this;
    }

    template <int N>
    const Matrix<N> & TransformBase<N>::get_linear() const
    {
      return _linear;
    }

    template <int N>
    Matrix<N> & TransformBase<N>::get_linear()
    {
      const_cast<TransformBase<N>*>(this)->_use_linear = true;
      return _linear;
    }

    template <int N>
    void TransformBase<N>::linear_reset()
    {
      _linear.set(0);
      _linear.set_diag(1.0);
      _use_linear = false;
    }

    template <int N>
    void TransformBase<N>::translation_reset()
    {
      _translation.set(0.0);
    }

    template <int N>
    void TransformBase<N>::reset()
    {
      linear_reset();
      translation_reset();
    }

    template <int N>
    Vector<N> TransformBase<N>::transform_translate(const Vector<N> &v) const
    {
      return v + _translation;
    }

    template <int N>
    Vector<N> TransformBase<N>::transform_linear(const Vector<N> &v) const
    {
      if (_use_linear)
        return _linear * v;
      else
        return v;
    }

    template <int N>
    Vector<N> TransformBase<N>::transform(const Vector<N> &v) const
    {
      return transform_linear(v) + _translation;
    }

    template <int N>
    VectorPair<N> TransformBase<N>::transform_line(const VectorPair<N> &v) const
    {
      return VectorPair<N> (transform(v.origin()),
                            transform_linear(v.direction()));
    }

    template <int N>
    VectorPair<N> TransformBase<N>::transform_pair(const VectorPair<N> &p) const
    {
      return VectorPair<N>(transform(p[0]), transform(p[1]));
    }

    template <int N>
    TransformBase<N> TransformBase<N>::inverse() const
    {
      TransformBase<N> r;

      r._linear = _linear.inverse();
      r._use_linear = true;
      r._translation = r.transform_linear(-_translation);

      return r;
    }

    template <int N>
    void TransformBase<N>::compose(const TransformBase<N> &t)
    {
      _translation = t.transform_linear(_translation) + t._translation;

      _use_linear |= t._use_linear;
      _linear = t._linear * _linear;
    }

    template <int N>
    std::ostream & operator<<(std::ostream &o, const TransformBase<N> &t)
    {
      o << "[ "<< N << "D transform, use_linear=" << t._use_linear << std::endl;
      o << t._translation << std::endl;
      o << t._linear << "]" <<  std::endl;

      return o;
    }

    // **********************************************************************

    Transform<2>::Transform()
    {
    }

    Transform<2>::Transform(const TransformBase<2> &t)
      : TransformBase<2>(t)
    {
    }

    // **********************************************************************

    Transform<3>::Transform()
    {
    }

    Transform<3>::Transform(const TransformBase<3> &t)
      : TransformBase<3>(t)
    {
    }

    Transform<3>::Transform(const Quaternion &q, const Vector3 &v)
    {
      set_rotation(q);
      set_translation(v);
    }

    void Transform<3>::set_rotation(const Quaternion &q)
    {
      _use_linear = true;
      _linear.value(0, 0) = 1.0 - 2.0 * (q.y() * q.y() + q.z() * q.z());
      _linear.value(1, 0) = 2.0 * (q.x() * q.y() + q.z() * q.w());
      _linear.value(2, 0) = 2.0 * (q.x() * q.z() - q.y() * q.w());

      _linear.value(0, 1) = 2.0 * (q.x() * q.y() - q.z() * q.w());
      _linear.value(1, 1) = 1.0 - 2.0 * (q.x() * q.x() + q.z() * q.z());
      _linear.value(2, 1) = 2.0 * (q.z() * q.y() + q.x() * q.w());

      _linear.value(0, 2) = 2.0 * (q.x() * q.z() + q.y() * q.w());
      _linear.value(1, 2) = 2.0 * (q.y() * q.z() - q.x() * q.w());
      _linear.value(2, 2) = 1.0 - 2.0 * (q.x() * q.x() + q.y() * q.y());
    }

    void Transform<3>::set_direction(const Vector3 &v)
    {
      if (v.x() == 0.0 && v.y() == 0.0)
        {
          linear_reset();
          if (v.z() < 0.0)
            get_linear().value(2, 2) = -1.0;
        }
      else
        {
          set_rotation(math::Quaternion(math::vector3_001, v));
        }
    }

    Transform<3> & Transform<3>::linear_rotation(const Vector3 &v)
    {
      for (unsigned int i = 0; i < 3; i++)
        if (v[i] != 0.0)
          TransformBase<3>::linear_rotation(i, v[i]);

      return *this;
    }

    Transform<3> & Transform<3>::linear_rotation_rad(const Vector3 &v)
    {
      for (unsigned int i = 0; i < 3; i++)
        if (v[i] != 0.0)
          TransformBase<3>::linear_rotation_rad(i, v[i]);

      return *this;
    }

    Transform<3> & Transform<3>::affine_rotation(const Vector3 &v)
    {
      for (unsigned int i = 0; i < 3; i++)
        if (v[i] != 0.0)
          TransformBase<3>::affine_rotation(i, v[i]);

      return *this;
    }

    Transform<3> & Transform<3>::affine_rotation_rad(const Vector3 &v)
    {
      for (unsigned int i = 0; i < 3; i++)
        if (v[i] != 0.0)
          TransformBase<3>::affine_rotation_rad(i, v[i]);

      return *this;
    }

  }

}

#endif

