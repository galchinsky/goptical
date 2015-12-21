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


#ifndef GOPTICAL_MATERIAL_SELLMEIERMOD_HH_
#define GOPTICAL_MATERIAL_SELLMEIERMOD_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/material/dielectric.hpp"

namespace _goptical {

  namespace material {

    /**
       @short Modified sellmeier model for optical glass material
       @header <goptical/core/material/SellmeierMod
       @module {Core}
       @main

       This class models optical properties of dielectric
       materials with various modified Sellmeier formulas:

       @list
         @item @ref SellmeierMod2 : @math $ n_\lambda = \sqrt{A + \frac{B\times\lambda^2}{\lambda^2 - C^2} 
                                 + \frac{D}{\lambda^2 - E^2}} $

         @item @ref Handbook1 : @math $ n_\lambda = \sqrt{A + B\times\lambda^2 + \frac{C}{\lambda^2-D}} $

         @item @ref Handbook2 : @math $ n_\lambda = \sqrt{A + B\times\lambda^2 + \frac{C\times\lambda^2}{\lambda^2-D}} $
       @end list

       with @math $\lambda$ the micrometer wavelength.

       @see Sellmeier
     */

    template <enum SellmeierModFormula m>
    class SellmeierMod : public Dielectric
    {
    public:

      /** Create an empty modified sellmeier model */
      SellmeierMod();

      /** Create a modified sellmeier model with given coefficients */
      SellmeierMod(double A, double B,
                   double C, double D, double E = 0.0);

      /** Change coefficients */
      inline void set_coefficients(double A, double B,
                                   double C, double D,
                                   double E = 0.0);

      /** @override */
      double get_measurement_index(double wavelen) const;
    private:

      double _a, _b, _c, _d, _e;
    };

  }
}

#endif

