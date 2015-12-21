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


#ifndef GOPTICAL_SHAPE_ELLIPTICALRING_HH_
#define GOPTICAL_SHAPE_ELLIPTICALRING_HH_

#include "goptical/core/common.hpp"

#include "base.hpp"
#include "shape_round.hpp"

namespace _goptical {

  namespace shape {

    /**
       @short Elliptical ring base class
       @header <goptical/core/shape/EllipticalRing
       @module {Core}
       @internal

       @see Ellipse
       @see Ring
     */
    class EllipticalRingBase : public Base
    {
    public:
      /** Set external ring radius along the x and y axes and hole
          radius along the x axis. */
      void set_radius(double x_radius, double y_radius, double x_hole_radius);

      /** Get external ring radius along the x axis */
      inline double get_x_radius(void) const;
      /** Get external ring radius along the y axis */
      inline double get_y_radius(void) const;

      /** Get hole radius along the x axis */
      inline double get_x_hole_radius(void) const;
      /** Get hole radius along the y axis */
      inline double get_y_hole_radius(void) const;

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

      double _xr, _yr, _xhr;
      double _xy_ratio;
      double _e2;
    };

    /**
       @short Elliptical ring shape
       @header <goptical/core/shape/EllipticalRing
       @module {Core}
       @main

       This is an ellipse shape with a hole in center.
     */
    class EllipticalRing : public Round<EllipticalRingBase, true>
    {
    public:
      /** Create a new elliptical ring with given external ring radius
          along the x and y axes and hole radius along the x axis. */
      EllipticalRing(double x_radius, double y_radius, double x_hole_radius);
    };

  }

}

#endif

