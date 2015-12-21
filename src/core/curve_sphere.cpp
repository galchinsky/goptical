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


#include <goptical/core/curve/Sphere>
#include <goptical/core/math/Vector>
#include <goptical/core/math/VectorPair>
#include <goptical/core/math/VectorPair>

namespace _goptical {

  namespace curve {

    Sphere::Sphere(double roc)
      : ConicBase(roc, 0.0)
    {
    }

    double Sphere::sagitta(double r) const
    {
      double    x = fabs(_roc) - sqrt(math::square(_roc) - math::square(r));

      return _roc < 0 ? -x : x;
    }

    double Sphere::derivative(double r) const
    {
      return r / sqrt(math::square(_roc) - math::square(r));
    }

    /*

    ligne AB A + t * B
    sphere passant par C(Cx, 0, 0), rayon R

    d = Ax - R - Cx
    (Bz*t+Az)^2+(By*t+Ay)^2+(Bx*t+d)^2=R^2

    t=-(sqrt((Bz^2+By^2+Bx^2)*R^2+(-Bz^2-By^2)*d^2+(2*Az*Bx*Bz+2*Ay*Bx*By)
    *d-Ay^2*Bz^2+2*Ay*Az*By*Bz-Az^2*By^2+(-Az^2-Ay^2)*Bx^2)+Bx*d+Az*Bz+Ay*By)/(Bz^2+By^2+Bx^2),

    t= (sqrt((Bz^2+By^2+Bx^2)*R^2+(-Bz^2-By^2)*d^2+(2*Az*Bx*Bz+2*Ay*Bx*By)
    *d-Ay^2*Bz^2+2*Ay*Az*By*Bz-Az^2*By^2+(-Az^2-Ay^2)*Bx^2)-Bx*d-Az*Bz-Ay*By)/(Bz^2+By^2+Bx^2)

    */

    bool Sphere::intersect(math::Vector3 &point, const math::VectorPair3 &ray) const
    {
      const double      ax = (ray.origin().x());
      const double      ay = (ray.origin().y());
      const double      az = (ray.origin().z());
      const double      bx = (ray.direction().x());
      const double      by = (ray.direction().y());
      const double      bz = (ray.direction().z());

      // double bz2_by2_bx2 = math::square(bx) + math::square(by) + math::square(bx);
      // == 1.0

      double d = az - _roc;
      double ay_by = ay * by;
      double ax_bx = ax * bx;

      double s =
        + math::square(_roc) // * bz2_by2_bx2
        + 2.0 * (ax_bx + ay_by) * bz * d
        + 2.0 * ax_bx * ay_by
        - math::square(ay * bx)
        - math::square(ax * by)
        - (math::square(bx) + math::square(by)) * math::square(d)
        - (math::square(ax) + math::square(ay)) * math::square(bz)
        ;

      // no sphere/ray colision
      if (s < 0)
        return false;

      s = sqrt(s);

      // there are 2 possible sphere/line colision point, keep the right
      // one depending on ray direction
      if (_roc * bz > 0)
        s = -s;

      double t = (s - (bz * d + ax_bx + ay_by)); // / bz2_by2_bx2;

      // do not colide if line intersection is before ray start position
      if (t <= 0)
        return false;

      // intersection point
      point = ray.origin() + ray.direction() * t;

      return true;
    }

    void Sphere::normal(math::Vector3 &normal, const math::Vector3 &point) const
    {
      // normalized vector to sphere center
      normal = point;
      normal.z() -= _roc;
      normal.normalize();
      if (_roc < 0)
        normal = -normal;
    }

  }

}

