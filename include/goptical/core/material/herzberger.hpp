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


#ifndef GOPTICAL_MATERIAL_HERZBERGER_HH_
#define GOPTICAL_MATERIAL_HERZBERGER_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/material/dielectric.hpp"

namespace _goptical {

  namespace material {

    /**
       @short Herzberger model for optical glass material
       @header <goptical/core/material/Herzberger
       @module {Core}
       @main

       This class models optical properties of dielectric
       materials with Herzberger refractive index dispersion formula:

       @math $ n_\lambda = A + B\times\lambda^2 + C\times\lambda^4 + D\times\lambda^6
               + \frac{E}{\lambda^2-0.028} + \frac{F}{\left(\lambda^2-0.028\right)^2} $

       with @math $\lambda$ the micrometer wavelength.
     */

    class Herzberger : public Dielectric
    {
    public:

      /** Create an empty herzberger model */
      Herzberger();

      /** Create an herzberger model with given coefficients */
      Herzberger(double A, double B, double C,
                 double D, double E, double F);

      /** Change herzberger constant term */
      inline void set_coefficients(double A, double B, double C,
                                   double D, double E, double F);

      /** @override */
      double get_measurement_index(double wavelen) const;
    private:

      double _a, _b, _c, _d, _e, _f;
    };

  }
}

#endif

