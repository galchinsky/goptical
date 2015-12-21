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

#include <gsl/gsl_multifit.h>

#include <goptical/core/math/Vector>
#include <goptical/core/curve/Zernike>
#include <goptical/core/trace/Distribution>
#include <goptical/core/shape/Disk>

namespace _goptical {

  namespace curve {

    const trace::Distribution Zernike::default_dist(trace::HexaPolarDist, 10);

    struct zp_precalc_s
    {
      double x, x2;
      double y, y2;
      double r;

      inline zp_precalc_s(double x_, double y_)
        : x(x_),
          x2(x_ * x_),
          y(y_),
          y2(y_ * y_),
          r(x2 + y2)
      {
      }
    };

    typedef double (zp_t)(const struct zp_precalc_s &p);
    typedef void (zp_d_t)(const struct zp_precalc_s &p, math::Vector2 & dxdy);

    // Zernike polynomial 0

    static double zp_0(const struct zp_precalc_s &p)
    {
      return 1;
    }

    // Zernike polynomial 1

    static double zp_1(const struct zp_precalc_s &p)
    {
      return p.x;
    }

    static void zp_1_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = 1.0;
      dxdy.y() = 0.0;
    }

    // Zernike polynomial 2

    static double zp_2(const struct zp_precalc_s &p)
    {
      return p.y;
    }

