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


#ifndef GOPTICAL_MATH_VECTORPAIR_HH_
#define GOPTICAL_MATH_VECTORPAIR_HH_

#include <ostream>

#include "goptical/core/common.hpp"

#include "goptical/core/error.hpp"
#include "goptical/core/math/vector.hpp"

namespace _goptical {

  namespace math {

    /**
       @short Vector pair base class
       @header <goptical/core/math/VectorPair
       @module {Core}
       @internal

       This class is the base class for N dimensions vector pair.
     */
    template <int N> struct VectorPairBase
    {
      inline VectorPairBase();

      inline VectorPairBase(const Vector<N> &a, const Vector<N> &b);

      /** Get a reference to a vector stored in the pair */
      inline const Vector<N> & operator[](int n) const;
      /** Get a reference to a vector stored in the pair */
      inline Vector<N> & operator[](int n);

      inline const VectorPairBase & operator+=(const VectorPairBase &p);
      inline VectorPairBase operator*(double factor);

      /** Get reference to first vector */
      inline Vector<N> & origin();
      /** Get reference to first vector */
      inline const Vector<N> & origin() const;

      /** Get reference to second vector */
      inline Vector<N> & direction();
      /** Get reference to second vector */
      inline const Vector<N> & direction() const;

      /** Get reference to second vector */
      inline Vector<N> & normal();
      /** Get reference to second vector */
      inline const Vector<N> & normal() const;

      /** Consider the @ref VectorPair as a segment with two point
          vectors and find the closest point on this segment to a
          point in space.

          @return position of the closest point on segment.
          @see seg_pt_clst_pt_scale
      */
      inline Vector<N> seg_pt_clst_pt(const Vector<N> &point) const;

      /** Consider the @ref VectorPair as a segment with two point
          vectors and find the closest point on this segment to a
          point in space.

          @return position on segment, value is in [0, 1] range if on segment.
          @see seg_pt_clst_pt
      */
      inline double seg_pt_clst_pt_scale(const Vector<N> &point) const;

      /** Consider the @ref VectorPair as a line with origin and
          direction vectors and find the closest point on this line to
          a point.

          @return position of the closest point on the line.
          @see ln_pt_clst_pt_scale
      */
      inline Vector<N> ln_pt_clst_pt(const Vector<N> &point) const;

      /** Consider the @ref VectorPair as a line with origin and
          direction vectors and find the closest point on this line to
          an other line.

          @return scale factor of the direction vector from origin.
          @see ln_pt_clst_pt
      */
      inline double ln_pt_clst_pt_scale(const Vector<N> &point) const;

    protected:
      Vector<N> _v[2];
    };

    /**
       @short Vector pair class
       @header <goptical/core/math/VectorPair
       @module {Core}

       This class hold two N dimensions vectors.
     */
    template <int N> struct VectorPair : public VectorPairBase<N>
    {
      inline VectorPair();

      inline VectorPair(const VectorPairBase<N> &vp);

      inline VectorPair(const Vector<N> &a, const Vector<N> &b);
    };

