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


#ifndef GOPTICAL_CURVE_GRID_HH_
#define GOPTICAL_CURVE_GRID_HH_

#include "goptical/core/common.hpp"

#include "base.hpp"
#include "goptical/core/data/grid.hpp"

namespace _goptical {

  namespace curve {

    /**
       @short Define surface curve from interpolated grid data set.
       @header <goptical/core/curve/Grid
       @module {Core}
       @main

       This class use the @ref data::Grid 2d data set container to
       define a 3d non-symmetric curve. Several data interpolation
       algorithms are available allowing use of @ref data::Bicubic spline
       interpolation with or without prescribed derivative/gradient
       data. @see data::Grid .

       The @ref Spline curve model is preferred when dealing
       with @ref Rotational {rotationally symmetric curves}.
    */
    class Grid : public Base
    {
    public:
      /** Creates a grid curve with n*n sample points defined over the
          given radius. */
      Grid(unsigned int n, double radius);

      ~Grid();

      /** Get embedded sagitta/gradient data container */
      inline const data::Grid &get_data() const;

      /** Get embedded sagitta/gradient data container */
      inline data::Grid &get_data();

      /** Set grid values to best fit an other curve. Gradient data
          will be extracted only if the interpolation have been set to
          data::BicubicDeriv previously */
      void fit(const Base &c);

      double sagitta(const math::Vector2 & xy) const;
      void derivative(const math::Vector2 & xy, math::Vector2 & dxdy) const;

    protected:
      data::Grid _data;
    };

  }

}

#endif
