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


#ifndef GOPTICAL_MATERIAL_AIR_HH_
#define GOPTICAL_MATERIAL_AIR_HH_

#include "goptical/core/common.hpp"

#include "base.hpp"

namespace _goptical {

  namespace material {

    /**
       @short Air optical material model
       @header <goptical/core/material/Air
       @module {Core}
       @main

       This class models optical properties of air. Refractive index
       of air depends on temperature and pressure.

       This class provides different formulas:
       @list
         @item @ref AirBirch94 : described in "@em{Birch, Metrologia, 1994, 31, 315}".
         @item @ref AirKohlrausch68 : described in "@em{F. Kohlrausch, Praktische Physik, 1968, 1, 408}".
       @end list

       Global variables @ref air and @ref std_air are available with
       default parameters and Birch model.
     */

    template <enum AirFormula m = AirBirch94Formula>
    class Air : public Base
    {
    public:

      /** Standard air pressure is 101325 @em Pa */
      static const double std_pressure;

      /** Create a new air material model with 101325 Pa pressure. */
      Air(double pressure = std_pressure);

      /** @override */
      bool is_opaque() const;
      /** @override */
      bool is_reflecting() const;

      /** @override */
      double get_internal_transmittance(double wavelen, double thickness) const;
      /** @override */
      double get_refractive_index(double wavelen) const;
      /** @override */
      double get_extinction_coef(double wavelen) const;

      GOPTICAL_ACCESSORS(double, pressure, "relative air pressure in @em Pa @see std_pressure")

    private:
      double _pressure;
    };

    /** A global read only instance of @ref AirBirch94 material with standard parameters. */
    extern const AirBirch94 std_air;

    /** A global instance of @ref AirBirch94 material. */
    extern AirBirch94 air;

  }
}

#endif

