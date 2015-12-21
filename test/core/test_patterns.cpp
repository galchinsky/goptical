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
#include <cstring>
#include <stdlib.h>

#include "config.hpp"

#include <goptical/core/io/RendererViewport>
#include <goptical/core/io/RendererSvg>
#include <goptical/core/io/Rgb>

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

using namespace goptical;

#define SINGLE_IMAGE

size_t err = 0;

int main()
{
  struct shape_test_s
  {
    const char *name;
    shape::Base *s;
    bool unobstructed;
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

  shape_test_s st[] = {
    { "disk", new shape::Disk(30) },
    { "ring", new shape::Ring(30, 15) },
    { "ring_unob", new shape::Ring(30, 15), true },
    { "ring2", new shape::Ring(35, 10) },
    { "ring2_unob", new shape::Ring(35, 10), true },
    { "ellipse1", new shape::Ellipse(20, 30) },
    { "ellipse2", new shape::Ellipse(30, 20) },
    { "ellipticalring1", new shape::EllipticalRing(30, 20, 20) },
    { "ellipticalring1_unob", new shape::EllipticalRing(30, 20, 20), true },
    { "ellipticalring2", new shape::EllipticalRing(20, 32, 10) },
    { "ellipticalring2_unob", new shape::EllipticalRing(20, 32, 10), true },
    { "rectangle", new shape::Rectangle(70., 40.) },
    { "regularpolygon6", new shape::RegularPolygon(30, 6) },
    { "regularpolygon5", new shape::RegularPolygon(30, 5) },
    { "regularpolygon5r", new shape::RegularPolygon(30, 5, 10.) },
    { "polygon1", poly1 },
    { 0 }
  };

  const char *pname[] = {
    "default",
    "sagittal",
    "tangential",
    "cross",
    "square",
    "triangular",
    "hexpolar",
    "random"
  };

  for (int i = 0; st[i].name; i++)
    {
      shape_test_s &s = st[i];

#ifdef SINGLE_IMAGE
      char fname[48];
      std::sprintf(fname, "test_pattern_%s.svg", s.name);

      io::RendererSvg rsvg(fname, 800, 400, io::rgb_white);
      io::RendererViewport &r = rsvg;

      r.set_page_layout(4, 2);
#endif

      for (int j = 0; j <= trace::RandomDist; j++)
        {
#ifndef SINGLE_IMAGE
          char fname[48];
          std::sprintf(fname, "test_pattern_%s_%s.svg", s.name, pname[j]);

          io::RendererSvg rsvg(fname, 400, 400, io::rgb_white);
          io::RendererViewport &r = rsvg;
#else
          r.set_page(j);
#endif

          std::cerr << s.name << " " << pname[j] << std::endl;

          r.set_window(math::vector2_0, 70);
          r.set_feature_size(.1);

          std::vector<math::Vector2 > pts;
          dpp::delegate_push<typeof(pts)> d(pts);

          try {
            trace::Distribution dist((trace::Pattern)j);
            s.s->get_pattern(d, dist, s.unobstructed);
          } catch (...) {
            continue;
          }

          if (pts.size() < 4)
            err++;

          bool first = true;

          for (auto&v, pts)
            {
              r.draw_point(*v, io::rgb_red, io::PointStyleCross);

              // Chief ray must be the first ray in list, some analysis do rely on this
              if (!first && v->close_to(math::vector2_0, 1) && j != trace::RandomDist)
                {
                  std::cerr << "-- chief !first " << *v << "\n";
                  err++;
                }

              if (!s.unobstructed && !s.s->inside(*v))
                {
                  std::cerr << "-- !inside " << *v << "\n";
                  err++;
                }
              
              if (j != trace::RandomDist)
                {
                  // check for duplicates
                  for (auto&w, pts)
                    if (&*v != &*w && v->close_to(*w, 1))
                      {
                        std::cerr << "-- dup " << *w << *v << "\n";
                        err++;
                      }
                }

              first = false;
            }

          for (unsigned int c = 0; c < s.s->get_contour_count(); c++)
            {
              std::vector<math::Vector2 > poly;
              delegate_push<typeof(poly)> d(poly);
              s.s->get_contour(c, d, 10.);
              r.draw_polygon(&poly[0], poly.size(), io::rgb_black);
            }

          r.draw_text(math::Vector2(0, -43), math::vector2_10,
                      fstring("%s: %i points", pname[j], pts.size()),
                      //pname[j],
                      io::TextAlignCenter | io::TextAlignBottom, 18);
        }

      //      r.draw_pages_grid(io::rgb_black);
    }

  return err > 0;
}

