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


#ifndef GOPTICAL_SPECTRALLINE_HXX_
#define GOPTICAL_SPECTRALLINE_HXX_

#include "goptical/core/error.hpp"

namespace _goptical {

  namespace light {

    double SpectralLine::get_wavelen() const
    {
      return _wavelen;
    }

    double SpectralLine::get_intensity() const
    {
      return _intensity;
    }

    io::Rgb SpectralLine::get_color() const
    {
      return get_wavelen_color(_wavelen);
    }

  }
}

#endif

