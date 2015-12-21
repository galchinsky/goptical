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


#ifndef GOPTICAL_MATERIAL_SELLMEIER_HH_
#define GOPTICAL_MATERIAL_SELLMEIER_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/material/dielectric.hpp"

namespace _goptical {

  namespace material {

    /**
       @short Sellmeier model for optical glass material
       @header <goptical/core/material/Sellmeier
       @module {Core}
       @main

       This class models optical properties of dielectric
       materials using @url http://en.wikipedia.org/wiki/Sellmeier_equation
       {Sellmeier} refractive index dispersion formula:

       @math $ n_\lambda = \sqrt{ A + \sum\limits_{i}^{} \frac{B_i \times \lambda^2}{\lambda^2 - C_i}} $

       with @math $\lambda$ the micrometer wavelength.

       @see SellmeierMod
     */

    class Sellmeier : public Dielectric
    {
    public:

      /** Create an empty sellmeier model */
      Sellmeier();

      /** Create an 3rd order sellmeier model with given coefficients
          and 1.0 constant */
      Sellmeier(double K1, double L1,
                double K2, double L2,
                double K3, double L3);

      /** Set terms count */
      void set_terms_count(unsigned int c);

      /** Change sellmeier constant term */
      inline void set_contant_term(double A);

      /** Set term coefficients */
      inline void set_term(unsigned int i, double B, double C);

      /** @override */
      double get_measurement_index(double wavelen) const;
    private:

      std::vector<double> _coeff;
      double _constant;
    };

  }
}

#endif

