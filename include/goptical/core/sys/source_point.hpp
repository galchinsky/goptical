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


#ifndef GOPTICAL_SOURCE_POINT_HH_
#define GOPTICAL_SOURCE_POINT_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/sys/source.hpp"

namespace _goptical {

  namespace sys {

      /**
         @short Point light source
         @header <goptical/core/sys/SourcePoint
         @module {Core}
         @main

         This class implement a point light source. The source can
         either be infinitely far away with a direction vector, or
         located at a given position but without direction.

         A ray is generated for each defined spectrum line for each
         distribution pattern point on target surface.

         Default wavelen list contains a single 550nm entry.
      */

    class SourcePoint : public Source
    {
    public:
      /** Create a point source with given mode. A direction vector
          must be provided when source is in infinity mode. If not in
          infinity mode, a position vector must be provided
          instead. */
      SourcePoint(SourceInfinityMode m, const math::Vector3 &pos_dir);

      /** Set point source mode to infinity and adjust source direction vector */
      void set_infinity_direction(const math::Vector3 &dir);

      /** Set point source mode to positioned and adjust source position vector */
      void set_position(const math::Vector3 &pos);

      /** Change current point source infinity mode */
      inline void set_mode(SourceInfinityMode mode);

    private:

      void generate_rays_simple(trace::Result &result,
                                const targets_t &entry) const;
      void generate_rays_intensity(trace::Result &result,
                                   const targets_t &entry) const;

      template <SourceInfinityMode mode>
      inline void get_lightrays_(trace::Result &result,
                                 const Element &target) const;

      SourceInfinityMode _mode;
    };

  }
}

#endif

