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

#include <cassert>

#include <gsl/gsl_fit.h>

#include <goptical/core/curve/ConicBase>

namespace _goptical {

  namespace curve {

    /*
      ellipse and hyperbola equation standard forms:

      ellipse:    (x^2)/(a^2) + (y^2)/(b^2) = 1
      hyperbola:  (x^2)/(a^2) - (y^2)/(b^2) = 1

      with bend point at (0,0):

      ellipse:    ((x-a)^2)/(a^2) + (y^2)/(b^2) = 1
      hyperbola:  ((x-a)^2)/(a^2) - (y^2)/(b^2) = 1

      and eccentricity:

      ellipse:    e = sqrt(1 - (b^2) / (a^2))
      hyperbola:  e = sqrt(1 + (b^2) / (a^2))

      both can be rewritten as:

      y^2 = (e^2 - 1) * x^2 - 2 * a * (e^2 - 1) * x

      Best fit conic through (0,0) with fixed eccentricity
      ====================================================

      * Ellipse, Circle, Hyperbola:

      y^2 - (e^2 - 1) * x^2 = -2 * a * (e^2 - 1) * x

      is a line model of this form:

      Y = C1 * X

      with

      Y = y^2 - (e^2 - 1) * x^2
      X = -2 * (e^2 - 1) * x
      C1 = a

      C1 is found by least squares fit of points (x,y)

      radius of curvature can be computed:

      roc = a * (1 + sc);
      roc = C1 * sc

      * Parabola:

      y^2 = 4 * a * x

      is used instead, with

      Y = y^2
      X = 4 * x
      C1 = a

      radius of curvature can be computed:

      roc = 2 * C1

    */

    double ConicBase::fit_roc(const Rotational &c, double radius, unsigned int count)
    {
      double X[count], Y[count];

      double step = radius / (double)count;
      double y = step / 2.0;
      double c1, cov11, chisq;

      if (_sh != 0.0)
        {
          for (unsigned int i = 0; i < count; i++)
            {
              double x = c.sagitta(y);

              Y[i] = math::square(y) + _sh * math::square(x);
              X[i] = 2.0 * _sh * x;

              y += step;
            }

          gsl_fit_mul(X, 1, Y, 1, count, &c1, &cov11, &chisq);

          _roc = c1 * _sh;
        }
      else
        { // Parabola special case
          for (unsigned int i = 0; i < count; i++)
            {
              double x = c.sagitta(y);

              Y[i] = math::square(y);
              X[i] = 4.0 * x;

              y += step;
            }

          gsl_fit_mul(X, 1, Y, 1, count, &c1, &cov11, &chisq);

          _roc = 2.0 * c1;
        }

      return sqrt(chisq / count); // FIXME bad rms error
    }

  }

}

