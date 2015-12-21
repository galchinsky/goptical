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

#include <goptical/core/material/Proxy>
#include <goptical/core/io/Rgb>

namespace _goptical {

  namespace material {

    Proxy::Proxy(const const_ref<Base> &m)
      : _m(m)
    {
    }

    bool Proxy::is_opaque() const
    {
      return _m->is_opaque();
    }

    bool Proxy::is_reflecting() const
    {
      return _m->is_reflecting();
    }

    double Proxy::get_internal_transmittance(double wavelen, double thickness) const
    {
      return _m->get_internal_transmittance(wavelen, thickness);
    }

    double Proxy::get_refractive_index(double wavelen) const
    {
      return _m->get_refractive_index(wavelen);
    }

    double Proxy::get_extinction_coef(double wavelen) const
    {
      return _m->get_extinction_coef(wavelen);
    }

    double Proxy::get_normal_reflectance(const Base *from, double wavelen) const
    {
      return _m->get_normal_reflectance(from, wavelen);
    }

    double Proxy::get_normal_transmittance(const Base *from, double wavelen) const
    {
      return _m->get_normal_transmittance(from, wavelen);
    }

    double Proxy::get_temperature() const
    {
      return _m->get_temperature();
    }

    io::Rgb Proxy::get_color() const
    {
      return _m->get_color();
    }

  }
}