    /**
       @short Vector pair class
       @header <goptical/core/math/VectorPair
       @module {Core}
       @alias VectorPair2

       This class hold two 2d vectors.
     */
    template <> struct VectorPair<2> : public VectorPairBase<2>
    {
      inline VectorPair();

      inline VectorPair(const VectorPairBase<2> &vp);

      inline VectorPair(const Vector<2> &a, const Vector<2> &b);

      inline VectorPair(double ax, double ay, double bx, double by);

      /** @multiple Get vector pair component */
      inline double x0() const;
      inline double &x0();
      inline double y0() const;
      inline double &y0();
      inline double x1() const;
      inline double &x1();
      inline double y1() const;
      inline double &y1();

      /** Create a 2d vector pair and initialize vectors from
          specified components of vectors from an other pair. */
      template <int N>
      inline VectorPair(const VectorPairBase<N> &v, unsigned int c0, unsigned int c1);

      /** Consider the @ref VectorPair as a line with origin and
          direction vectors and find the intersection point with an
          other line.

          @return position of the intersection point.
          @this throws if lines are almost parallel.
          @see ln_intersect_ln_scale
      */
      inline Vector<2> ln_intersect_ln(const VectorPair<2> &line) const throw (Error);

      /** Consider the @ref VectorPair as a line with origin and
          direction vectors and find the intersection point with an
          other line.

          @return scale factor of the direction vector from line origin.
          @this throws if lines are almost parallel.
          @see ln_intersect_ln
      */
      inline double ln_intersect_ln_scale(const VectorPair<2> &line) const throw (Error);

      /** Consider the @ref VectorPair as a segment with two point
          vectors and find the intersection point with an other segment.

          @return position of the intersection point.          

          The @tt infinite_1st and @tt infinite_2nd template parameters
          can be used to disable point-in-segment tests for each segments.
          @this throws if lines are almost parallel or if intersection
          point lies outside segment and associated test is enabled.
      */
      template <bool infinite_1st, bool infinite_2nd>
      inline Vector<2> seg_intersect_seg(const VectorPair<2> &segment) const throw (Error);
    };

    /**
       @short Vector pair class
       @header <goptical/core/math/VectorPair
       @module {Core}
       @alias VectorPair3

       This class hold two 3d vectors.
     */
    template <> struct VectorPair<3> : public VectorPairBase<3>
    {
      inline VectorPair();

      inline VectorPair(const VectorPairBase<3> &vp);

      inline VectorPair(const Vector<3> &a, const Vector<3> &b = vector3_001);

      inline VectorPair(double ax, double ay, double az,
                        double bx = 0.0, double by = 0.0, double bz = 1.0);

      /** @multiple Get vector pair component */
      inline double x0() const;
      inline double &x0();
      inline double y0() const;
      inline double &y0();
      inline double z0() const;
      inline double &z0();
      inline double x1() const;
      inline double &x1();
      inline double y1() const;
      inline double &y1();
      inline double z1() const;
      inline double &z1();

      /** Consider the @ref VectorPair as a line with origin and
          direction vectors and find the closest point on this line to
          an other line.

          @return position of the closest point on the line.
          @this throws if lines are almost parallel.
          @see ln_ln_clst_pt_scale
      */
      inline Vector<3> ln_ln_clst_pt(const VectorPair<3> &line) const throw (Error);

      /** Consider the @ref VectorPair as a line with origin and
          direction vectors and find the closest point on this line to
          an other line.

          @return scale factor of the direction vector from origin.
          @this throws if lines are almost parallel.
          @see ln_ln_clst_pt
      */
      inline double ln_ln_clst_pt_scale(const VectorPair<3> &line) const throw (Error);

      /** Consider the @ref VectorPair as a plane and find
          intersection point with a line. Plane is defined by origin
          and normal vectors and line is defined by origin and
          direction vectors.

          @return position of the intersection point on the line.
          @see pl_ln_intersect_scale
      */
      inline Vector<3> pl_ln_intersect(const VectorPair<3> &line) const;

      /** Consider the @ref VectorPair as a plane and find
          intersection point with a line. Plane is defined by origin
          and normal vectors and line is defined by origin and
          direction vectors.

          @return scale factor of the line direction vector from line origin.
          @see pl_ln_intersect
      */
      inline double pl_ln_intersect_scale(const VectorPair<3> &line) const;

    };

    template <int N>
    inline std::ostream & operator<<(std::ostream &o, const VectorPairBase<N> &l);

    static const VectorPair<2> vector2_pair_00 = VectorPair<2>(vector2_0, vector2_0);
    static const VectorPair<2> vector2_pair_01 = VectorPair<2>(vector2_0, vector2_1);

    static const VectorPair<3> vector3_pair_00 = VectorPair<3>(vector3_0, vector3_0);
    static const VectorPair<3> vector3_pair_01 = VectorPair<3>(vector3_0, vector3_1);

  }
}

#endif

