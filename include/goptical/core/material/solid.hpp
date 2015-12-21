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


#ifndef GOPTICAL_MATERIAL_SOLID_HH_
#define GOPTICAL_MATERIAL_SOLID_HH_

#include "goptical/core/common.hpp"

#include "base.hpp"

namespace _goptical {

  namespace material {

    /**
       @short Solid optical material base class.
       @header <goptical/core/material/Solid
       @module {Core}

       This class is the base class for all solid material models.
     */

    class Solid : public Base
    {
    public:
      inline Solid(const std::string& name_);

      GOPTICAL_ACCESSORS(double, thermal_expansion,
                         "material thermal expansion coefficient");

      GOPTICAL_ACCESSORS(double, thermal_conductivity,
                         "Set material thermal conductivity in W/m.K");

      GOPTICAL_ACCESSORS(double, density,
                         "material density in g/cm^3");

      GOPTICAL_ACCESSORS(double, young_modulus,
                         "material young modulus");

      GOPTICAL_ACCESSORS(double, poisson_ratio,
                         "material poisson ratio");

    private:
      double            _thermal_expansion; // thermal expansion coefficient
      double            _thermal_conductivity; // thermal conductivity W/m.K
      double            _density;     // density g/cm^3
      double            _young_modulus;
      double            _poisson_ratio;
    };

  }

}

#endif

