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

#include <goptical/core/curve/Base>
#include <goptical/core/math/Vector>
#include <goptical/core/math/VectorPair>

#include <gsl/gsl_deriv.h>

namespace _goptical {

  namespace curve {

    // Default curve/ray intersection iterative method

    bool Base::intersect(math::Vector3 &point, const math::VectorPair3 &ray) const
    {
      math::VectorPair3 p;

      // initial intersection with z=0 plane
      {
        double  s = ray.direction().z();

        if (s == 0)
          return false;

        double  a = -ray.origin().z() / s;

        if (a < 0)
          return false;

        p.origin() = ray.origin() + ray.direction() * a;
      }

      unsigned int n = 32;      // avoid infinite loop

      while (n--)
        {
          double new_sag = sagitta(p.origin().project_xy());
          double old_sag = p.origin().z();

          // project previous intersection point on curve
          p.origin().z() = new_sag;

          // stop if close enough
          if (fabs(old_sag - new_sag) < 1e-10)
            break;

          // get curve tangeante plane at intersection point
          normal(p.normal(), p.origin());

          // intersect again with new tangeante plane
          double a = p.pl_ln_intersect_scale(ray);

          if (a < 0)
            return false;

          p.origin() = ray.origin() + ray.direction() * a;
        }

      point = p.origin();

      return true;
    }

    // Default curve derivative use gsl numerical differentiation

    struct curve_gsl_params_s
    {
      const Base *c;
      double x, y;
    };

    static double gsl_func_sagitta_x(double x, void *params)
    {
      struct curve_gsl_params_s *p = (struct curve_gsl_params_s*)params;

      return p->c->sagitta(math::Vector2(x, p->y));
    }

    static double gsl_func_sagitta_y(double y, void *params)
    {
      struct curve_gsl_params_s *p = (struct curve_gsl_params_s*)params;

      return p->c->sagitta(math::Vector2(p->x, y));
    }

    void Base::derivative(const math::Vector2 & xy, math::Vector2 & dxdy) const
    {
      double abserr;
      struct curve_gsl_params_s params;
      gsl_function gsl_func;

      gsl_func.params = &params;

      params.c = this;
      params.x = xy.x();
      params.y = xy.y();

      gsl_func.function = gsl_func_sagitta_x;
      gsl_deriv_central(&gsl_func, xy.x(), 1e-6, &dxdy.x(), &abserr);

      gsl_func.function = gsl_func_sagitta_y;
      gsl_deriv_central(&gsl_func, xy.y(), 1e-6, &dxdy.y(), &abserr);
    }

    void Base::normal(math::Vector3 &normal, const math::Vector3 &point) const
    {
      math::Vector2 d;

      derivative(point.project_xy(), d);

      normal = math::Vector3(d.x(), d.y(), -1.0);
      normal.normalize();
    }

  }

}

