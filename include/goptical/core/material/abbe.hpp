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
  
      Copyright (C) 2011 Free Software Foundation, Inc
      Author: Alexandre Becoulet

*/


#ifndef GOPTICAL_MATERIAL_ABBE_HH_
#define GOPTICAL_MATERIAL_ABBE_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/material/dielectric.hpp"

namespace _goptical {

  namespace material {

    /**
       @short Abbe model for optical glass material
       @header <goptical/core/material/Abbe
       @module {Core}
       @main

       This class models optical properties of glass materials
       using known refractive index value,
       @url http://en.wikipedia.org/wiki/Abbe_number {Abbe number}
       and partial dispersion deviation (dpgF).

       @ref AbbeVd and @ref AbbeVe template instances are available for
       @em d and @em e line definitions of Abbe number.

       The following formula is used to determine refractive index at
       any wavelen in visible spectrum:

       @math $ n_\lambda = n_d + \frac{\left( n_d-1\right)}{v_d}\times\left( c_1+a\times c_0 +
               \frac{c_3+a\times c_2}{\lambda} + \frac{c_5+a\times c_4}{{\lambda}^{2}} +
               \frac{c_7+a\times c_6}{{\lambda}^{3}} \right) $
               
       with @math $ a = a_1\times v_d+a_0+\Delta P_{g,F} $
       and @math $\lambda$ the micrometer wavelength.

       @em a0 and @em a1 values are given in Schott "@em{TIE-29:
       Refractive Index and Dispersion}" technical information document.

       @em c0 to @em c7 values were determined using least square
       fitting on indexes obtained using Sellmeier data from 118 glass
       materials from the Schott catalog for 360 wavelengths between
       390nm and 750nm. Two different coefficients sets are used for
       @em nd/vd and @em ne/ve pairs.

       Mean error is less than 0.00002 from Sellmeier indexes and
       largest error found across fitted glasses and wavelengths is
       0.0008. When dpgF is set to 0, mean error becomes 0.0001 and
       largest error is close to 0.006.
     */

    template <enum AbbeFormula m = AbbeVdFormula>
    class Abbe : public Dielectric
    {
    public:

      /** Create an abbe glass model */
      Abbe(double n, double v, double dpgF = 0.);

      /** @override */
      double get_measurement_index(double wavelen) const;
    private:

      double _n, _q, _a;
    };

  }
}

#endif

