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


#ifndef GOPTICAL_MATERIAL_SCHOTT_HH_
#define GOPTICAL_MATERIAL_SCHOTT_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/material/dielectric.hpp"

namespace _goptical {

  namespace material {

    /**
       @short Schott model for optical glass material
       @header <goptical/core/material/Schott
       @module {Core}
       @main

       This class models optical properties of dielectric
       materials with Schott refractive index dispersion formula:

       @math $ n_\lambda = \sqrt{ \sum\limits_{i} A_{i}\times\lambda^{2i}} $

       with @math $\lambda$ the micrometer wavelength.
     */

    class Schott : public Dielectric
    {
    public:

      /** Create an empty schott model */
      Schott();

      /** Create a schott model with [-8,2] exponents range:

          @math $ n_\lambda = \sqrt{ A + B\times\lambda^2 + \frac{C}{\lambda^2}
                  + \frac{D}{\lambda^4} + \frac{E}{\lambda^6} + \frac{F}{\lambda^8}} $
      */
      Schott(double A, double B,
             double C, double D,
             double E, double F);

      /** Set exponents range, must be multiples of 2 */
      void set_terms_range(int first, int last);

      /** Set term coefficient, @tt term must be a multiple of 2. */
      inline void set_term(int term, double K);

    private:
      double get_measurement_index(double wavelen) const;

      std::vector<double> _coeff;
      int _first;
    };

  }
}

#endif

