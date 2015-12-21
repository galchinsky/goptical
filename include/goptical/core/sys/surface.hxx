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


#ifndef GOPTICAL_SURFACE_HXX_
#define GOPTICAL_SURFACE_HXX_

#include <cassert>

#include "goptical/core/sys/element.hxx"
#include "goptical/core/shape/base.hxx"
#include "goptical/core/curve/base.hxx"

namespace _goptical {

  namespace sys {

    void Surface::set_curve(const const_ref<curve::Base> &c)
    {
      _curve = c;
    }

    const curve::Base & Surface::get_curve() const
    {
      return *_curve;
    }

    void Surface::set_shape(const const_ref<shape::Base> &s)
    {
      _shape = s;
    }

    const shape::Base & Surface::get_shape() const
    {
      return *_shape;
    }

    void Surface::set_discard_intensity(double intensity)
    {
      _discard_intensity = intensity;
    }

    inline double Surface::get_discard_intensity() const
    {
      return _discard_intensity;
    }

  }
}

#endif

