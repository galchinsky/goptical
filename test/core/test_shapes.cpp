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

#include <cstdio>
#include <stdlib.h>

#include "config.hpp"

#include <goptical/core/io/Renderer>
#include <goptical/core/io/RendererViewport>
#include <goptical/core/io/Rgb>
#include <goptical/core/io/RendererSvg>

#include <goptical/core/trace/Distribution>

#include <goptical/core/shape/Base>
#include <goptical/core/shape/Disk>
#include <goptical/core/shape/Ring>
#include <goptical/core/shape/Ellipse>
#include <goptical/core/shape/EllipticalRing>
#include <goptical/core/shape/Rectangle>
#include <goptical/core/shape/RegularPolygon>
#include <goptical/core/shape/Polygon>
#include <goptical/core/shape/Composer>

#include <goptical/core/math/Vector>
#include <goptical/core/math/Triangle>

using namespace goptical;

int main()
{
  struct shape_test_s
  {
    const char *name;
    shape::Base *s;
  };

  shape::Polygon *poly1 = new shape::Polygon;

  poly1->add_vertex(math::Vector2(30, 20));
  poly1->add_vertex(math::Vector2(20, 30));
  poly1->add_vertex(math::Vector2(-20, 30));
  poly1->add_vertex(math::Vector2(-30, 20));
  poly1->add_vertex(math::Vector2(-30, -10));
  poly1->add_vertex(math::Vector2(-20, -20));
  poly1->add_vertex(math::Vector2(20, -20));
  poly1->add_vertex(math::Vector2(30, -10));

  srand48(time(0));

#if 0
  shape::Polygon *poly2 = new shape::Polygon;

  for (double a = 0; a < 2.0 * M_PI - 1e-8; a += 2.0 * M_PI / 15)
    {
      double r = (5 + drand48() * 30);
      poly2->add_vertex(math::Vector2(cos(a) * r,
                                        sin(a) * r));
    }
#endif

  shape::Composer *compose1 = new shape::Composer;

  compose1->add_shape(ref<shape::Disk>::create(30.))
    .translate(20.)
    .exclude(ref<shape::Rectangle>::create(20.));

  compose1->add_shape(ref<shape::Disk>::create(20.))
    .translate(-20.);

  shape_test_s st[] = {
    { "disk", new shape::Disk(30) },
    { "ring", new shape::Ring(30, 15) },
    { "ellipse1", new shape::Ellipse(20, 35) },
    { "ellipse2", new shape::Ellipse(35, 20) },
    { "ellipticalring1", new shape::EllipticalRing(35, 20, 20) },
    { "ellipticalring2", new shape::EllipticalRing(20, 35, 10) },
    { "rectangle", new shape::Rectangle(70., 40.) },
    { "regularpolygon6", new shape::RegularPolygon(30, 6) },
    { "regularpolygon5", new shape::RegularPolygon(30, 5) },
    { "regularpolygon5r", new shape::RegularPolygon(30, 5, 10.) },
    { "polygon1", poly1 },
    //    { "polygon2", poly2 },
    { "composer1", compose1 },
    { 0 }
  };

  for (int i = 0; st[i].name; i++)
    {
      shape_test_s &s = st[i];

      char fname[48];
      std::sprintf(fname, "test_shape_%s.svg", s.name);

      io::RendererSvg rsvg(fname, 800, 600, io::rgb_black);
      io::RendererViewport &r = rsvg;

      r.set_window(math::vector2_0, 70);

      {
        DPP_DELEGATE1_OBJ(d, void, (const math::Triangle<2> &t),
                          io::Renderer &, r, // _0
        {
          _0.draw_triangle(t, true, io::Rgb(.2, .2, .2));
          _0.draw_triangle(t, false, io::rgb_gray);
        });
        s.s->get_triangles(d, 10.);
      }

      for (unsigned int c = 0; c < s.s->get_contour_count(); c++)
        {
          std::vector<math::Vector2 > poly;
          delegate_push<typeof(poly)> d(poly);
          s.s->get_contour(c, d, 10.);
          r.draw_polygon(&poly[0], poly.size(), io::rgb_yellow);
        }

      for (double a = 0; a < 2.0 * M_PI - 1e-8; a += 2.0 * M_PI / 50)
        {
          math::Vector2 d(cos(a), sin(a));

          double ro = s.s->get_outter_radius(d);
          r.draw_point(d * ro, io::rgb_magenta, io::PointStyleCross);
          double rh = s.s->get_hole_radius(d);
          r.draw_point(d * rh, io::rgb_cyan, io::PointStyleCross);
        }

      r.draw_circle(math::vector2_0, s.s->max_radius(), io::rgb_red);
      r.draw_circle(math::vector2_0, s.s->min_radius(), io::rgb_blue);

      r.draw_box(s.s->get_bounding_box(), io::rgb_cyan);

      {
        DPP_DELEGATE1_OBJ(d, void, (const math::Vector2 &v),
                          io::Renderer &, r, // _0
        {
          _0.draw_point(v, io::rgb_green);
        });

        trace::Distribution dist;
        s.s->Base::get_pattern(d, dist);
      }
    }

}

