/*

      This file is part of the <goptical/core Design library.
  
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


#ifndef GOPTICAL_DESIGN_TELESCOPE_HH_
#define GOPTICAL_DESIGN_TELESCOPE_HH_

#include <goptical/core/common.hpp>
#include <goptical/design/common.hpp>

#include <goptical/core/sys/group.hpp>

namespace _goptical {

  namespace Design {

    namespace telescope {

    /**
       @short Base class for telescope optical designs
       @header <goptical/core/Design/telescope/telescope
       @module {Design}
    */
    class telescope : public sys::Group
    {
    public:
      inline telescope(const math::VectorPair3 &p);

      /** Set telescope focal length */
      virtual void set_focal(double focal) = 0;
      /** Get telescope focal length */
      virtual double get_focal() const = 0;

      /** Set telescope primary mirror diameter */
      virtual void set_diameter(double diameter) = 0;
      /** Get telescope primary mirror diameter */
      virtual double get_diameter() const = 0;

      /** Set telescope back working distance */
      virtual void set_bwd(double bwd) = 0;
      /** Get telescope back working distance */
      virtual double get_bwd() const = 0;

      /** Set telescope field angle */
      virtual void set_field_angle(double bwd) = 0;
      /** Get telescope field angle */
      virtual double get_field_angle() const = 0;

      /** Get diameter of unvignetted image on focal plane */
      virtual double get_unvignetted_image_diameter() const = 0;

      /** Get telescope focal plane in parent coordinate system */
      virtual math::VectorPair3 get_focal_plane() const = 0;
    };

    }
  }
}

#endif

