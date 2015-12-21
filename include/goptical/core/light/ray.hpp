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


#ifndef GOPTICAL_LIGHTRAY_HH_
#define GOPTICAL_LIGHTRAY_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/math/vector_pair.hpp"

namespace _goptical {

  namespace light {

    /**
       @short Describe a ray of light
       @header <goptical/core/light/Ray
       @module {Core}
       @main

       This class is used to describe a ray of light. It contains
       geometrical and physical informations about a light ray.
     */

    class Ray : public math::VectorPair3
    {
    public:

      inline Ray();

      /** Create a ray of light with specified properties */
      inline Ray(const math::VectorPair3 &ray, double intensity = 1, double wavelen = 550);

      /** Get light ray intensity */
      inline double get_intensity() const;
      /** Set light ray intensity */
      inline void set_intensity(double intensity);

      /** Set light ray wavelen */
      inline double get_wavelen() const;
      /** Get light ray wavelen */
      inline void set_wavelen(double wavelen);

    private:
      // nanometer
      double    _wavelen;
      // FIXME unit ?
      double    _intensity;
    };

  }
}

#endif

