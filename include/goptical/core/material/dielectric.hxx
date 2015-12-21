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


#ifndef GOPTICAL_MATERIAL_DIELECTRIC_HXX_
#define GOPTICAL_MATERIAL_DIELECTRIC_HXX_

#include <cassert>

#include "goptical/core/material/solid.hxx"
#include "goptical/core/data/discrete_set.hxx"

namespace _goptical {

  namespace material {

    void Dielectric::clear_internal_transmittance()
    {
      _transmittance.clear();
    }

    data::DiscreteSet & Dielectric::get_transmittance_dataset()
    {
      return _transmittance;
    }

    void Dielectric::set_temperature_schott(double d0, double d1, double d2,
                                                double e0, double e1, double wl_tk)
    {
      _temp_model = ThermalSchott;
      _temp_d0 = d0;
      _temp_d1 = d1;
      _temp_d2 = d2;
      _temp_e0 = e0;
      _temp_e1 = e1;
      _temp_wl_tk = wl_tk;
    }

    void Dielectric::set_temperature_dndt(double dndt)
    {
      _temp_model = ThermalDnDt;
      _temp_d0 = dndt;
    }

    void Dielectric::disable_temperature_coeff()
    {
      _temp_model = ThermalNone;
    }

    void Dielectric::set_measurement_medium(const const_ref<Base> &medium)
    {
      assert(medium.ptr() != this);
      _measurement_medium = medium;
    }

    void Dielectric::set_wavelen_range(double low, double high)
    {
      _low_wavelen = low;
      _high_wavelen = high;
    }

  }
}

#endif

