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


#ifndef GOPTICAL_DATA_SET1D_INTERPOLATE_HH_
#define GOPTICAL_DATA_SET1D_INTERPOLATE_HH_

#include <vector>

#include "goptical/core/common.hpp"

namespace _goptical {

  namespace data {

    /** @internal */
    enum cubic_2nd_deriv_init_e
      {
        Cubic2ndDerivQuadratic,
        Cubic2ndDerivFirst,
        Cubic2ndDerivSecond,
      };

    /**
       @short Common iterpolation code for 1d data sets
       @module {Core}
       @internal
     */
    template <class X> class Interpolate1d : public X
    {
    public:
      Interpolate1d();
      ~Interpolate1d();

      inline double interpolate(const double x) const;
      inline double interpolate(const double x, unsigned int deriv) const;

      void set_interpolation(Interpolation i);

    private:
      /** quadratic and cubic polynomial coefficients */
      struct poly_s
      {
        double a, b, c, d;
      };

      /** set polynomial coefficients of line through 2 points */
      static void set_linear_poly(struct poly_s &p,
                                  double p1x, double p1y,
                                  double p2x, double p2y);

      /** set polynomial coefficients of line through 1 points with given derivative */
      static void set_linear_poly(struct poly_s &p,
                                  double p1x, double p1y,
                                  double d1);

      /** set polynomial coefficients of parabola through 3 points */
      static void set_quadratic_poly(struct poly_s &p,
                                     double p1x, double p1y,
                                     double p2x, double p2y,
                                     double p3x, double p3y);

      /** set polynomial coefficients of parabola through 1 points with
          given first and second derivatives */
      static void set_quadratic_poly(struct poly_s &p,
                                     double px, double py,
                                     double d, double dd);

      /** set polynomial coefficients of cubic through 2 points with
          given first derivatives */
      static void set_cubic_poly(struct poly_s &p,
                                 double p1x, double p1y,
                                 double p2x, double p2y,
                                 double d1, double d2);

      /** set polynomial coefficients of cubic through 2 points with
          given second derivatives */
      static void set_cubic_poly2(struct poly_s &p,
                                  double p1x, double p1y,
                                  double p2x, double p2y,
                                  double dd1, double dd2);

      /** compute cubic sencond derivatives using the tridiagonal algorithm */
      template <cubic_2nd_deriv_init_e de>
      void compute_cubic_2nd_deriv(unsigned int n, double dd[],
                                   double d0, double dn) const;

      double update_nearest(unsigned int d, double x) const;
      double interpolate_nearest(unsigned int d, double x) const;

      double update_linear(unsigned int d, double x) const;
      double interpolate_linear(unsigned int d, double x) const;

      double update_quadratic(unsigned int d, double x) const;
      double interpolate_quadratic(unsigned int d, double x) const;

      double update_cubic(unsigned int d, double x) const;
      double update_cubic2(unsigned int d, double x) const;
      double update_cubic_deriv(unsigned int d, double x) const;
      double update_cubic2_deriv(unsigned int d, double x) const;
      double update_cubic_simple(unsigned int d, double x) const;
      double update_cubic_deriv_init(unsigned int d, double x) const;
      double update_cubic2_deriv_init(unsigned int d, double x) const;
      double interpolate_cubic(unsigned int d, double x) const;

      void invalidate();

      double (Interpolate1d::*_update)(unsigned int d, double x) const;
      double (Interpolate1d::*_interpolate)(unsigned int d, double x) const;

      std::vector<struct poly_s>        _poly;
    };

  }
}

#endif

