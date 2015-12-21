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


#ifndef GOPTICAL_SHAPE_RING_HH_
#define GOPTICAL_SHAPE_RING_HH_

#include "goptical/core/common.hpp"

#include "base.hpp"
#include "shape_round.hpp"

namespace _goptical {

  namespace shape {

    /**
       @short Ring shape base class
       @header <goptical/core/shape/Ring
       @module {Core}
       @internal
     */

    class RingBase : public Base
    {
    public:
      /** Set ring external radius and hole radius */
      inline void set_radius(double radius, double hole_radius);

      /** Get ring external radius */
      inline double get_radius() const;
      /** Get hole radius */
      inline double get_hole_radius() const;

      /** @override */
      inline double max_radius() const;
      /** @override */
      inline double min_radius() const;
      /** @override */
      double get_outter_radius(const math::Vector2 &dir) const;
      /** @override */
      double get_hole_radius(const math::Vector2 &dir) const;
      /** @override */
      math::VectorPair2 get_bounding_box() const;
      /** @override */
      bool inside(const math::Vector2 &point) const;

    protected:

      inline double get_external_xradius() const;
      inline double get_internal_xradius() const;
      inline double get_xy_ratio() const;

      double _radius, _hole_radius;
    };

    /**
       @short Ring shape
       @header <goptical/core/shape/Ring
       @module {Core}
       @main

       This is a disk shape with a hole in center.
     */

    class Ring : public Round<RingBase, true>
    {
    public:
      /** Create a new ring with given external radius and hole radius. */
      inline Ring(double radius, double hole_radius);
    };

  }
}

#endif

