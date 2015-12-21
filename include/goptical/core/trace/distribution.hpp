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

#ifndef GOPTICAL_TRACE_DISTRIBUTION_HH_
#define GOPTICAL_TRACE_DISTRIBUTION_HH_

#include "goptical/core/common.hpp"

namespace _goptical
{

  namespace trace
  {

    /**
       @short Ray distribution pattern descriptor
       @header <goptical/core/trace/Distribution
       @module {Core}
       @main

       This class describes distribution pattern and ray density used
       for light ray distribution over surfaces during light
       propagation.

       Ray density is expressed as average number of rays along
       surface radius.

       @image dist_patterns.png {Different patterns rendered on a disk with default density}
     */

    class Distribution
    {
    public:
      /** Creates a distribution pattern with specified pattern,
          radial ray density and scaling.

          The scaling ratio parameter may be used to avoid
          distributing rays too close to the surface edge. */
      inline Distribution(Pattern pattern = DefaultDist,
                          unsigned int radial_density = 5,
                          double scaling = 0.999);

      /** Set distribution pattern */
      inline void set_pattern(Pattern p);

      /** Get distribution pattern */
      inline Pattern get_pattern() const;

      /** Set average number of ray to distribute on surface radius */
      inline void set_radial_density(unsigned int count);

      /** Get current radial density */
      inline unsigned int get_radial_density() const;

      /** Set scaling, Rays are distributed on surface but are
          kept away from surface edge if scaling is less than
          1.0. */
      inline void set_scaling(double scaling);

      /** get current scaling */
      inline double get_scaling() const;

      /** Ensure uniform 2d pattern suitable for spot diagram and
          image analysis is selected. Change to default pattern if
          not. */
      inline void set_uniform_pattern();

    private:
      Pattern           _pattern;
      unsigned int      _radial_density;
      double            _scaling;
    };
  }
}

#endif

