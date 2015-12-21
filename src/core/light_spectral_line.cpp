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


#include <goptical/core/io/Rgb>
#include <goptical/core/light/SpectralLine>

namespace _goptical {

  namespace light {

    SpectralLine::SpectralLine(double wavelen, double intensity)
      : _wavelen(wavelen), _intensity(intensity)
    {
    }

    io::Rgb SpectralLine::get_wavelen_color(double wl)
    { 
      // based on algorithm from Dan Bruton (www.physics.sfasu.edu/astro/color.html)

      if (wl < 380.0 || wl > 780.0)
        return io::rgb_black;

      float s = 1.0;

      if (wl < 420.0)
        s = 0.3 + 0.7 * (wl - 380.0) / 40.0;
      else if (wl > 700.0)
        s = 0.3 + 0.7 * (780.0 - wl) / 80.0;

      if (wl < 510.0)
        {
          if (wl < 490.0)
            {
              if (wl < 440.0)
                // 380 to 440
                return io::Rgb(s * -(wl - 440.0) / 60.0, 0.0, s);
              else
                // 440 to 490
                return io::Rgb(0.0, s * (wl - 440.0) / 50.0, s);
            }
          else
            // 490 to 510
            return io::Rgb(0.0, s, s * -(wl - 510.0) / 20.0);
        }
      else
        {
          if (wl < 645.0)
            {
              if (wl < 580.0)
                // 510 to 580
                return io::Rgb(s * (wl - 510.0) / 70.0, s, 0.0);
              else
                // 580 to 645
                return io::Rgb(s, s * -(wl - 645.0) / 65.0, 0.0);
            }
          else
            {
              // 645 to 780
              return io::Rgb(s, 0.0, 0.0);
            }
        }
    }

    const double SpectralLine::red = 645.0;
    const double SpectralLine::green = 510.0;
    const double SpectralLine::blue = 440.0;
    const double SpectralLine::t = 1013.98;
    const double SpectralLine::s = 852.11;
    const double SpectralLine::r = 706.5188;
    const double SpectralLine::C = 656.2725;
    const double SpectralLine::C_ = 643.8469; // C'
    const double SpectralLine::D = 589.2938;
    const double SpectralLine::d = 587.5618;
    const double SpectralLine::e = 546.074;
    const double SpectralLine::F = 486.1327;
    const double SpectralLine::F_ = 479.9914; // F'
    const double SpectralLine::g = 435.8343;
    const double SpectralLine::h = 404.6561;
    const double SpectralLine::i = 365.0146;
  }

}

