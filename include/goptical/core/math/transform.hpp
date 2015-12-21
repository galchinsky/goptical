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


#ifndef GOPTICAL_MATH_TRANSFORM_HH_
#define GOPTICAL_MATH_TRANSFORM_HH_

#include <ostream>

#include "goptical/core/common.hpp"

#include "goptical/core/math/vector.hpp"
#include "goptical/core/math/vector_pair.hpp"
#include "goptical/core/math/matrix.hpp"
#include "goptical/core/math/quaternion.hpp"

namespace _goptical {

  namespace math {

    template <int N> class TransformBase;

    template <int N>
    inline std::ostream & operator<<(std::ostream &o, const TransformBase<N> &t);

    /**
       @short Linear and affine transformation base class
       @header <goptical/core/math/Transform
       @module {Core}
       @internal

       This class is the base class for linear and affine
       transformations in N dimensions.
     */

    template <int N> class TransformBase
    {
      friend std::ostream & operator<< <>(std::ostream &o, const TransformBase<N> &t);

    public:
      inline TransformBase();
      inline virtual ~TransformBase();

      /** get translation vector */
      inline const Vector<N> & get_translation() const;
      /** get translation vector */
      inline Vector<N> & get_translation();

      /** get linear transform matrix */
      inline const Matrix<N> & get_linear() const;
      /** get linear transform matrix */
      inline Matrix<N> & get_linear();

      /** set current translation */
      inline void set_translation(const Vector<N> &v);

      /** apply translation to current transform */
      inline TransformBase & apply_translation(const Vector<N> &v);

      /** apply rotation to current transform (degree) (does not transform translation vector) */
      inline TransformBase & linear_rotation(unsigned int axis, double dangle);

      /** apply rotation to current transform (radian) (does not transform translation vector) */
      inline TransformBase & linear_rotation_rad(unsigned int axis, double rangle);

      /** apply linear scaling to current transform (does not transform translation vector) */
      inline TransformBase & linear_scaling(const Vector<N> &v);

      /** apply scaling to current transform (does not transform translation vector) */
      inline TransformBase & linear_scaling(double s);

      /** apply scaling to current transform */
      inline TransformBase & affine_scaling(const Vector<N> &v);

      /** apply scaling to current transform */
      inline TransformBase & affine_scaling(double s);

      /** apply rotation to current transform (degree) */
      inline TransformBase & affine_rotation(unsigned int axis, double dangle);

      /** apply rotation to current transform (radian) */
      inline TransformBase & affine_rotation_rad(unsigned int axis, double rangle);

      /** reset linear transform to identity and translation to zero */
      inline void reset();
      /** reset linear transform to identity */
      inline void linear_reset();
      /** reset translation to zero */
      inline void translation_reset();

      /** compose with t. new transform is equivalent to applying t
          then this transform */
      inline void compose(const TransformBase<N> &t);

      /** get inverse transformation */
      inline TransformBase inverse() const;

      /** apply translation to vector */
      inline Vector<N> transform_translate(const Vector<N> &v) const;
      /** apply linear transform to vector */
      inline Vector<N> transform_linear(const Vector<N> &v) const;
      /** apply affine transform (translation and linear) to vector */
      inline Vector<N> transform(const Vector<N> &v) const;

      /** apply affine transform to line origin and linear to direction */
      inline VectorPair<N> transform_line(const VectorPair<N> &v) const;
      /** apply affine transform to both vectors in pair */
      inline VectorPair<N> transform_pair(const VectorPair<N> &p) const;

    protected:

      Vector<N> _translation;
      Matrix<N> _linear;
      bool      _use_linear;
    };

    // **********************************************************************

    /**
       @short 2d linear and affine transformation class.
       @header <goptical/core/math/Transform
       @module {Core}
       @alias Transform2

       This class describes linear and affine transformations in 2d
       space. It contains a 2x2 transformation matrix and a 2d
       translation vector. It can be used for translation and rotation in 2d.
     */

    template <> class Transform<2> : public TransformBase<2>
    {
    public:
      inline Transform<2>();
      inline Transform<2>(const TransformBase<2> &t);

    };

    /** Setup a 2d rotation matrix with specified angle in
        radian. axis parameter must be 0. */
    void get_rotation_matrix(Matrix<2> &m, unsigned int axis, double rangle);

    // **********************************************************************

    /**
       @short 3d linear and affine transformation class.
       @header <goptical/core/math/Transform
       @module {Core}
       @alias Transform3

       This class describes linear and affine transformations in 3d
       space. It contains a 3x3 transformation matrix and a 3d
       translation vector. It can be used for translation and rotation in 3d.
     */

    template <> class Transform<3> : public TransformBase<3>
    {
    public:
      inline Transform<3>();
      inline Transform<3>(const TransformBase<3> &t);

      /** Create a transform from rotation quaternion and translation vector */
      inline Transform<3>(const Quaternion &q, const Vector3 &v);

      using TransformBase<3>::transform;

      /** set rotation from given quaternion */
      inline void set_rotation(const Quaternion &q);

      /** set rotation from given direction vector */
      inline void set_direction(const Vector3 &d);

      /** apply rotation to current transform (degree) (does not transform translation vector) */
      inline Transform<3> & linear_rotation(const Vector3 &dangles);

      /** apply rotation to current transform (radian) (does not transform translation vector) */
      inline Transform<3> & linear_rotation_rad(const Vector3 &rangles);

      /** apply rotation to current transform (degree) */
      inline Transform<3> & affine_rotation(const Vector3 &rangles);

      /** apply rotation to current transform (radian) */
      inline Transform<3> & affine_rotation_rad(const Vector3 &rangles);
    };

    /** Setup a 3d rotation matrix with specified angle in
        radian. axis parameter can be 0, 1 or 2 for x, y and z
        axes. */
    void get_rotation_matrix(Matrix<3> &m, unsigned int axis, double rangle);

  }
}

#endif

