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


#include <goptical/core/math/Vector>
#include <goptical/core/math/VectorPair>
#include <goptical/core/io/Rgb>
#include <goptical/core/io/RendererAxes>
#include <goptical/core/io/RendererPlplot>
#include <goptical/core/data/Plot>
#include <goptical/core/data/PlotData>
#include <goptical/core/data/Set1d>

#define PL_DOUBLE
#undef HAVE_CONFIG_H
#include <plplot/plstream.h>

namespace _goptical {

  namespace io {

    void RendererPlplot::palette_init()
    {
      // FIXME handle more predefined plplot col0 colors
      _palette[rgb_white] = 15;
    }

    RendererPlplot::RendererPlplot(const char *device, const char *filename,
                                   unsigned int width, unsigned int height,
                                   const Rgb &bg)
      : _user_pls(false),
        _palette(),
        _palette_size(16)
    {
      _2d_output_res = math::Vector2(width, height);

      _pls = new plstream();

      // colors init
      _styles_color[StyleBackground] = bg;
      _styles_color[StyleForeground] = ~bg;

      _pls->scolbg((int)(bg.r * 255), (int)(bg.g * 255), (int)(bg.b * 255));

      _palette[bg] = 0;
      palette_init();

      // backend init
      _pls->sdev(device);
      _pls->spage(0.0, 0.0, (PLINT)_2d_output_res.x(), (PLINT)_2d_output_res.y(), 0, 0);

      _pls->sfnam(filename);
      _pls->star(1, 1);
      _pls->adv(0);
    }

    RendererPlplot::RendererPlplot(plstream *pls)
      : _pls(pls),
        _user_pls(true),
        _palette(),
        _palette_size(16)
    {
      PLINT width, height, i;
      PLFLT f;
      _pls->gpage(f, f, width, height, i, i);
      _2d_output_res = math::Vector2(width, height);

      // FIXME init _styles_color[StyleBackground] and [StyleForeground] using gcolbg

      palette_init();
    }

    RendererPlplot::~RendererPlplot()
    {
      if (!_user_pls)
        delete _pls;
    }

    void RendererPlplot::set_page_layout(unsigned int cols, unsigned int rows)
    {
      _cols = cols;
      _rows = rows;
      _pls->ssub(cols, rows);
    }

    void RendererPlplot::set_page(unsigned int page)
    {
      _pageid = page;
      _pls->adv(page + 1);
    }

    void RendererPlplot::pl_set_drawing_window()
    {
      // margin inside drawable area
      _pls->vpor(0., 1., 0., 1.);

      _pls->wind(_window2d[0].x(), _window2d[1].x(),
                 _window2d[0].y(), _window2d[1].y());
    }

    void RendererPlplot::pl_set_plot_window()
    {
      // margin outside drawable area
      _pls->vpor(
                 (_window2d_fit[0].x() - _window2d[0].x()) / (_window2d[1].x() - _window2d[0].x()),
                 1. - (_window2d[1].x() - _window2d_fit[1].x()) / (_window2d[1].x() - _window2d[0].x()),
                 (_window2d_fit[0].y() - _window2d[0].y()) / (_window2d[1].y() - _window2d[0].y()),
                 1. - (_window2d[1].y() - _window2d_fit[1].y()) / (_window2d[1].y() - _window2d[0].y())
                 );

      _pls->wind(_window2d_fit[0].x(), _window2d_fit[1].x(),
                 _window2d_fit[0].y(), _window2d_fit[1].y());
    }

    void RendererPlplot::update_2d_window()
    {
      pl_set_drawing_window();
    }

    void RendererPlplot::set_2d_size(double width, double height)
    {
      RendererViewport::set_2d_size(width, height);
      _pls->spage(0.0, 0.0, (PLINT)width, (PLINT)height, 0, 0);
    }

    void RendererPlplot::draw_point(const math::Vector2 &v, const Rgb &rgb, enum PointStyle s)
    {
      int code;

      _pls->col0(get_color_id(rgb));

      switch (s)
        {
        default:
        case PointStyleDot:
          code = 1;
          break;

        case PointStyleCross:
          code = 2;
          break;

        case PointStyleRound:
          code = 4;
          break;

        case PointStyleSquare:
          code = 6;
          break;

        case PointStyleTriangle:
          code = 11;
          break;
        }

      PLFLT x = v.x(), y = v.y();
      _pls->poin(1, &x, &y, code);
    }

    void RendererPlplot::draw_segment(const math::VectorPair2 &l, const Rgb &rgb)
    {
      _pls->col0(get_color_id(rgb));
      _pls->join(l[0].x(), l[0].y(), l[1].x(), l[1].y());
    }

