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


#ifndef GOPTICAL_SHAPE_DISK_HH_
#define GOPTICAL_SHAPE_DISK_HH_

#include "goptical/core/common.hpp"

#include "base.hpp"
#include "shape_round.hpp"

namespace _goptical {

  namespace shape {

    /**
       @short Disk shape base class
       @header <goptical/core/shape/Disk
       @module {Core}
       @internal
     */

    class DiskBase : public Base
    {
    public:
      /** Set disk radius */
      inline void set_radius(double r);
      /** Get disk radius */
      inline double get_radius(void) const;

    protected:

      /** @override */
      inline double max_radius() const;
      /** @override */
      inline double min_radius() const;
      /** @override */
      double get_outter_radius(const math::Vector2 &dir) const;
      /** @override */
      math::VectorPair2 get_bounding_box() const;
      /** @override */
      bool inside(const math::Vector2 &point) const;

      inline double get_external_xradius() const;
      inline double get_internal_xradius() const;
      inline double get_xy_ratio() const;

      double _radius;
    };

    /**
       @short Disk shape
       @header <goptical/core/shape/Disk
       @module {Core}
       @main

       This is the most common lens shape.
     */

    class Disk : public Round<DiskBase, false>
    {
    public:
      /** Create a new disk with given radius */
      inline Disk(double radius);
    };

  }
}

#endif

