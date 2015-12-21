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

#ifndef GOPTICAL_RENDERER_SVG_HH_
#define GOPTICAL_RENDERER_SVG_HH_

#include <sstream>

#include "goptical/core/common.hpp"

#include "goptical/core/io/renderer_2d.hpp"

namespace _goptical {

  namespace io {

    /**
       @short SVG file rendering driver
       @header <goptical/core/io/RendererSvg
       @module {Core}
       @main

       This class implements a SVG graphic output driver.
     */
    class RendererSvg : public Renderer2d
    {
    public:

      /** Create a new svg renderer with given resolution. The
          @ref write function must be used to write svg to output
          stream. */
      RendererSvg(double width = 800, double height = 600,
                  const Rgb &background = rgb_white);

      /** Create a new svg renderer with given resolution and
          viewport window. Svg output will be written to given
          filename when the renderer object is destroyed. */
      RendererSvg(const char *filename, double width = 800, double height = 600,
                  const Rgb &background = rgb_white);

      ~RendererSvg();

      /** Write svg output to given stream. */
      void write(std::ostream &s);

    private:
      /** @override */
      void clear();

      /** @override */
      void draw_point(const math::Vector2 &p, const Rgb &rgb, enum PointStyle s);
      /** @override */
      void draw_segment(const math::VectorPair2 &l, const Rgb &rgb);
      /** @override */
      void draw_circle(const math::Vector2 &c, double r, const Rgb &rgb, bool filled);
      /** @override */
      void draw_text(const math::Vector2 &pos, const math::Vector2 &dir,
                     const std::string &str, TextAlignMask a, int size, const Rgb &rgb);
      /** @override */
      void draw_polygon(const math::Vector2 *array, unsigned int count, const Rgb &rgb, bool filled, bool closed);

      /** @override */
      void draw_plot_data_2d(const data::Set1d &data, const data::Plotdata &style);

      /** @override */
      void group_begin(const std::string &name);
      /** @override */
      void group_end();

      void write_srgb(const Rgb & rgb);

      void svg_begin_line(double x1, double y1, double x2, double y2, bool terminate = false);
      void svg_begin_rect(double x1, double y1, double x2, double y2, bool terminate = false);
      void svg_begin_ellipse(double x, double y, double rx, double ry, bool terminate = false);
      void svg_begin_use(const std::string &id, double x, double y, bool terminate = false);
      void svg_add_stroke(const Rgb & rgb);
      void svg_add_fill(const Rgb & rgb);
      void svg_add_id(const std::string &id);
      void svg_end();

      inline double y_trans_pos(double y) const;
      inline math::Vector2 trans_pos(const math::Vector2 &v);

      std::ostringstream _out;
      const char *_filename;
    };

  }
}

#endif