    void RendererPlplot::draw_circle(const math::Vector2 &c, double r,
                                     const Rgb &rgb, bool filled)
    {
      _pls->col0(get_color_id(rgb));
      std::cout << "draw_circle was not tested (added 8th parameter to compile" << std::endl;
      exit(-1);
      _pls->arc(c.x(), c.y(), r, r, 0., 360., 360., filled);
    }

    void RendererPlplot::draw_polygon(const math::Vector2 *array, unsigned int count,
                                      const Rgb &rgb, bool filled, bool closed)
    {
      if (count < 3)
        return;

      if (!filled)
        return Renderer::draw_polygon(array, count, rgb, filled, closed);

      PLFLT x[count];
      PLFLT y[count];

      for (unsigned int i = 0; i < count; i++)
        {
          x[i] = array[i].x();
          y[i] = array[i].y();
        }

      _pls->col0(get_color_id(rgb));
      _pls->fill(count, x, y);
    }

    void RendererPlplot::draw_text(const math::Vector2 &c, const math::Vector2 &dir,
                                   const std::string &str, TextAlignMask a, int size, const Rgb &rgb)
    {
      double j = .5;

      if (a & TextAlignLeft)
        j = 0.;
      else if (a & TextAlignRight)
        j = 1.;

      _pls->schr(size/3., 1.);
      _pls->col0(get_color_id(rgb));
      _pls->ptex(c.x(), c.y(), dir.x(), dir.y(), j, str.c_str());
    }

    void RendererPlplot::pl_draw_axes(const RendererAxes &a)
    {
      std::string xopts, yopts;

      if (a.get_show_axes(0))
        xopts += "abc";
      if (a.get_show_grid())
        xopts += "g";
      if (a.get_show_tics(0))
        xopts += "t";
      if (a.get_show_values(0))
        xopts += "n";

      if (a.get_show_axes(1))
        yopts += "abc";
      if (a.get_show_grid())
        yopts += "g";
      if (a.get_show_tics(1))
        yopts += "t";
      if (a.get_show_values(1))
        yopts += "n";

      _pls->col0(get_color_id(get_style_color(StyleForeground)));

      // temporarily change window coordinates to adjust axes numeric origin

      math::Vector2 o(project(a.get_origin()));

      _pls->wind(_window2d_fit[0].x() - o.x(), _window2d_fit[1].x() - o.x(),
                _window2d_fit[0].y() - o.y(), _window2d_fit[1].y() - o.y());

      // draw axes and box

      math::Vector2 ap(project(a.get_position() - a.get_origin()));

      math::range_t rx(_window2d_fit[0].x(), _window2d_fit[1].x());
      math::range_t ry(_window2d_fit[0].y(), _window2d_fit[1].y());

      math::Vector2 step(fabs(a.get_tics_step(0, rx)),
                           fabs(a.get_tics_step(1, ry)));

      _pls->axes(ap.x(), ap.y(),
                xopts.c_str(), step.x(), 0,
                yopts.c_str(), step.y(), 0);
    }

    void RendererPlplot::draw_axes_2d(const RendererAxes &a)
    {
      pl_set_plot_window();
      pl_draw_axes(a);
      pl_set_drawing_window();
    }

    void RendererPlplot::draw_axes_3d(const RendererAxes &a, const math::Vector3 &range)
    {
      // FIXME
    }

    void RendererPlplot::draw_plot(const data::Plot &plot)
    {
      switch (plot.get_dimensions())
        {
        case 1:
          // FIXME
          set_2d_plot_window(plot);

          pl_set_plot_window();
          pl_draw_axes(plot.get_axes());

          _pls->col0(get_color_id(get_style_color(StyleForeground)));

          pllab((char*)plot.get_axes().get_label(0).c_str(),
                (char*)plot.get_axes().get_label(1).c_str(),
                (char*)plot.get_title().c_str());

          for (unsigned int i = 0; i < plot.get_plot_count(); i++)
            {
              const data::Plotdata &d = plot.get_plot_data(i);
              draw_plot_data_2d(static_cast<const data::Set1d&>(d.get_set()), d);
            }

          pl_set_drawing_window();
          break;

        default:
          RendererViewport::draw_plot(plot);
        }
    }

    int RendererPlplot::get_color_id(const Rgb &rgb)
    {
      palette_t::iterator i = _palette.find(rgb);

      if (i == _palette.end())
        {
          if (_palette_size > 1000)
            return 15;

          if (_palette_size > 15)
            _pls->scmap0n(_palette_size + 1);
          _palette[rgb] = _palette_size;
          _pls->scol0(_palette_size, (int)(rgb.r * 255), (int)(rgb.g * 255), (int)(rgb.b * 255));
          return _palette_size++;
        }

      return i->second;
    }

  }

}