    static void zp_2_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = 0.0;
      dxdy.y() = 1.0;
    }

    // Zernike polynomial 3

    static double zp_3(const struct zp_precalc_s &p)
    {
      return -1.0 + 2.0 * p.r;
    }

    static void zp_3_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = 4.0 * p.x;
      dxdy.y() = 4.0 * p.y;
    }

    // Zernike polynomial 4

    static double zp_4(const struct zp_precalc_s &p)
    {
      return p.x2 - p.y2;
    }

    static void zp_4_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = 2.0 * p.x;
      dxdy.y() = -2.0 * p.y;
    }

    // Zernike polynomial 5

    static double zp_5(const struct zp_precalc_s &p)
    {
      return 2.0 * p.x * p.y;
    }

    static void zp_5_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = 2.0 * p.y;
      dxdy.y() = 2.0 * p.x;
    }

    // Zernike polynomial 6

    static double zp_6(const struct zp_precalc_s &p)
    {
      return p.x * (-2.0 + 3.0 * p.r);
    }

    static void zp_6_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = 3.0 * p.r + 6.0 * p.x2 - 2.0;
      dxdy.y() = 6.0 * p.x * p.y;
    }

    // Zernike polynomial 7

    static double zp_7(const struct zp_precalc_s &p)
    {
      return p.y * (-2.0 + 3.0 * p.r);
    }

    static void zp_7_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = 6.0 * p.x * p.y;
      dxdy.y() = 3.0 * p.r + 6.0 * p.y2 - 2.0;
    }

    // Zernike polynomial 8

    static double zp_8(const struct zp_precalc_s &p)
    {
      return 1.0 + 6.0 * p.r * (- 1.0 + p.r);
    }

    static void zp_8_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 12.0 * (2 * p.r - 1.0);

      dxdy.x() = p.x * a;
      dxdy.y() = p.y * a;
    }

    // Zernike polynomial 9

    static double zp_9(const struct zp_precalc_s &p)
    {
      return p.x * (p.x2 - 3.0 * p.y2);
    }

    static void zp_9_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = 3.0 * (p.x2 - p.y2);
      dxdy.y() = -6.0 * p.x * p.y;
    }

    // Zernike polynomial 10

    static double zp_10(const struct zp_precalc_s &p)
    {
      return -p.y * (p.y2 - 3.0 * p.x2);
    }

    static void zp_10_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = 6.0 * p.x * p.y;
      dxdy.y() = 3.0 * (p.x2 - p.y2);
    }

    // Zernike polynomial 11

    static double zp_11(const struct zp_precalc_s &p)
    {
      return (p.y2 - p.x2) * (3.0 + (-4.0 * p.r));
    }

    static void zp_11_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = p.x * (16.0 * p.x2 - 6.0);
      dxdy.y() = p.y * (-16.0 * p.y2 + 6.0);
    }

    // Zernike polynomial 12

    static double zp_12(const struct zp_precalc_s &p)
    {
      return p.x * p.y * (-6.0 + 8.0 * p.r);
    }

    static void zp_12_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = p.y * (8.0 * p.y2 + 24.0 * p.x2 - 6.0);
      dxdy.y() = p.x * (8.0 * p.x2 + 24.0 * p.y2 - 6.0);
    }

    // Zernike polynomial 13

    static double zp_13(const struct zp_precalc_s &p)
    {
      return p.x * (3.0 - p.r * (12.0 - 10.0 * p.r));
    }

    static void zp_13_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = (12.0 - 10.0 * p.r);

      dxdy.x() = p.x2 * (20.0 * p.r - 2.0 * a) - a * p.r + 3.0;
      dxdy.y() = 8.0 * p.x * p.y * (5.0 * p.r - 3.0); 
    }

    // Zernike polynomial 14

    static double zp_14(const struct zp_precalc_s &p)
    {
      return p.y * (3.0 - p.r * (12.0 - 10.0 * p.r));
    }

    static void zp_14_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = (12.0 - 10.0 * p.r);

      dxdy.x() = 8.0 * p.x * p.y * (5.0 * p.r - 3.0); 
      dxdy.y() = p.y2 * (20.0 * p.r - 2.0 * a) - a * p.r + 3.0;
    }

    // Zernike polynomial 15

    static double zp_15(const struct zp_precalc_s &p)
    {
      return -1.0 + p.r * (12 - p.r * (30.0 - 20.0 * p.r));
    }

    static void zp_15_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 24.0 * (5.0 * p.r * (p.r - 1.0) + 1.0);

      dxdy.x() = a * p.x;
      dxdy.y() = a * p.y;
    }

    // Zernike polynomial 16

    static double zp_16(const struct zp_precalc_s &p)
    {
      return p.x2*p.x2 + p.y2 * (p.y2 - 6.0 * p.x2);
    }    

    static void zp_16_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = -4.0 * p.x * (3.0 * p.y2 - p.x2);
      dxdy.y() = 4.0 * p.y * (p.y2 - 3.0 * p.x2);
    }

    // Zernike polynomial 17

    static double zp_17(const struct zp_precalc_s &p)
    {
      return 4.0 * p.x * p.y * (p.x2 - p.y2);
    }

    static void zp_17_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = -4.0 * p.y * (p.y2 - 3.0 * p.x2);
      dxdy.y() = -4.0 * p.x * (3.0 * p.y2 - p.x2);
    }

    // Zernike polynomial 18

    static double zp_18(const struct zp_precalc_s &p)
    {
      return p.x * (- 4.0 + 5.0 * p.r) * (p.x2 - 3.0 * p.y2);
    }

    static void zp_18_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = (5.0 * p.r - 4.0);
      double b = (p.x2 - 3.0 * p.y2);

      dxdy.x() = b * (10.0 * p.x2 + a) + 2.0 * a * p.x2;
      dxdy.y() = p.x * p.y * (10.0 * b - 6.0 * a);
    }

    // Zernike polynomial 19

    static double zp_19(const struct zp_precalc_s &p)
    {
      return p.y * (- 4.0 + 5.0 * p.r) * (3.0 * p.x2 - p.y2);
    }

    static void zp_19_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = (3.0 * p.x2 - p.y2);
      double b = (5.0 * p.r - 4.0);

      dxdy.x() = p.x * p.y * (10.0 * a + 6.0 * b);
      dxdy.y() = p.y2 * (-2.0 * b + 10.0 * a) + a * b;
    }

    // Zernike polynomial 20

    static double zp_20(const struct zp_precalc_s &p)
    {
      return (p.x2 - p.y2) * (6.0 - p.r * (20.0 - 15.0 * p.r));
    }

    static void zp_20_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 20.0 - 15.0 * p.r;
      double b = (30.0 * p.r - 2.0 * a) * (p.x2 - p.y2);
      double c = 2.0 * (6.0 - a * p.r);

      dxdy.x() = p.x * (b + c);
      dxdy.y() = p.y * (b - c);
    }

    // Zernike polynomial 21

    static double zp_21(const struct zp_precalc_s &p)
    {
      return p.x * p.y * (12.0 - p.r * (40.0 - 30.0 * p.r));
    }

    static void zp_21_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = (40.0 - 30.0 * p.r);
      double b = (60.0 * p.r - 2.0 * a);
      double c = (12.0 - a * p.r);

      dxdy.x() = p.y * (p.x2 * b + c);
      dxdy.y() = p.x * (p.y2 * b + c);
    }

    // Zernike polynomial 22

    static double zp_22(const struct zp_precalc_s &p)
    {
      return p.x * (-4.0 + p.r * (30.0 - p.r * (60.0 - 35.0 * p.r)));
    }

    static void zp_22_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 60.0 - 35.0 * p.r;
      double b = 30.0 - a * p.r;
      double c = (70.0 * p.r - 2.0 * a);
      double e = (p.r * c + 2.0 * b);

      dxdy.x() = p.x2      * e + b * p.r - 4.0;
      dxdy.y() = p.x * p.y * e;
    }

    // Zernike polynomial 23

    static double zp_23(const struct zp_precalc_s &p)
    {
      return p.y * (-4.0 + p.r * (30.0 - p.r * (60.0 - 35.0 * p.r)));
    }

    static void zp_23_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 60.0 - 35.0 * p.r;
      double b = 30.0 - a * p.r;
      double c = (70.0 * p.r - 2.0 * a);
      double e = (p.r * c + 2.0 * b);

      dxdy.x() = p.x * p.y * e;
      dxdy.y() = p.y2      * e + b * p.r - 4.0;
    }

    // Zernike polynomial 24

    static double zp_24(const struct zp_precalc_s &p)
    {
      return 1.0 - p.r * (20.0 + p.r * (-90.0 - p.r * (-140.0 + 70.0 * p.r)));
    }

    static void zp_24_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 70.0 * p.r - 140.0;
      double b = (p.r * ((140.0 * p.r + 6.0 * a) * p.r + 360.0) - 40.f); 

      dxdy.x() = p.x * b;
      dxdy.y() = p.y * b;
    }

    // Zernike polynomial 25

    static double zp_25(const struct zp_precalc_s &p)
    {
      return p.x * (p.x2 * (p.x2 - 10.0 * p.y2) + 5.0 * p.y2*p.y2);
    }

    static void zp_25_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = 5.0 * math::square(p.y2) + 2.0 * math::square(p.x2) + 3.0 * (p.x2 - 10.0 * p.y2) * p.x2;
      dxdy.y() = 20.0 * p.x * p.y * (p.y2 - p.x2);
    }

    // Zernike polynomial 26

    static double zp_26(const struct zp_precalc_s &p)
    {
      return p.y * (p.y2 * (p.y2 - 10.0 * p.x2) + 5.0 * p.x2*p.x2);
    }

    static void zp_26_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = 20.0 * p.x * p.y * (p.x2 - p.y2);
      dxdy.y() = 5.0 * math::square(p.x2) + 2.0 * math::square(p.y2) + 3.0 * (p.y2 - 10.0 * p.x2) * p.y2;
    }

    // Zernike polynomial 27

    static double zp_27(const struct zp_precalc_s &p)
    {
      return (-math::square(p.x2) + p.y2 * (-p.y2 + p.x2 * 6.0)) * (5.0 - 6.0 * p.r);
    }

    static void zp_27_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() = p.x * (p.y2 * (-60.0 * p.y2 - 120.0 * p.x2 + 60.0) + p.x2 * (36.0 * p.x2 - 20.0));
      dxdy.y() = p.y * (p.x2 * (-60.0 * p.x2 - 120.0 * p.y2 + 60.0) + p.y2 * (36.0 * p.y2 - 20.0));
    }

    // Zernike polynomial 28

    static double zp_28(const struct zp_precalc_s &p)
    {
      return p.x * p.y * (p.x2 - p.y2) * (-20.0 + 24.0 * p.r);
    }

    static void zp_28_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      dxdy.x() =  p.y * (p.y2 * (-24.0 * p.y2 + 20.0) + p.x2 * (120.0 * p.x2 - 60.f));
      dxdy.y() = -p.x * (p.x2 * (-24.0 * p.x2 + 20.0) + p.y2 * (120.0 * p.y2 - 60.f));
    }

    // Zernike polynomial 29

    static double zp_29(const struct zp_precalc_s &p)
    {
      return p.x * (10.0 - p.r * (30.0 - 21.0 * p.r)) * (p.x2 + p.y2 * -3.0);
    }

    static void zp_29_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 30.0 - 21.0 * p.r;
      double b = 10.0 - a * p.r;
      double c = (p.x2 - 3.0 * p.y2);
      double e = (42.0 * p.r - 2.0 * a);

      dxdy.x() = c * (p.x2 * e + b) + 2 * b * p.x2;
      dxdy.y() = p.x * p.y * (e * c - 6.0 * b);
    }

    // Zernike polynomial 30

    static double zp_30(const struct zp_precalc_s &p)
    {
      return -p.y * (10.0 - p.r * (30.0 - 21.0 * p.r)) * (p.y2 + p.x2 * -3.0);
    }

    static void zp_30_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 30.0 - 21.0 * p.r;
      double b = 10.0 - a * p.r;
      double c = (p.y2 - 3.0 * p.x2);
      double e = (42.0 * p.r - 2.0 * a);

      dxdy.x() = p.x * p.y * (6.0 * b - e * c);
      dxdy.y() = -c * (p.y2 * e + b) - 2.0 * b * p.y2;
    }

    // Zernike polynomial 31

    static double zp_31(const struct zp_precalc_s &p)
    {
      return (p.x2 - p.y2) * (-10.0 + p.r * (60.0 - p.r * (105.0 - 56.0 * p.r)));
    }

    static void zp_31_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 105.0 - 56.0 * p.r;
      double b = 60.0 - a * p.r;
      double c = 2.0 * (p.r * (56.0 * p.r - a) + b) * (p.x2 - p.y2);
      double e = 2.0 * (b * p.r - 10.0);

      dxdy.x() = p.x * (c + e);
      dxdy.y() = p.y * (c - e);
    }

    // Zernike polynomial 32

    static double zp_32(const struct zp_precalc_s &p)
    {
      return p.x * p.y * (-20.0 + p.r * (120.0 - p.r * (210.0 - (112.0 * p.r))));
    }

    static void zp_32_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 210.0 - 112.0 * p.r;
      double b = 120.0 - a * p.r;
      double c = 2.0 * (p.r * (112.0 * p.r - a) + b);
      double e = (b * p.r - 20.0);

      dxdy.x() = p.y * (p.x2 * c + e);
      dxdy.y() = p.x * (p.y2 * c + e);
    }

    // Zernike polynomial 33

    static double zp_33(const struct zp_precalc_s &p)
    {
      return p.x * (5.0 + p.r * (-60.0 + p.r * (210.0 - p.r * (280.0 - 126.0 * p.r))));
    }

    static void zp_33_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 280.0 - 126.0 * p.r;
      double b = 210.0 - a * p.r;
      double c = b * p.r - 60.0;
      double e = 2.0 * (p.r * (p.r * (126.0 * p.r - a) + b) + c);

      dxdy.x() = p.x2 * e + c * p.r + 5.0;
      dxdy.y() = p.x * p.y * e;
    }

    // Zernike polynomial 34

    static double zp_34(const struct zp_precalc_s &p)
    {
      return p.y * (5.0 + p.r * (-60.0 + p.r * (210.0 - p.r * (280.0 - 126.0 * p.r))));
    }

    static void zp_34_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 280.0 - 126.0 * p.r;
      double b = 210.0 - a * p.r;
      double c = b * p.r - 60.0;
      double e = 2.0 * (p.r * (p.r * (126.0 * p.r - a) + b) + c);

      dxdy.x() = p.y * p.x * e;
      dxdy.y() = p.y2 * e + c * p.r + 5.0;
    }

    // Zernike polynomial 35

    static double zp_35(const struct zp_precalc_s &p)
    {
      return -1.0 + p.r * (30.0 + p.r * (-210.0 + p.r * (560.0 - p.r * (630.0 - 252.0 * p.r))));
    }

    static void zp_35_d(const struct zp_precalc_s &p, math::Vector2 & dxdy)
    {
      double a = 630.0 - 252.0 * p.r;
      double b = 560.0 - a * p.r;
      double c = b * p.r - 210.0;
      double e = 2.0 * (p.r * (p.r * (p.r * (252.0 * p.r - a) + b) + c) + (c * p.r + 30.0));

      dxdy.x() = p.x * e;
      dxdy.y() = p.y * e;
    }

    ////////////////////////////////////////////////////

    zp_t * const zp[Zernike::term_count] =
    {
      &zp_0,
      &zp_1, &zp_2, &zp_3,
      &zp_4, &zp_5, &zp_6, &zp_7, &zp_8,
      &zp_9, &zp_10, &zp_11, &zp_12, &zp_13, &zp_14, &zp_15,
      &zp_16, &zp_17, &zp_18, &zp_19, &zp_20, &zp_21, &zp_22, &zp_23, &zp_24,
      &zp_25, &zp_26, &zp_27, &zp_28, &zp_29, &zp_30, &zp_31, &zp_32, &zp_33, &zp_34, &zp_35
    };

    zp_d_t * const zp_d[Zernike::term_count] =
    {
      0,
      &zp_1_d, &zp_2_d, &zp_3_d,
      &zp_4_d, &zp_5_d, &zp_6_d, &zp_7_d, &zp_8_d,
      &zp_9_d, &zp_10_d, &zp_11_d, &zp_12_d, &zp_13_d, &zp_14_d, &zp_15_d,
      &zp_16_d, &zp_17_d, &zp_18_d, &zp_19_d, &zp_20_d, &zp_21_d, &zp_22_d, &zp_23_d, &zp_24_d,
      &zp_25_d, &zp_26_d, &zp_27_d, &zp_28_d, &zp_29_d, &zp_30_d, &zp_31_d, &zp_32_d, &zp_33_d, &zp_34_d, &zp_35_d
    };

    Zernike::Zernike(double radius, double unit_scale)
      : _scale(unit_scale),
        _threshold(1e-10),
        _radius(radius),
        _enabled_count(0)
    {
      for (unsigned int i = 0; i < term_count; i++)
        _coeff[i] = 0.0;
    }

    Zernike::Zernike(double radius, double coefs[],
                     unsigned int coefs_count, double unit_scale)
      : _scale(unit_scale),
        _threshold(1e-10),
        _radius(radius),
        _enabled_count(0)
    {
      unsigned int i;

      for (i = 0; i < coefs_count; i++)
        {
          double c = coefs[i];

          _coeff[i] = c;

          if (c >= _threshold)
            _enabled_list[_enabled_count++] = i;
        }

      for (; i < term_count; i++)
        _coeff[i] = 0.0;
    }

    double Zernike::sagitta(const math::Vector2 & xy) const
    {
      struct zp_precalc_s p(xy.x() / _radius, xy.y() / _radius);

      if (p.r > 1.0)
        return 0;

      double sum = 0.0;

      for (unsigned int i = 0; i < _enabled_count; i++)
        {
          unsigned int n = _enabled_list[i];
          sum += zp[n](p) * _coeff[n];
        }

      return sum * _scale;
    }

    void Zernike::derivative(const math::Vector2 & xy, math::Vector2 & dxdy) const
    {
      struct zp_precalc_s p(xy.x() / _radius, xy.y() / _radius);

      dxdy.set(0.0);

      if (p.r > 1.0)
        return;

      for (unsigned int i = 0; i < _enabled_count; i++)
        {
          unsigned int n = _enabled_list[i];

          if (n > 0)
            {
              math::Vector2 dtmp;

              zp_d[n](p, dtmp);
              dxdy += dtmp * (_coeff[n] / _radius);
            }
        }

      dxdy *= _scale;
    }

    double Zernike::fit(const Base &curve, const trace::Distribution & d)
    {
      // get distributed sample points on surface
      std::vector<math::Vector2 > pattern;
      delegate_push<typeof(pattern)> pattern_push(pattern);
      shape::Disk shape(1.0);
      shape.get_pattern(pattern_push, d, false);

      unsigned int pcount = pattern.size();

      gsl_matrix *X = gsl_matrix_alloc(pcount, term_count);
      gsl_vector *y = gsl_vector_alloc(pcount);
      gsl_vector *c = gsl_vector_alloc(term_count);
      gsl_matrix *cov = gsl_matrix_alloc(term_count, term_count);

      unsigned int i = 0;

      // setup least square fit matrix from sample points

      GOPTICAL_FOREACH(pt, pattern)
        {
          struct zp_precalc_s p(pt->x(), pt->y());

          gsl_vector_set(y, i, curve.sagitta(*pt * _radius));
          
          for (unsigned int j = 0; j < term_count; j++)
            gsl_matrix_set(X, i, j, zp[j](p) * _scale);

          i++;
        }

      double chisq;

      // process least square fit

      gsl_multifit_linear_workspace *work = gsl_multifit_linear_alloc(pcount, term_count);
      gsl_multifit_linear(X, y, c, cov, &chisq, work);
      gsl_multifit_linear_free(work);

      gsl_matrix_free(X);
      gsl_vector_free(y);
      gsl_matrix_free(cov);

      for (unsigned int j = 0; j < term_count; j++)
        _coeff[j] = gsl_vector_get(c, j);

      gsl_vector_free(c);

      update_threshold_state();

      return sqrt(chisq / pcount);
    }

    void Zernike::update_threshold_state()
    {
      unsigned int n = 0, i = 0;

      for (; i < term_count; i++)
        if (fabs(_coeff[i]) >= _threshold)
          _enabled_list[n++] = i;

      _enabled_count = n;
    }

    void Zernike::set_term_state(unsigned int n, bool enabled)
    {
      unsigned int i;

      assert(n < term_count);

      if (enabled)
        {
          for (i = 0; i < _enabled_count; i++)
            if (_enabled_list[i] == n)
              return;
          assert(i < term_count);
          _enabled_list[i] = n;
          _enabled_count = i + 1;
        }
      else
        {
          for (i = 0; i < _enabled_count; i++)
            if (_enabled_list[i] == n)
              {
                _enabled_count--;
                _enabled_list[i] = _enabled_list[_enabled_count];
                return;
              }
        }
    }

    bool Zernike::get_term_state(unsigned int n)
    {
      for (unsigned int i = 0; i < _enabled_count; i++)
        if (_enabled_list[i] == n)
          return true;

      return false;
    }

    double Zernike::zernike_poly(unsigned int n, const math::Vector2 & xy)
    {
      struct zp_precalc_s p(xy.x(), xy.y());

      assert(n < term_count);

      return zp[n](p);
    }

    void Zernike::zernike_poly_d(unsigned int n, const math::Vector2 & xy, math::Vector2 & dxdy)
    {
      struct zp_precalc_s p(xy.x(), xy.y());

      assert(n < term_count);

      zp_d[n](p, dxdy);
    }

  }

}
