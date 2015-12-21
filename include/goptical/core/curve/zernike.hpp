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


#ifndef GOPTICAL_CURVE_ZERNIKE_HH_
#define GOPTICAL_CURVE_ZERNIKE_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/trace/distribution.hpp"
#include "base.hpp"

namespace _goptical {

  namespace curve {

    /**
       @short Define surface curve from Zernike polynomials
       @header <goptical/core/curve/Zernike
       @module {Core}
       @main

       This class enables use of zernikes polynomials to define a
       surface curve. Zernikes terms can be enabled and disabled
       individually.

       Fitting can be used to get best fit Zernike polynomials
       coefficients of an other curve.

       This implementation uses Zernike polynomials set as described
       in ISO standard 10110-5.

       Based on Zernikes pages by James C Wyant and Michael Koch.
       @url http://www.optics.arizona.edu/jcwyant/Zernikes/ZernikePolynomials.htm
       and @url http://www.astro-electronic.de/faq2.htm
    */
    class Zernike : public Base
    {
      static const trace::Distribution default_dist;

    public:
      /** Number of implemented zernike polynomials */
      static const unsigned int term_count = 36;

      /** Create a Zernike curve defined over the given circle radius. 
          @param radius Zernike circle radius
          @param unit_scale Sagitta scale factor used to change units globally
      */
      Zernike(double radius, double unit_scale = 1.0);

      /** Create a Zernike curve defined over the given circle radius
          and initialize coefficients from table.
          @param radius Zernike circle radius
          @param coefs Table of Zernike coefficients starting with z0 (piston)
          @param coefs_count Number of coefficients available in the table
          @param unit_scale Sagitta scale factor used to change units globally
      */
      Zernike(double radius, double coefs[], unsigned int coefs_count, double unit_scale = 1.0);

      /** Set Zernike circle radius */
      inline void set_radius(double radius);
      /** Get Zernike circle radius */
      inline double get_radius() const;

      /** Set coefficient associated with zernike term n and enable
          term according to current threshold. See
          set_coefficients_threshold() */
      inline void set_coefficient(unsigned int n, double c);

      /** Get coefficient associated with zernike term n */
      inline double get_coefficient(unsigned int n) const;

      /** Set coefficients unit scale factor. default is 1 (1 mm). */
      inline void set_coefficients_scale(double s);

      /** Set new coefficient threshold and adjust all term states
          (enabled/disabled) depending on new threshold. Term with
          coefficient absolute value below the specified threshold
          will be disabled. Disabling useless terms improve
          performance. Default threshold is 10^-10. */
      inline void set_coefficients_threshold(double t);

      /** This functions must be used to enable/disable a zernike
          term without changing its coefficient */
      void set_term_state(unsigned int n, bool enabled);

      /** Get current term enable state */
      bool get_term_state(unsigned int n);

      /** Compute all zernike coefficient to best fit the given
          curve. RMS difference is returned. The specified
          distribution is used to choose sampling points on
          curve. Terms state is adjusted according to current
          threshold */
      double fit(const Base &c, const trace::Distribution & d = default_dist);

      double sagitta(const math::Vector2 & xy) const;
      void derivative(const math::Vector2 & xy, math::Vector2 & dxdy) const;

      /** Evaluate zernike polynomial n */
      static double zernike_poly(unsigned int n, const math::Vector2 & xy);
      /** Evaluate x and y derivatives of zernike polynomial n */
      static void zernike_poly_d(unsigned int n, const math::Vector2 & xy, math::Vector2 & dxdy);
    private:

      void update_threshold_state();

      double _scale;
      double _threshold;
      double _radius;
      double _coeff[term_count];
      unsigned int _enabled_count;
      unsigned char _enabled_list[term_count];
    };

  }
}

#endif

