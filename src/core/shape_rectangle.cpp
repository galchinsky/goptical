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
#include <cassert>

#include <goptical/core/shape/Rectangle>

#include <goptical/core/trace/Distribution>
#include <goptical/core/math/Triangle>
#include <goptical/core/math/VectorPair>

namespace _goptical {

  namespace shape {

    bool Rectangle::inside(const math::Vector2 &point) const
    {
      return (fabs(point.x()) <= _halfsize.x() && fabs(point.y()) <= _halfsize.y());
    }

    void Rectangle::get_pattern(const math::Vector2::put_delegate_t  &f,
                                const trace::Distribution &d,
                                bool unobstructed) const
    {
      static const double epsilon = 1e-8;
      const math::Vector2 hs = _halfsize * d.get_scaling();
      const math::Vector2 step = hs / (double)(d.get_radial_density() / 2);

      switch (d.get_pattern())
        {
        case trace::MeridionalDist: {

          f(math::Vector2(0, 0));

          for (double y = step.y(); y < hs.y() + epsilon; y += step.y())
            {
              f(math::Vector2(0, y));
              f(math::Vector2(0, -y));
            }
          break;
        }

        case trace::SagittalDist: {

          f(math::Vector2(0, 0));

          for (double x = step.x(); x < hs.x() + epsilon; x += step.x())
            {
              f(math::Vector2(x, 0));
              f(math::Vector2(-x, 0));
            }
          break;
        }

        case trace::CrossDist: {

          f(math::Vector2(0, 0));

          for (double x = step.x(); x < hs.x() + epsilon; x += step.x())
            {
              f(math::Vector2(x, 0));
              f(math::Vector2(-x, 0));
            }

          for (double y = step.y(); y < hs.y() + epsilon; y += step.y())
            {
              f(math::Vector2(0, y));
              f(math::Vector2(0, -y));
            }
          break;
        }

        case trace::DefaultDist:
        case trace::SquareDist: {
          double x, y;

          f(math::Vector2(0, 0));

          for (x = step.x(); x < hs.x() + epsilon; x += step.x())
            for (y = step.y(); y < hs.y() + epsilon; y += step.y())
              {
                f(math::Vector2(x, y));
                f(math::Vector2(-x, y));
                f(math::Vector2(x, -y));
                f(math::Vector2(-x, -y));
              }

          for (x = step.x(); x < hs.x() + epsilon; x += step.x())
            {
              f(math::Vector2(x, 0));
              f(math::Vector2(-x, 0));
            }

          for (y = step.y(); y < hs.y() + epsilon; y += step.y())
            {
              f(math::Vector2(0, y));
              f(math::Vector2(0, -y));
            }
          break;
        }

        default:
          Base::get_pattern(f, d, unobstructed);
        }
    }

    math::VectorPair2 Rectangle::get_bounding_box() const
    {
      return math::VectorPair2(-_halfsize, _halfsize);
    }

    unsigned int Rectangle::get_contour_count() const
    {
      return 1;
    }

    inline math::Vector2 Rectangle::get_step(double resolution) const
    {
      math::Vector2 step;

      for (unsigned int i = 0; i < 2; i++)
        {
          if (resolution > _halfsize[i])
            step[i] = _halfsize[i];
          else
            step[i] = _halfsize[i] / round(_halfsize[i] / resolution);
        }

      return step;
    }

    void Rectangle::get_contour(unsigned int contour, const math::Vector2::put_delegate_t  &f, double resolution) const
    {
      static const double epsilon = 1e-8;
      assert(contour < 1);

      math::Vector2 step = get_step(resolution);

      double x, y;

      for (x = - _halfsize.x(); x < _halfsize.x() - epsilon; x += step.x())
        f(math::Vector2(x, - _halfsize.y()));

      for (y = - _halfsize.y(); y < _halfsize.y() - epsilon; y += step.y())
        f(math::Vector2(_halfsize.x(), y));

      for (x = _halfsize.x(); x > - _halfsize.x() + epsilon; x -= step.x())
        f(math::Vector2(x, _halfsize.y()));

      for (y = _halfsize.y(); y > - _halfsize.y() + epsilon; y -= step.y())
        f(math::Vector2(- _halfsize.x(), y));
    }

    void Rectangle::get_triangles(const math::Triangle<2>::put_delegate_t  &f, double resolution) const
    {
      static const double epsilon = 1e-8;
      math::Vector2 step = get_step(resolution);

      for (unsigned int i = 0; i < 2; i++)
        {
          if (resolution > _halfsize[i])
            step[i] = _halfsize[i];
          else
            step[i] = _halfsize[i] / round(_halfsize[i] / resolution);
        }

      for (double x = 0; x < _halfsize.x() - epsilon; x += step.x())
        for (double y = 0; y < _halfsize.y() - epsilon; y += step.y())
          {
            math::Vector2 a(x, y);
            math::Vector2 b(x + step.x(), y);
            math::Vector2 c(x, y + step.y());
            math::Vector2 d(x + step.x(), y + step.y());

            f(math::Triangle<2>(b, a, c));
            f(math::Triangle<2>(d, b, c));
            f(math::Triangle<2>(-b, -a, -c));
            f(math::Triangle<2>(-d, -b, -c));

            a.x() = -a.x();
            b.x() = -b.x();
            c.x() = -c.x();
            d.x() = -d.x();

            f(math::Triangle<2>(a, b, c));
            f(math::Triangle<2>(b, d, c));
            f(math::Triangle<2>(-a, -b, -c));
            f(math::Triangle<2>(-b, -d, -c));
          }
    }

    double Rectangle::get_outter_radius(const math::Vector2 &dir) const
    {
      const math::Vector2 &e = (fabs(dir.x() / dir.y()) < _halfsize.x() / _halfsize.y())
        ? math::vector2_10 : math::vector2_01;

      return (math::VectorPair2(_halfsize, e)
              .ln_intersect_ln(math::VectorPair2(math::vector2_0, dir))).len();
    }

  }

}
