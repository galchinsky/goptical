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

#include <fstream>

#include <goptical/core/io/RendererSvg>
#include <goptical/core/math/VectorPair>
#include <goptical/core/data/PlotData>
#include <goptical/core/data/Plot>
#include <goptical/core/data/Set1d>

namespace _goptical {

  namespace io {

    RendererSvg::RendererSvg(double width, double height, const Rgb &bg)
      : _filename(0)
    {
      _2d_output_res = math::Vector2(width, height);

      _styles_color[StyleBackground] = bg;
      _styles_color[StyleForeground] = ~bg;

      clear();
    }

    RendererSvg::RendererSvg(const char *filename, double width, double height,
                             const Rgb &bg)
      : _filename(filename)
    {
      _2d_output_res = math::Vector2(width, height);

      // FIXME handle background alpha
      _styles_color[StyleBackground] = bg;
      _styles_color[StyleForeground] = ~bg;

      clear();
    }

    RendererSvg::~RendererSvg()
    {
      if (_filename)
        {
          std::ofstream file(_filename);
          write(file);
        }
    }

    void RendererSvg::group_begin(const std::string &name)
    {
      _out << "<g>";
      if (!name.empty())
        _out << "<title>"+name+"</title>";
      _out << std::endl;
    }

    void RendererSvg::group_end()
    {
      _out << "</g>" << std::endl;
    }

    void RendererSvg::write(std::ostream &s)
    {
      s << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;

      s << "<svg width=\"" << _2d_output_res.x()
        << "px\" height=\"" <<  _2d_output_res.y() << "px\" "
        << "version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">" << std::endl;

      // content
      s << _out.str();

      s << "</svg>" << std::endl;
    }

    void RendererSvg::write_srgb(const Rgb & rgb)
    {
      char str[8];

      snprintf(str, 8, "#%02x%02x%02x",
               (unsigned char)(rgb.r * 255.0),
               (unsigned char)(rgb.g * 255.0),
               (unsigned char)(rgb.b * 255.0));

      _out << str;
    }

    void RendererSvg::svg_begin_line(double x1, double y1, double x2, double y2, bool terminate)
    {
      _out << "<line "
           << "x1=\"" << (x1) << "\" "
           << "y1=\"" << (y1) << "\" "
           << "x2=\"" << (x2) << "\" "
           << "y2=\"" << (y2) << "\" ";

      if (terminate)
        _out << " />" << std::endl;
    }

    void RendererSvg::svg_begin_rect(double x1, double y1, double x2, double y2, bool terminate)
    {
      _out << "<rect "
           << "x=\"" << (x1) << "\" "
           << "y=\"" << (y1) << "\" "
           << "width=\"" << (x2 - x1) << "\" "
           << "height=\"" << (y2 - y1) << "\" ";

      if (terminate)
        _out << " />" << std::endl;
    }

    void RendererSvg::svg_begin_ellipse(double x, double y, double rx, double ry, bool terminate)
    {
      _out << "<ellipse "
           << "cx=\"" << x << "\" "
           << "cy=\"" << y << "\" "
           << "rx=\"" << rx << "\" "
           << "ry=\"" << ry << "\" ";

      if (terminate)
        _out << " />" << std::endl;
    }

    void RendererSvg::svg_begin_use(const std::string &id, double x, double y, bool terminate)
    {
      _out << "<use "
           << "x=\"" << x << "\" "
           << "y=\"" << y << "\" "
           << "xlink:href=\"#" << id << "\" ";

      if (terminate)
        _out << " />" << std::endl;
    }

    void RendererSvg::svg_add_stroke(const Rgb & rgb)
    {
      _out << " stroke=\"";
      write_srgb(rgb);
      _out << "\"";
    }

    void RendererSvg::svg_add_fill(const Rgb & rgb)
    {
      _out << " fill=\"";
      write_srgb(rgb);
      _out << "\"";
    }

    void RendererSvg::svg_add_id(const std::string &id)
    {
      _out << " fill=\"" << id << "\"";
    }

    void RendererSvg::svg_end()
    {
      _out << " />" << std::endl;
    }

    void RendererSvg::draw_plot_data_2d(const data::Set1d &data, const data::Plotdata &style)
    {
      // use SVG bezier curve for interpolated data plot
      if (style.get_style() & data::InterpolatePlot)
        {
          _out << "<path fill=\"none\" ";

          svg_add_stroke(style.get_color());

          std::pair<double, double> p0, p1, p2, p3;

          p0.first = data.get_x_value((unsigned int)0);
          p0.second = data.get_y_value((unsigned int)0);

          p1 = p0;

          p2.first = data.get_x_value(1);
          p2.second = data.get_y_value(1);

          _out << " d=\"M" << x_trans_pos(p1.first) << "," << y_trans_pos(p1.second) << " ";

          for (unsigned int j = 1; j < data.get_count(); j++)
            {
              if (j + 1 < data.get_count())
                {
                  p3.first = data.get_x_value(j + 1);
                  p3.second = data.get_y_value(j + 1);
                }
              else
                {
                  p3 = p2;
                }

              _out << "S"
                   << x_trans_pos(p2.first - (p3.first - p1.first) / 6) << ","
                   << y_trans_pos(p2.second - (p3.second - p1.second) / 6) << " "
                   << x_trans_pos(p2.first) << "," << y_trans_pos(p2.second) << " ";

              p0 = p1;
              p1 = p2;
              p2 = p3;
            }

          _out << "\" />" << std::endl;
        }

      // plot other styles using the default methods
      data::Plotdata other(style);
      other.disable_style(data::InterpolatePlot);
      RendererViewport::draw_plot_data_2d(data, other);
    }

