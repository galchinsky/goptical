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


#ifndef GOPTICAL_RGB_HXX_
#define GOPTICAL_RGB_HXX_

namespace _goptical {

  namespace io {

    Rgb::Rgb()
    {
    }

    Rgb::Rgb(float red, float green, float blue, float alpha)
        : r(red), g(green), b(blue), a(alpha)
    {
    }

    bool Rgb::operator< (const Rgb &rgb) const
    {
      return (a * 1000.0 + r * 100.0 + g * 10.0 + b <
              rgb.a * 1000.0 + rgb.r * 100.0 + rgb.g * 10.0 + rgb.b);
    }

    Rgb Rgb::operator~ () const
    {
      return Rgb(1. - r, 1. - g, 1. - b, a);
    }

  }
}

#endif

