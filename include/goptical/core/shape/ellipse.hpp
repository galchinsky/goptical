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


#ifndef GOPTICAL_SHAPE_ELLIPSE_HH_
#define GOPTICAL_SHAPE_ELLIPSE_HH_

#include "goptical/core/common.hpp"

#include "base.hpp"
#include "shape_round.hpp"

namespace _goptical {

  namespace shape {

    /**
       @short Ellipse shape base class
       @header <goptical/core/shape/Ellipse
       @module {Core}
       @internal

       @see EllipticalRing
     */

    class EllipseBase : public Base
    {
    public:
      /** Set ellipse radius along x and y axis */
      void set_radius(double xr, double yr);
      /** Set ellipse radius along x axis */
      inline double get_x_radius(void) const;
      /** Set ellipse radius along y axis */
      inline double get_y_radius(void) const;

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

    protected:

      inline double get_external_xradius() const;
      inline double get_internal_xradius() const;
      inline double get_xy_ratio() const;

      double _xr, _yr;
      double _xy_ratio;
      double _e2;
    };

    /**
       @short Ellipse shape
       @header <goptical/core/shape/Ellipse
       @module {Core}
       @main
     */
    class Ellipse : public Round<EllipseBase, false>
    {
    public:
      /** Create a new ellipse with given radius */
      Ellipse(double x_radius, double y_radius);
    };

  }

}

#endif

