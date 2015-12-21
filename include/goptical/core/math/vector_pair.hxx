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

#ifndef GOPTICAL_MATH_VECTORPAIR_HXX_
#define GOPTICAL_MATH_VECTORPAIR_HXX_

#include "goptical/core/math/vector.hxx"

namespace _goptical {

  namespace math {

    template <int N>
    VectorPairBase<N>::VectorPairBase()
    {
    }

    template <int N>
    VectorPairBase<N>::VectorPairBase(const Vector<N> &first, const Vector<N> &second)
    {
      _v[0] = first;
      _v[1] = second;
    }

    template <int N>
    const Vector<N> & VectorPairBase<N>::operator[](int n) const
    {
      return _v[n];
    }

    template <int N>
    Vector<N> & VectorPairBase<N>::operator[](int n)
    {
      return _v[n];
    }

    template <int N>
    const VectorPairBase<N> & VectorPairBase<N>::operator+=(const VectorPairBase<N> &p)
    {
      for (unsigned int i = 0; i < N; i++)
        _v[i] += p._v[i];

      return *this;
    }

    template <int N>
    VectorPairBase<N> VectorPairBase<N>::operator*(double factor)
    {
      return VectorPairBase(_v[0] * factor, _v[1] * factor);
    }

    template <int N>
    Vector<N> & VectorPairBase<N>::origin()
    {
      return _v[0];
    }

    template <int N>
    const Vector<N> & VectorPairBase<N>::origin() const
    {
      return _v[0];
    }

    template <int N>
    Vector<N> & VectorPairBase<N>::direction()
    {
      return _v[1];
    }

    template <int N>
    const Vector<N> & VectorPairBase<N>::direction() const
    {
      return _v[1];
    }

    template <int N>
    Vector<N> & VectorPairBase<N>::normal()
    {
      return _v[1];
    }

    template <int N>
    const Vector<N> & VectorPairBase<N>::normal() const
    {
      return _v[1];
    }

    template <int N>
    double VectorPairBase<N>::seg_pt_clst_pt_scale(const Vector<N> &point) const
    {
      Vector<N> d = _v[1] - _v[0];
      return ((point - _v[0]) * d) / (d * d);
    }

    template <int N>
    Vector<N> VectorPairBase<N>::seg_pt_clst_pt(const Vector<N> &point) const
    {
      Vector<N> d = _v[1] - _v[0];
      double s = std::min(std::max(((point - _v[0]) * d) / (d * d), 0.0), 1.0);
      return _v[0] + d * s;
    }

    template <int N>
    double VectorPairBase<N>::ln_pt_clst_pt_scale(const Vector<N> &point) const
    {
      return _v[1] * (point - _v[0]);
    }

    template <int N>
    Vector<N> VectorPairBase<N>::ln_pt_clst_pt(const Vector<N> &point) const
    {
      return _v[0] + _v[1] * ln_pt_clst_pt_scale(point);
    }

    template <int N>
    std::ostream & operator<<(std::ostream &o, const VectorPairBase<N> &l)
    {
      o << "[" << l[0] << ", " << l[1] << "]";

      return o;
    }

    // **********************************************************************

    template <int N>
    VectorPair<N>::VectorPair()
    {
    }

    template <int N>
    VectorPair<N>::VectorPair(const VectorPairBase<N> &vp)
      : VectorPairBase<N>(vp)
    {
    }

    template <int N>
    VectorPair<N>::VectorPair(const Vector<N> &first, const Vector<N> &second)
      : VectorPairBase<N>(first, second)
    {
    }

    // **********************************************************************

    VectorPair<2>::VectorPair()
    {
    }

    VectorPair<2>::VectorPair(const VectorPairBase<2> &vp)
      : VectorPairBase<2>(vp)
    {
    }

    VectorPair<2>::VectorPair(const Vector<2> &first, const Vector<2> &second)
      : VectorPairBase<2>(first, second)
    {
    }

    VectorPair<2>::VectorPair(double ax, double ay,
                              double bx, double by)
      : VectorPairBase<2>(math::Vector<2>(ax, ay),
                          math::Vector<2>(bx, by))
    {
    }

    double VectorPair<2>::x0() const
    {
      return _v[0].x();
    }

    double & VectorPair<2>::x0()
    {
      return _v[0].x();
    }

    double VectorPair<2>::y0() const
    {
      return _v[0].y();
    }

    double & VectorPair<2>::y0()
    {
      return _v[0].y();
    }

    double VectorPair<2>::x1() const
    {
      return _v[1].x();
    }

    double & VectorPair<2>::x1()
    {
      return _v[1].x();
    }

    double VectorPair<2>::y1() const
    {
      return _v[1].y();
    }

    double & VectorPair<2>::y1()
    {
      return _v[1].y();
    }

