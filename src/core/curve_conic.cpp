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

#include <goptical/core/curve/Conic>
#include <goptical/core/math/Vector>
#include <goptical/core/math/VectorPair>
#include <goptical/core/math/VectorPair>

namespace _goptical {

  namespace curve {

    Conic::Conic(double roc, double sc)
      : ConicBase(roc, sc)
    {
    }

    inline double Conic::derivative(double r) const
    {
      // conical section differentiate (computed with Maxima)

      const double s2 = _sh * math::square(r);
      const double s3 = sqrt(1 - s2 / math::square(_roc));
      const double s4 = 2.0/(_roc * (s3+1)) + s2/(math::square(_roc) * _roc * s3 * math::square(s3 + 1));

      return r * s4;
    }

    double Conic::sagitta(double r) const
    {
      return math::square(r) / (_roc * (sqrt( 1 - (_sh * math::square(r)) / math::square(_roc)) + 1));
    }

    bool Conic::intersect(math::Vector3 &point, const math::VectorPair3 &ray) const
    {
      const double      ax = ray.origin().x();
      const double      ay = ray.origin().y();
      const double      az = ray.origin().z();
      const double      bx = ray.direction().x();
      const double      by = ray.direction().y();
      const double      bz = ray.direction().z();

      /*
        find intersection point between conical section and ray,
        telescope optics, page 266
      */
      double a = (_sh * math::square(bz) + math::square(by) + math::square(bx));
      double b = ((_sh * bz * az + by * ay + bx * ax) / _roc - bz) * 2.0;
      double c = (_sh * math::square(az) + math::square(ay) + math::square(ax)) / _roc - 2.0 * az;

      double t;

      if (a == 0)
        {
          t = -c / b;
        }
      else
        {
          double d = math::square(b) - 4.0 * a * c / _roc;

          if (d < 0)
            return false;               // no intersection

          double s = sqrt(d);

          if (a * bz < 0)
            s = -s;

          if (_sh < 0)
            s = -s;

          t = (2 * c) / (s - b);
        }

      if (t <= 0)               // ignore intersection if before ray origin
        return false;

      point = ray.origin() + ray.direction() * t;

      return true;
    }

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

      Best fit conic through (0,0)
      ============================

      y^2 / x = (e^2 - 1) * x - 2 * a * (e^2 - 1)

      is a line model of this form:

      Y = C0 + C1 * X

      with

      Y = y^2 / x
      X = x
      C0 = - 2 * a * (e^2 - 1)
      C1 = (e^2 - 1)

      C0 and C1 are found by least squares fit of points (x,y)

      eccentricity can then be computed for all conic sections with:

      e = sqrt(C1 + 1)
      sc = -C1 - 1

      radius of curvature can be computed:

      a = C0 / (-2 * C1)
      roc = a * (1 + sc);

      once simplified, works for all conic sections:

      roc = C0 / 2

     */

    double Conic::fit(const Rotational &c, double radius, unsigned int count)
    {
      double X[count], Y[count];

      double step = radius / (double)count;
      double y = step / 2.0;

      for (unsigned int i = 0; i < count; i++)
        {
          double x = c.sagitta(y);

          Y[i] = math::square(y) / x;
          X[i] = x;

          y += step;
        }

      double c0, c1, cov00, cov01, cov11, chisq;

      gsl_fit_linear(X, 1, Y, 1, count,
                     &c0, &c1, &cov00, &cov01, &cov11,
                     &chisq);

      _sh = -c1;
      _roc = c0 / 2.0;

      return sqrt(chisq / count); // FIXME bad rms error
    }

  }

}

