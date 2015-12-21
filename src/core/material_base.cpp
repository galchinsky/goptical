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

#include <iostream>
#include <goptical/core/material/Base>
#include <goptical/core/io/Rgb>
 
namespace _goptical {

  namespace material {

    Base::Base()
      : _temperature(20.0)
    {
        std::cout << "material created" << std::endl;
    }

    Base::Base(const std::string& name_ )
      : _temperature(20.0),
        name(name_)
    {
        std::cout << "material created " << name << std::endl;
    }

    Base::~Base()
    {
    }

    // compute extinction coefficient from internal transmittance
    double Base::get_extinction_coef(double wavelen) const
    {
      // Beer-Lambert law
      // FIXME check this formula
      return - (log(get_internal_transmittance(wavelen, 1.0)) * (wavelen * 1e-9f)) / (4 * M_PI * 0.001/* 1 mm */);
    }

    // compute internal transmittance from extinction coefficient
    double Base::get_internal_transmittance(double wavelen, double thickness) const
    {
      // Beer-Lambert law

      // FIXME simplify and check
      double tr = exp(-(4 * M_PI * get_extinction_coef(wavelen) * 0.001/* 1 mm */) / (wavelen * 1e-9f));

      return pow(tr, thickness);
    }

    // compute internal transmittance from extinction coefficient
    double Base::get_internal_transmittance(double wavelen) const
    {
      return get_internal_transmittance(wavelen, 1.0);
    }

    // default reflectance at normal incidence, valid for metal and dielectric material
    double Base::get_normal_reflectance(const Base *from, double wavelen) const
    {
      // McGraw Hill, Handbook of optics, vol1, 1995, 5-10 (47)
      
      double n0 = from->get_refractive_index(wavelen);
      double k12 = math::square(get_extinction_coef(wavelen));
      double n1 = get_refractive_index(wavelen);
      double res = (math::square(n0 - n1) + k12) / (math::square(n0 + n1) + k12);
      
      return res;
    }

    // default transmittance at normal incidence, valid for non absorbing material
    double Base::get_normal_transmittance(const Base *from, double wavelen) const
    {
      // McGraw Hill, Handbook of optics, vol1, 1995, 5-8 (23)

      double n0 = from->get_refractive_index(wavelen);
      double n1 = get_refractive_index(wavelen);

      return (4.0 * n0 * n1) / math::square(n0 + n1);
    }

    io::Rgb Base::get_color() const
    {
      // FIXME color depends on material properties
      return io::Rgb(1, 1, 1, 1);
    }

    void Base::set_temperature(double temp)
    {
      _temperature = temp;
    }

    double Base::get_temperature() const
    {
      return _temperature;
    }

  }
}