    void RendererSvg::draw_point(const math::Vector2 &p, const Rgb &rgb, enum PointStyle s)
    {
      const char *id[5] = { "dot", "cross", "round", "square", "triangle" };

      if (s >= 5)
        s = PointStyleCross;

      math::Vector2 v2d = trans_pos(p);

      svg_begin_use(id[s], v2d.x(), v2d.y());
      svg_add_stroke(rgb);
      svg_end();
    }

    void RendererSvg::draw_segment(const math::VectorPair2 &l, const Rgb &rgb)
    {
      math::Vector2 v2da = trans_pos(l[0]);
      math::Vector2 v2db = trans_pos(l[1]);

      svg_begin_line(v2da.x(), v2da.y(), v2db.x(), v2db.y());
      svg_add_stroke(rgb);
      svg_end();
    }

    void RendererSvg::draw_circle(const math::Vector2 &c, double r,
                                  const Rgb &rgb, bool filled)
    {
      math::Vector2 v2d = trans_pos(c);

      svg_begin_ellipse(v2d.x(), v2d.y(), x_scale(r), y_scale(r));
      svg_add_stroke(rgb);
      if (filled)
        svg_add_fill(rgb);
      else
        _out << " fill=\"none\"";
      svg_end();
    }

    void RendererSvg::draw_text(const math::Vector2 &v, const math::Vector2 &dir,
                                const std::string &str, TextAlignMask a, int size, const Rgb &rgb)
    {
      const int margin = size / 2;
      math::Vector2 v2d = trans_pos(v);
      double x = v2d.x();
      double y = v2d.y();
      double yo = y, xo = x;

      _out << "<text style=\"font-size:" << size << ";";

      if (a & TextAlignLeft)
        {
          //_out << "text-align:left;text-anchor:start;";
          x += margin;
        }
      else if (a & TextAlignRight)
        {
          _out << "text-align:right;text-anchor:end;";
          x -= margin;
        }
      else
        _out << "text-align:center;text-anchor:middle;";

      if (a & TextAlignTop)
        y += size + margin;
      else if (a & TextAlignBottom)
        y -= margin;
      else
        y += size / 2;

      _out << "\" x=\"" << x << "\" y=\"" << y << "\"";

      double ra = math::rad2degree(atan2(-dir.y(), dir.x()));
      if (ra != 0)
        _out << " transform=\"rotate(" << ra << "," << xo << "," << yo << ")\"";

      svg_add_fill(rgb);

      _out << ">" << str << "</text>" << std::endl;
    }

    void RendererSvg::draw_polygon(const math::Vector2 *array, unsigned int count, const Rgb &rgb, bool filled, bool closed)
    {
      if (count < 3)
        return;

      closed |= filled;

      if (closed)
        {
          _out << "<polygon";

          if (filled)
            svg_add_fill(rgb);
          else
            {
              _out << " fill=\"none\"";
              svg_add_stroke(rgb);
            }
        }
      else
        {
          _out << "<polyline fill=\"none\"";

          svg_add_stroke(rgb);
        }

      _out << " points=\"";

      for (unsigned int i = 0; i < count; i++)
        {
          math::Vector2 v2d = trans_pos(array[i]);

          _out << v2d.x() << "," << v2d.y() << " ";
        }

      _out << "\" />" << std::endl;
    }

    void RendererSvg::clear()
    {
      _out.str("");

      // background
      svg_begin_rect(0, 0,  _2d_output_res.x(),  _2d_output_res.y());
      svg_add_fill(get_style_color(StyleBackground));
      svg_end();

      _out << "<defs>" << std::endl;

      // dot shaped point
      _out << "<g id=\"" << "dot" << "\">" << std::endl;
      svg_begin_line(1, 1, 0, 0, true);
      _out << "</g>" << std::endl;      

      // cross shaped point
      _out << "<g id=\"" << "cross" << "\">" << std::endl;
      svg_begin_line(-3, 0, 3, 0, true);
      svg_begin_line(0, -3, 0, 3, true);
      _out << "</g>" << std::endl;      

      // square shaped point
      _out << "<g id=\"" << "square" << "\">" << std::endl;
      svg_begin_line(-3, -3, -3, 3, true);
      svg_begin_line(-3, 3, 3, 3, true);
      svg_begin_line(3, 3, 3, -3, true);
      svg_begin_line(3, -3, -3, -3, true);
      _out << "</g>" << std::endl;      

      // round shaped point
      _out << "<g id=\"" << "round" << "\">" << std::endl;
      svg_begin_ellipse(0, 0, 3, 3, false);
      _out << " fill=\"none\" />";
      _out << "</g>" << std::endl;      

      // triangle shaped point
      _out << "<g id=\"" << "triangle" << "\">" << std::endl;
      svg_begin_line(0, -3, -3, 3, true);
      svg_begin_line(-3, 3, 3, 3, true);
      svg_begin_line(0, -3, +3, +3, true);
      _out << "</g>" << std::endl;      

      _out << "</defs>" << std::endl;

    }

  }

}

