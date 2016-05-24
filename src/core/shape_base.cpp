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

#include <cstdlib>

#include <goptical/core/shape/Base>
#include <goptical/core/math/Vector>
#include <goptical/core/trace/Distribution>

namespace _goptical {

  namespace shape {

#define ADD_PATTERN_POINT(v)                    \
    {                                           \
      math::Vector2 v_(v);                    \
      if (unobstructed || inside(v_))           \
        f(v_);                                  \
    }

    void Base::get_pattern(const math::Vector2::put_delegate_t  &f,
                            const trace::Distribution &d,
                            bool unobstructed) const
    {
      static const double epsilon = 1e-8;
      // FIXME use bounding box instead of max radius
      const double      tr = max_radius() * d.get_scaling();
      const double      step = tr / d.get_radial_density();

      trace::Pattern p = d.get_pattern();

      switch (p)
        {
        case trace::MeridionalDist: {

          double        r = tr;

          ADD_PATTERN_POINT(math::vector2_0);

          for (unsigned int i = 0; i < d.get_radial_density(); i++)
            {
              ADD_PATTERN_POINT(math::Vector2(0, r));
              ADD_PATTERN_POINT(math::Vector2(0, -r));
              r -= step;
            }
          break;
        }

        case trace::SagittalDist: {

          double r = tr;

          ADD_PATTERN_POINT(math::Vector2(0, 0));

          for (unsigned int i = 0; i < d.get_radial_density(); i++)
            {
              ADD_PATTERN_POINT(math::Vector2(r, 0));
              ADD_PATTERN_POINT(math::Vector2(-r, 0));
              r -= step;
            }
          break;
        }

        case trace::CrossDist: {

          double r = step;

          ADD_PATTERN_POINT(math::Vector2(0, 0));

          for (unsigned int i = 0; i < d.get_radial_density(); i++)
            {
              ADD_PATTERN_POINT(math::Vector2(0, r));
              ADD_PATTERN_POINT(math::Vector2(r, 0));
              ADD_PATTERN_POINT(math::Vector2(0, -r));
              ADD_PATTERN_POINT(math::Vector2(-r, 0));
              r += step;
            }
          break;
        }

        case trace::RandomDist: {

          double x, y;

          for (x = -tr; x < tr; x += step)
            {
              double ybound = sqrt(math::square(tr) - math::square(x));

              for (y = -ybound; y < ybound; y += step)
              {
                  ADD_PATTERN_POINT(math::Vector2(x + (drand48() - .5) * step,
                                                    y + (drand48() - .5) * step));
                }

            }
          break;
        }

        case trace::HexaPolarDist: {

          ADD_PATTERN_POINT(math::Vector2(0, 0));

          for (double r = tr; r > epsilon; r -= step)
            {
              double astep = (step / r) * (M_PI / 3);

              for (double a = 0; a < 2 * M_PI - epsilon; a += astep)
                ADD_PATTERN_POINT(math::Vector2(sin(a) * r, cos(a) * r));
            }

          break;
        };

        case trace::SquareDist: {

          ADD_PATTERN_POINT(math::Vector2(0, 0));

          double x, y;

          for (x = tr; x > epsilon; x -= step)
            {
              double ybound = sqrt(math::square(tr) - math::square(x));

              for (y = step; y < ybound; y += step)
                {
                  ADD_PATTERN_POINT(math::Vector2(x, y));
                  ADD_PATTERN_POINT(math::Vector2(x, -y));
                  ADD_PATTERN_POINT(math::Vector2(-x, y));
                  ADD_PATTERN_POINT(math::Vector2(-x, -y));
                }

              ADD_PATTERN_POINT(math::Vector2(x, 0));
              ADD_PATTERN_POINT(math::Vector2(-x, 0));
            }

          for (y = step; y < tr + epsilon; y += step)
            {
              ADD_PATTERN_POINT(math::Vector2(0, y));
              ADD_PATTERN_POINT(math::Vector2(0, -y));
            }
          break;
        }

        case trace::DefaultDist:
        case trace::TriangularDist: {
          static const double sqrt_3_2 = 0.86602540378443864676;
          double x, y;
          unsigned i = 1;

          for (x = step * sqrt_3_2; x < tr + epsilon; x += step * sqrt_3_2)
            {
              for (y = step / (double)i; y < tr + epsilon; y += step)
                {
                  double h = hypot(x, y);

                  if (h > tr)
                    break;

                  ADD_PATTERN_POINT(math::Vector2(x, y));
                  ADD_PATTERN_POINT(math::Vector2(-x, y));
                  ADD_PATTERN_POINT(math::Vector2(x, -y));
                  ADD_PATTERN_POINT(math::Vector2(-x, -y));
                }

              i ^= 3;
            }

          for (y = step / 2.0; y < tr + epsilon; y += step)
            {
              ADD_PATTERN_POINT(math::Vector2(0, y));
              ADD_PATTERN_POINT(math::Vector2(0, -y));
            }

          for (x = step * sqrt_3_2; x < tr + epsilon; x += step * sqrt_3_2 * 2.0)
            {
              ADD_PATTERN_POINT(math::Vector2(x, 0));
              ADD_PATTERN_POINT(math::Vector2(-x, 0));
            }
          break;
        }

        default:
          throw Error("distribution pattern not supported for this shape");
        }
    }

    double Base::get_hole_radius(const math::Vector2 &dir) const
    {
      return 0.;
    }

  }

}

