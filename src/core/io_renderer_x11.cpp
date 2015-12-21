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

#include <goptical/core/io/RendererX11>

namespace _goptical {

  namespace io {

    RendererX11::RendererX11(unsigned int width, unsigned int height,
                             const Rgb &bg,
                             Display *disp, int scr,
                             Window win, GC gc)
      : _disp(disp),
        _user_disp(false),
        _scr(scr),
        _win(win),
        _gc(gc),
        _user_gc(false)
    {
      _styles_color[StyleBackground] = bg;
      _styles_color[StyleForeground] = ~bg;

      if (!_disp)
        {
          if (!(_disp = XOpenDisplay(NULL)))
            throw Error("can not open display");
          _user_disp = true;
        }

      if (_scr == -1)
        _scr = DefaultScreen(_disp);

      if ((signed)_win == -1)
        {
          Window rw = RootWindow(_disp, _scr);

          _win = XCreateSimpleWindow(_disp, rw, 0, 0, width, height, 0,
                                     get_color(bg), get_color(bg));

          XMapWindow(_disp, _win);

          XSelectInput(_disp, _win, StructureNotifyMask);
          XEvent e;

          do {
            XNextEvent(_disp, &e);
          } while (e.type != MapNotify);
        }

      if (!_gc)
        {
          if (!(_gc = XCreateGC(_disp, _win, 0, NULL)))
            throw Error("can not create graphics context");

          _user_gc = true;
        }

      set_2d_size(width, height);
    }

    RendererX11::~RendererX11()
    {
      if (_user_gc)
        XFreeGC(_disp, _gc);
      if (_user_disp)
        XCloseDisplay(_disp);
    }

    void RendererX11::clear()
    {
      XClearWindow(_disp, _win);
    }

    void RendererX11::flush()
    {
      XFlush(_disp);
    }

    void RendererX11::draw_point(const math::Vector2 &p, const Rgb &rgb, enum PointStyle s)
    {
      math::Vector2 v2d(trans_pos(p));
      const unsigned int t = 3;

      XSetForeground(_disp, _gc, get_color(rgb));

      switch (s)
        {
        case PointStyleDot:
          XDrawPoint(_disp, _win, _gc, (int)v2d.x(), (int)v2d.y());
          break;

        case PointStyleCross:

          XDrawLine(_disp, _win, _gc,
                      (int)v2d.x() - t, (int)v2d.y(),
                      (int)v2d.x() + t, (int)v2d.y());
          XDrawLine(_disp, _win, _gc,
                      (int)v2d.x(), (int)v2d.y() - t,
                      (int)v2d.x(), (int)v2d.y() + t);
          break;

        case PointStyleRound:
          XDrawArc(_disp, _win, _gc, (int)v2d.x() - t, (int)v2d.y() - t,
                   t * 2, t * 2, 0, 64*360);
          break;

        case PointStyleSquare:
          XDrawLine(_disp, _win, _gc,
                      (int)v2d.x() - t, (int)v2d.y() + t,
                      (int)v2d.x() + t, (int)v2d.y() + t);
          XDrawLine(_disp, _win, _gc,
                      (int)v2d.x() + t, (int)v2d.y() - t,
                      (int)v2d.x() + t, (int)v2d.y() + t);
          XDrawLine(_disp, _win, _gc,
                      (int)v2d.x() - t, (int)v2d.y() - t,
                      (int)v2d.x() + t, (int)v2d.y() - t);
          XDrawLine(_disp, _win, _gc,
                      (int)v2d.x() - t, (int)v2d.y() - t,
                      (int)v2d.x() - t, (int)v2d.y() + t);
          break;

        case PointStyleTriangle:
          XDrawLine(_disp, _win, _gc,
                      (int)v2d.x(), (int)v2d.y() - t,
                      (int)v2d.x() + t, (int)v2d.y() + t);
          XDrawLine(_disp, _win, _gc,
                      (int)v2d.x() - t, (int)v2d.y() + t,
                      (int)v2d.x() + t, (int)v2d.y() + t);
          XDrawLine(_disp, _win, _gc,
                      (int)v2d.x(), (int)v2d.y() - t,
                      (int)v2d.x() - t, (int)v2d.y() + t);
          break;

        }
    }

    void RendererX11::draw_segment(const math::VectorPair2 &l, const Rgb &rgb)
    {
      math::Vector2 v2da = trans_pos(l[0]);
      math::Vector2 v2db = trans_pos(l[1]);

      XSetForeground(_disp, _gc, get_color(rgb));

      XDrawLine(_disp, _win, _gc,
                (int)v2da.x(), (int)v2da.y(),
                (int)v2db.x(), (int)v2db.y());
    }

    void RendererX11::draw_circle(const math::Vector2 &c, double r,
                                  const Rgb &rgb, bool filled)
    {
      math::Vector2 v2d = trans_pos(c);
      int xr = (int)x_scale(r);
      int yr = (int)y_scale(r);

      XSetForeground(_disp, _gc, get_color(rgb));

      if (filled)
        XFillArc(_disp, _win, _gc, (int)v2d.x() - xr, (int)v2d.y() - yr,
                 xr * 2, yr * 2, 0, 64*360);
      else
        XDrawArc(_disp, _win, _gc, (int)v2d.x() - xr, (int)v2d.y() - yr,
                 xr * 2, yr * 2, 0, 64*360);
    }

    void RendererX11::draw_polygon(const math::Vector2 *array, unsigned int count,
                                   const Rgb &rgb, bool filled, bool closed)
    {
      if (count < 3)
        return;

      XPoint pts[count];

      XSetForeground(_disp, _gc, get_color(rgb));

      for (unsigned int i = 0; i < count; i++)
        {
          pts[i].x = x_trans_pos(array[i].x());
          pts[i].y = y_trans_pos(array[i].y());
        }

      if (filled)
        XFillPolygon(_disp, _win, _gc, pts, count, Complex, CoordModeOrigin);
      else
        {
          XDrawLines(_disp, _win, _gc, pts, count, CoordModeOrigin);
          if (closed)
            XDrawLine(_disp, _win, _gc, pts[count - 1].x, pts[count - 1].y, pts[0].x, pts[0].y);
        }
    }

    void RendererX11::draw_text(const math::Vector2 &c, const math::Vector2 &dir,
                                const std::string &str, TextAlignMask a, int size, const Rgb &rgb)
    {
      math::Vector2 v2d = trans_pos(c);

      int x = (int)v2d.x();
      int y = (int)v2d.y();

      // FIXME handle rotated text

      XFontStruct *fs = XQueryFont(_disp, XGContextFromGC(_gc));

      unsigned int w = XTextWidth(fs, str.c_str(), str.size());

      if (a & TextAlignCenter)
        {
          x -= w / 2;
        }
      else if (a & TextAlignRight)
        {
          x -= w;
        }

      XSetForeground(_disp, _gc, get_color(rgb));
      XDrawString(_disp, _win, _gc, x, y, str.c_str(), str.size());
    }

    long RendererX11::get_color(const Rgb &rgb) const
    {
      // safe values for black & white cases
      if (&rgb == &rgb_white)
        return WhitePixel(_disp, _scr);
      if (&rgb == &rgb_black)
        return BlackPixel(_disp, _scr);

      else // rgb truecolors here
        return (long)(unsigned char)(rgb.r * 255) << 16 |
          (long)(unsigned char)(rgb.g * 255) << 8  |
          (long)(unsigned char)(rgb.b * 255);
    }

  }

}

