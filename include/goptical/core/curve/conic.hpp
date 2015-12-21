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


#ifndef GOPTICAL_CURVE_CONIC_HH_
#define GOPTICAL_CURVE_CONIC_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/curve/conic_base.hpp"

namespace _goptical {

  namespace curve {

    /**
       @short General purpose conic curve model
       @header <goptical/core/curve/Conic
       @module {Core}
       @main

       This class models a rotationally symmetric conic curves with
       given radius of curvature and deformation coefficient. The
       later can be provided either as Schwarzschild constant or
       Eccentricity value.

       Fitting can be used to find best fit conic of an other
       rotationally symmetric curve either with fixed or free
       deformation parameter.

       @ref Sphere and @ref Parabola offer optimized implementations
       for common special cases.
     */
    class Conic : public ConicBase
    {
    public:
      /** Creates a conic curve with given radius of curvature and
          Schwarzschild constant */
      Conic(double roc, double sc);

      /** Set Schwarzschild constant */
      inline void set_schwarzschild(double sc);

      /** Set eccentricity */
      inline void set_eccentricity(double e);

      /** Adjust radius of curvature _and_ deformation to best fit given curve

          @param curve curve to fit
          @param radius Maximum radius used to get sample points
          @param count Number of sample points to use
      */
      double fit(const Rotational &curve, double radius, unsigned int count);

      bool intersect(math::Vector3 &point, const math::VectorPair3 &ray) const;
      double sagitta(double r) const;
      double derivative(double r) const;

    };
  }

}

#endif

