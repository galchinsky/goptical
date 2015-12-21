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


#ifndef GOPTICAL_CURVE_SPLINE_HH_
#define GOPTICAL_CURVE_SPLINE_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/curve/rotational.hpp"
#include "goptical/core/data/discrete_set.hpp"

namespace _goptical {

  namespace curve {

    /**
       @short Rotationally symmetric spline curve implementation
       @header <goptical/core/curve/Spline
       @module {Core}
       @main

       This class uses a 1d discrete data set to define a rotationally
       symmetric sagitta/gradient curve. Several data interpolations
       algorithm are available allowing use of Smooth Cubic spline
       interpolation with or without prescribed derivative/gradient
       data.

       @see data::DiscreteSet
    */
    class Spline : public Rotational
    {
    public:
      /** Create an empty spline curve with no defined point */
      Spline();
      ~Spline();

      /** Get sagitta/derivative data container */
      inline const data::DiscreteSet & get_data() const;

      /** get sagitta/derivative data container */
      inline data::DiscreteSet & get_data();

      /** Clear all points and fit to an other rotationally symmetric curve.
          @param c curve to fit
          @param radius Maximum radius where curve is defined
          @param points Number of sample points
      */
      void fit(const Rotational &c, double radius, unsigned int points);

      inline double sagitta(double r) const;
      inline double derivative(double r) const;

    protected:
      data::DiscreteSet _data;
    };

  }
}

#endif

