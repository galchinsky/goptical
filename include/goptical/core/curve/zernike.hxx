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


#ifndef GOPTICAL_CURVE_ZERNIKE_HXX_
#define GOPTICAL_CURVE_ZERNIKE_HXX_

#include <cassert>

#include "base.hxx"

namespace _goptical {

  namespace curve {

    void Zernike::set_radius(double radius)
    {
      _radius = radius;
    }

    double Zernike::get_radius() const
    {
      return _radius;
    }

    void Zernike::set_coefficient(unsigned int n, double c)
    {
      assert(n < term_count);
      _coeff[n] = c;
      set_term_state(n, fabs(c) >= _threshold);
    }

    double Zernike::get_coefficient(unsigned int n) const
    {
      assert(n < term_count);
      return _coeff[n];
    }

    void Zernike::set_coefficients_threshold(double t)
    {
      _threshold = t;
      update_threshold_state();
    }

    void Zernike::set_coefficients_scale(double s)
    {
      _scale = s;
    }

  }
}

#endif

