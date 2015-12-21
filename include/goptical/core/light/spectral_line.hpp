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


#ifndef GOPTICAL_SPECTRALLINE_HH_
#define GOPTICAL_SPECTRALLINE_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/io/rgb.hpp"

namespace _goptical {

  namespace light {

    /**
       @short Describe a spectral line
       @header <goptical/core/light/SpectralLine
       @module {Core}

       This class can be used to describe a spectral line. It contains
       constants for wave length of standard rays
    */

    class SpectralLine
    {
    public:
      /** Create a spectral line with specified wavelen and
          intensity. */
      SpectralLine(double wavelen, double intensity = 1.0);

      /** get spectral line wavelen */
      inline double get_wavelen() const;
      /** get spectral line intensity */
      inline double get_intensity() const;
      /** get rgb color associated spectral line, ignore intensity */
      inline io::Rgb get_color() const;
      /** get rgb color associated with wavelen */
      static io::Rgb get_wavelen_color(double wl);

      /** red colored line at 645nm */
      static const double red;
      /** green colored line at 510nm */
      static const double green;
      /** blue colored line at 440nm */
      static const double blue;

      /** infrared mercury line at 1013.98nm */
      static const double t;
      /** infrared cesium line at 852.11nm */
      static const double s;
      /** red helium line at 706.5188nm */
      static const double r;
      /** red hydrogen line at 656.2725nm */
      static const double C;
      /** red cadmium line at 643.8469nm */
      static const double C_; // C'
      /** yellow sodium line at 589.2938nm */
      static const double D;
      /** yellow helium line at 587.5618nm */
      static const double d;
      /** green mercury line at 546.074nm */
      static const double e;
      /** blue hydrogen line at 486.1327nm */
      static const double F;
      /** blue cadmium line at 479.9914nm */
      static const double F_; // F'
      /** blue mercury line at 435.8343nm */
      static const double g;
      /** violet mercury line at 404.6561nm */
      static const double h;
      /** ultraviolet mercury line at 365.0146nm */
      static const double i;

    private:
      double    _wavelen;
      double    _intensity;
    };
  }
}

#endif

