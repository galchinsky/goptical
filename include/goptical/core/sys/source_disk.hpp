/*

      This file is part of the Goptical Core library.
  
      The Goptical library is free software; you can redistribute it
      and/or modify it under the terms of the GNU General Public
      License as published by the Free Software Foundation; either
      version 3 of the License, or (at your option) any later version.
  
      The Goptical library is distributed in the hope that it will be
      useful, but WITHOUT ANY WARRANTY; without even the implied
      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
      See the GNU General Public License for more details.
  
      You should have received a copy of the GNU General Public
      License along with the Goptical library; if not, write to the
      Free Software Foundation, Inc., 59 Temple Place, Suite 330,
      Boston, MA 02111-1307 USA
  
      Copyright (C) 2010-2011 Free Software Foundation, Inc
      Author: Dmitry Galchinsky

*/


#ifndef GOPTICAL_SOURCE_DISK_HH_
#define GOPTICAL_SOURCE_DISK_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/sys/source.hpp"

namespace _goptical {

  namespace sys {

      /**
         @short Disk light source
         @header SourceDisk
         @module {Core}
         @main

      */
      
    class SourceDisk : public Source
    {
    public:
        /** Create a disk source with given mode. A direction vector
            must be provided when source is in infinity mode. If not in
            infinity mode, a position vector must be provided
            instead. */
        SourceDisk(SourceInfinityMode m, 
                   const math::Vector3& pos_dir, 
                   const math::Vector2& size,
                   const math::Vector2& limit1 = math::Vector2(-math::Inf, -math::Inf),
                   const math::Vector2& limit2 = math::Vector2(math::Inf, math::Inf));
        
        /** Set point source mode to infinity and adjust source direction vector */
        void set_infinity_direction(const math::Vector3 &dir);
        
        /** Set point source mode to positioned and adjust source position vector */
        void set_position(const math::Vector3 &pos);
        
        /** Change current point source infinity mode */
        inline void set_mode(SourceInfinityMode mode);

        void set_limits(const math::Vector2& limit1,
                        const math::Vector2& limit2);

        
    private:
        
        void generate_rays_simple(trace::Result &result,
                                  const targets_t &entry) const;
        void generate_rays_intensity(trace::Result &result,
                                     const targets_t &entry) const;
        
        template <SourceInfinityMode mode>
        inline void get_lightrays_(trace::Result &result,
                                   const Element &target) const;
        
        SourceInfinityMode _mode;
        math::Vector2 _size;
        math::Vector3 _direction;
        math::Vector2 _limit1;
        math::Vector2 _limit2;

    };
      
  }
}

#endif

