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


#include <goptical/core/data/Set>
#include <goptical/core/material/Dielectric>
#include <goptical/core/material/Air>

namespace _goptical {

  namespace material {

    Dielectric::Dielectric()
      : Solid("dielectric"),
        _transmittance(),
        _temp_model(ThermalNone),
        _low_wavelen(350.0),
        _high_wavelen(750.0),
        _measurement_medium(std_air)
    {
      _transmittance.set_interpolation(data::Cubic);
    }

    bool Dielectric::is_opaque() const
    {
      return false;
    }

    bool Dielectric::is_reflecting() const
    {
      return false;
    }

    double Dielectric::get_internal_transmittance(double wavelen, double thickness) const
    {
      double t = _transmittance.interpolate(wavelen);

      return pow(t, thickness);
    }

    void Dielectric::set_internal_transmittance(double wavelen,
                                                double thickness,
                                                double transmittance)
    {
      _transmittance.add_data(wavelen, pow(transmittance, 1.0 / thickness));
    }

    double Dielectric::get_internal_transmittance(double wavelen) const
    {
      try {
        return _transmittance.interpolate(wavelen);
      } catch (...) {
        return 1.0;
      }
    }

    double Dielectric::get_schott_temp(double wavelen, double n) const
    {
      // SCHOTT TIE-19: Temperature Coefficient of the Refractive Index

      double dt = _temperature - _measurement_medium->get_temperature();
      double wl = wavelen / 1000.;
      double wl_tk = _temp_wl_tk;

      return (n*n - 1.) / (2*n) * dt * (_temp_d0 + _temp_d1*dt + _temp_d2*dt*dt
                                        + (_temp_e0 + _temp_e1*dt)/(wl*wl - wl_tk*wl_tk));
    }

    double Dielectric::get_refractive_index(double wavelen) const
    {
      double a = _measurement_medium->get_refractive_index(wavelen);
      double m = get_measurement_index(wavelen);

      // get absolute refractive index
      double n = m * a;

      // apply temperature coefficients
      switch(_temp_model)
        {
        case ThermalSchott:
          n = n + get_schott_temp(wavelen, m);
          break;

        case ThermalDnDt: {
          double dt = _temperature - _measurement_medium->get_temperature();
          n = n + dt * _temp_d0;
          break;
        }

        case ThermalNone:
          ;
        }

      return n;
    }

    double Dielectric::get_principal_dispersion() const
    {
      return get_measurement_index(light::SpectralLine::F)
           - get_measurement_index(light::SpectralLine::C);
    }

    double Dielectric::get_abbe_vd() const
    {
      return (get_measurement_index(light::SpectralLine::d) - 1.0)
        / (get_measurement_index(light::SpectralLine::F)
           - get_measurement_index(light::SpectralLine::C));
    }

    double Dielectric::get_abbe_ve() const
    {
      return (get_measurement_index(light::SpectralLine::e) - 1.0)
        / (get_measurement_index(light::SpectralLine::F_)
           - get_measurement_index(light::SpectralLine::C_));
    }

    double Dielectric::get_partial_dispersion(double wavelen1, double wavelen2) const
    {
      return (get_measurement_index(wavelen1)
              - get_measurement_index(wavelen2))
        / get_principal_dispersion();
    }

  }

}

