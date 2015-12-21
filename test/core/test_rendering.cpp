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

#include "config.hpp"

#include <goptical/core/io/Renderer>
#include <goptical/core/io/RendererViewport>
#include <goptical/core/io/Rgb>

#include <goptical/core/io/RendererSvg>
#include <goptical/core/io/RendererX3d>
#ifdef GOPTICAL_HAVE_PLPLOT
#include <goptical/core/io/RendererPlplot>
#endif
#ifdef GOPTICAL_HAVE_GD
#include <goptical/core/io/RendererGd>
#endif
#ifdef GOPTICAL_HAVE_PLPLOT
#include <goptical/core/io/RendererPlplot>
#endif
#ifdef GOPTICAL_HAVE_DIME
#include <goptical/core/io/RendererDxf>
#endif
#ifdef GOPTICAL_HAVE_X11
#include <goptical/core/io/RendererX11>
#endif

#include <goptical/core/math/Vector>
#include <goptical/core/math/VectorPair>

using namespace goptical;

void test_text_(io::Renderer &r, const math::Vector2 &pos, io::TextAlignMask a)
{
  using namespace io;

  for (double ra = 0; ra < 2.*M_PI - 1.e-6; ra += M_PI/6.)
    {
      math::Vector2 dir(cos(ra), sin(ra));

      r.draw_segment(math::VectorPair2(pos, pos + dir * 30), rgb_blue);
      r.draw_text(pos, dir, "A long long long test string",
                  a, 12, rgb_red);
    }
}

void test_text(io::Renderer &r)
{
  test_text_(r, math::Vector2(-90, 90), io::TextAlignLeft |   io::TextAlignMiddle);
  test_text_(r, math::Vector2(-30, 90), io::TextAlignCenter | io::TextAlignMiddle);
  test_text_(r, math::Vector2(30,  90), io::TextAlignRight |  io::TextAlignMiddle);
  test_text_(r, math::Vector2(90,  90), io::TextAlignCenter | io::TextAlignTop);
  test_text_(r, math::Vector2(-90, 30), io::TextAlignCenter | io::TextAlignMiddle);
  test_text_(r, math::Vector2(-30, 30), io::TextAlignCenter | io::TextAlignBottom);
}

void test_circle(io::Renderer &r)
{
  for (double ra = 0; ra < 80 ; ra += 10)
    {
      r.draw_circle(math::vector2_0, ra, io::rgb_green, false);
    }

  for (double x = -90; x < 90 + 1e-6 ; x += 30)
    {
      math::Vector2 v(x, 0.);
      r.draw_circle(v, 8, io::rgb_gray, true);
    }
}

void test_polygon(io::Renderer &r)
{
  math::Vector2 v[5];

  v[0] = math::Vector2(-10, -50);
  v[1] = math::Vector2(-5, -40);
  v[2] = math::Vector2(-15, -30);
  v[3] = math::Vector2(10, -25);
  v[4] = math::Vector2(5, -50);

  r.draw_polygon(v, 5, io::rgb_magenta, false, false);

  for (int i = 0; i < 5; i++)
    v[i] += math::Vector2(30, 0);

  r.draw_polygon(v, 5, io::rgb_magenta, true, false);

  for (int i = 0; i < 5; i++)
    v[i] += math::Vector2(30, 0);

  r.draw_polygon(v, 5, io::rgb_magenta, false, true);

  for (int i = 0; i < 5; i++)
    v[i] += math::Vector2(30, 0);

  r.draw_polygon(v, 5, io::rgb_magenta, true, true);
}

void test_point(io::Renderer &r)
{
  for (int i = 0; i < 5; i++)
    for (double x = -90; x < 90 + 1e-6 ; x += 5)
      r.draw_point(math::Vector2(x, -90 + i*5),
                   r.get_style_color(io::StyleForeground), (io::PointStyle)i);
}

/* Special class which forces use of fallback implementations from base class */

#define BASERENDERER_CLASS(R, ...)                                      \
  struct R##Basic : public io::R                                        \
{                                                                       \
  R##Basic()                                                            \
    : R(__VA_ARGS__)                                                    \
  {                                                                     \
  }                                                                     \
                                                                        \
  void draw_circle(const math::Vector2 &v, double r, const Rgb &rgb, bool filled) \
  {                                                                     \
    Renderer::draw_circle(v, r, rgb, filled);                           \
  }                                                                     \
                                                                        \
  void draw_polygon(const math::Vector2 *array, unsigned int count, const Rgb &rgb, bool filled, bool closed) \
  {                                                                     \
    Renderer::draw_polygon(array, count, rgb, filled, closed);          \
  }                                                                     \
};

using namespace io;

BASERENDERER_CLASS(RendererSvg, "test_basic_svg.svg", 1600, 1200);
#ifdef GOPTICAL_HAVE_DIME
BASERENDERER_CLASS(RendererDxf, "test_basic_dxf.dxf");
#endif

int main()
{
  io::Renderer *rt[] = {
#ifdef GOPTICAL_HAVE_X11
    new io::RendererX11 (),
#endif
    new io::RendererSvg ("test_2d_svg.svg", 1600, 1200),
    new RendererSvgBasic(),
    new io::RendererX3d ("test_2d_x3d.x3d"),
#ifdef GOPTICAL_HAVE_GD
    new io::RendererGd ("test_2d_gd_ttf.png", 1600, 1200, io::rgb_white, "/usr/share/fonts/TTF/FreeSans.ttf"),
    new io::RendererGd ("test_2d_gd.png", 1600, 1200, io::rgb_black),
#endif
#ifdef GOPTICAL_HAVE_PLPLOT
    new io::RendererPlplot ("png", "test_2d_plplot.png", 1600, 1200),
    new io::RendererPlplot ("ps", "test_2d_plplot.ps", 1600, 1200),
#endif
#ifdef GOPTICAL_HAVE_DIME
    new io::RendererDxf ("test_2d_dxf.dxf"),
    new RendererDxfBasic (),
#endif
    0
  };

  for (int i = 0; rt[i]; i++)
    {
      io::Renderer &r = *rt[i];
      io::RendererViewport *rv = dynamic_cast<io::RendererViewport*>(&r);

      if (rv)
        rv->set_window(math::VectorPair2(-100, -100, 100, 100));

      test_circle(r);
      test_polygon(r);
      test_point(r);
      test_text(r);

      if (rv)
        {
          rv->draw_frame_2d();
          rv->flush();
        }
    }

  sleep(5);

  for (int i = 0; rt[i]; i++)
    {
      delete rt[i];
    }

  return 0;
}