    template <int N>
    VectorPair<2>::VectorPair(const VectorPairBase<N> &v, unsigned int c0, unsigned int c1)
      : VectorPairBase<2>(Vector<2>(v[0], c0, c1), Vector<2>(v[1], c0, c1))
    {
    }

    double VectorPair<2>::ln_intersect_ln_scale(const VectorPair<2> &line) const throw (Error)
    {
      // based on http://geometryalgorithms.com/Archive/algorithm_0104/algorithm_0104B.htm

      const Vector<2> w = _v[0] - line._v[0];

      double d = _v[1].x() * line._v[1].y() - _v[1].y() * line._v[1].x();

      if (fabs(d) < 1e-10)
        throw Error("ln_intersect_ln_scale: lines are parallel");

      double s = (line._v[1].x() * w.y() - line._v[1].y() * w.x()) / d;

      return s;
    }

    Vector<2> VectorPair<2>::ln_intersect_ln(const VectorPair<2> &line) const throw (Error)
    {
      return _v[0] + _v[1] * ln_intersect_ln_scale(line);
    }

    template <bool infinite_1st, bool infinite_2nd>
    Vector<2> VectorPair<2>::seg_intersect_seg(const VectorPair<2> &s) const throw (Error)
    {
      double d= ((s.x1()-s.x0())*y1() + (s.x0()-s.x1())*y0() + (s.y0()-s.y1())*x1() + (s.y1()-s.y0())*x0());
      double t1= ((s.x1()-s.x0())*y1() + (s.y0()-s.y1())*x1() + s.x0()*s.y1() - s.x1()*s.y0()) / d;
      double t2=-((x0()-s.x1())*y1() + (s.x1()-x1())*y0() + s.y1()*x1() - s.y1()*x0()) / d;

      if (!infinite_1st && (t1 > 1. || t1 < 0.))
        throw Error("seg_intersect_seg: no intersection");

      if (!infinite_2nd && (t2 > 1. || t2 < 0.))
        throw Error("seg_intersect_seg: no intersection");

      return Vector<2>(x0() * t1 + x1() * (1. - t1),
                       y0() * t1 + y1() * (1. - t1));
    }

    // **********************************************************************

    VectorPair<3>::VectorPair()
    {
    }

    VectorPair<3>::VectorPair(const Vector<3> &first, const Vector<3> &second)
      : VectorPairBase<3>(first, second)
    {
    }

    VectorPair<3>::VectorPair(const VectorPairBase<3> &vp)
      : VectorPairBase<3>(vp)
    {
    }

    VectorPair<3>::VectorPair(double ax, double ay, double az,
                              double bx, double by, double bz)
      : VectorPairBase<3>(math::Vector<3>(ax, ay, az),
                          math::Vector<3>(bx, by, bz))
    {
    }

    double VectorPair<3>::x0() const
    {
      return _v[0].x();
    }

    double & VectorPair<3>::x0()
    {
      return _v[0].x();
    }

    double VectorPair<3>::y0() const
    {
      return _v[0].y();
    }

    double & VectorPair<3>::y0()
    {
      return _v[0].y();
    }

    double VectorPair<3>::z0() const
    {
      return _v[0].z();
    }

    double & VectorPair<3>::z0()
    {
      return _v[0].z();
    }

    double VectorPair<3>::x1() const
    {
      return _v[1].x();
    }

    double & VectorPair<3>::x1()
    {
      return _v[1].x();
    }

    double VectorPair<3>::y1() const
    {
      return _v[1].y();
    }

    double & VectorPair<3>::y1()
    {
      return _v[1].y();
    }

    double VectorPair<3>::z1() const
    {
      return _v[1].z();
    }

    double & VectorPair<3>::z1()
    {
      return _v[1].z();
    }

    double VectorPair<3>::ln_ln_clst_pt_scale(const VectorPair<3> &line) const throw (Error)
    {
      // based on http://geometryalgorithms.com/Archive/algorithm_0106/algorithm_0106.htm

      const math::Vector<3>     &u = _v[1];
      const math::Vector<3>     &v = line._v[1];
      const math::Vector<3>     w = _v[0] - line._v[0];

      double    b = u * v;
      double    c = v * v;
      double    k = (u * u) * c - b * b;

      if (k <= 1e-10)           // throw if lines are almost parallel
        throw Error();

      return (b * (v * w) - c * (u * w)) / k;
    }

    Vector<3> VectorPair<3>::ln_ln_clst_pt(const VectorPair<3> &line) const throw (Error)
    {
      return _v[0] + _v[1] * ln_ln_clst_pt_scale(line);
    }

    double VectorPair<3>::pl_ln_intersect_scale(const VectorPair<3> &line) const
    {
      return (_v[0] * _v[1] - _v[1] * line._v[0]) / (line._v[1] * _v[1]);
    }

    Vector<3> VectorPair<3>::pl_ln_intersect(const VectorPair<3> &line) const
    {
      return line._v[0] + line._v[1] * pl_ln_intersect_scale(line);
    }

  }
}

#endif

