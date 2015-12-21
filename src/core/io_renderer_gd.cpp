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

#include <algorithm>
#include <iostream>

#include <goptical/core/math/VectorPair>
#include <goptical/core/math/VectorPair>
#include <goptical/core/io/RendererGd>
#include <goptical/core/trace/Result>
#include <goptical/core/trace/Ray>
#include <goptical/core/Error>

namespace _goptical {

  namespace io {

    /**********************************************************************
     * Initialization
     */

    RendererGd::RendererGd(gdImagePtr im, const Rgb &bg,
                           gdFontPtr font, const char *TFfontlist)
      : _user_im(true),
        _filename(0),
        _thickness(1),
        _im(im),
        _font(font),
        _fontlist(TFfontlist)
    {
      _2d_output_res = math::Vector2(gdImageSX(im), gdImageSY(im));

      _styles_color[StyleBackground] = bg;
      _styles_color[StyleForeground] = ~bg;

      if (!_font)
        _font = gdFontGetLarge();
    }

    void RendererGd::init(const Rgb &bg)
    {
      _styles_color[StyleBackground] = bg;
      _styles_color[StyleForeground] = ~bg;

      _im = gdImageCreateTrueColor((int)_2d_output_res.x(), (int)_2d_output_res.y());
      _font = gdFontGetLarge();

      clear();
    }

    RendererGd::RendererGd(const char *filename, unsigned int width,
                           unsigned int height, const Rgb &bg, const char *TFfontlist)
      : _user_im(false),
        _filename(filename),
        _thickness(1),
        _fontlist(TFfontlist)
    {
      _2d_output_res = math::Vector2(width, height);

      init(bg);
    }

    RendererGd::RendererGd(unsigned int width, unsigned int height,
                           const Rgb &bg, const char *TFfontlist)
      : _user_im(false),
        _filename(0),
        _thickness(1),
        _fontlist(TFfontlist)
    {
      _2d_output_res = math::Vector2(width, height);

      init(bg);
    }

    RendererGd::~RendererGd()
    {
      if (_filename)
        write(_filename);
      if (!_user_im)
        gdImageDestroy(_im);
    }

    void RendererGd::set_brush_thickness(unsigned int t)
    {
      _thickness = t;
    }

    void RendererGd::set_2d_size(double width, double height)
    {
      throw Error("Gd renderer doesn't support output size change.");
    }

    /**********************************************************************
     * Colors
     */

    int RendererGd::rgb_to_gdcolor(const Rgb &rgb, float alpha) const
    {
      return gdTrueColorAlpha((int)(rgb.r * 255),
                              (int)(rgb.g * 255),
                              (int)(rgb.b * 255),
                              (int)(alpha * gdAlphaTransparent));
    }

    int RendererGd::rgb_to_gdcolor(const Rgb &rgb) const
    {
      return gdTrueColor((int)(rgb.r * 255),
                         (int)(rgb.g * 255),
                         (int)(rgb.b * 255));
    }

    int RendererGd::style_to_gdcolor(Style style) const
    {
      return rgb_to_gdcolor(get_style_color(style));
    }

    int RendererGd::ray_to_gdcolor(const light::Ray & ray)
    {
      return rgb_to_gdcolor(ray_to_rgb(ray), ray_to_alpha(ray));
    }

    /**********************************************************************
     * light ray drawing
     */

    void RendererGd::draw_ray_line(const math::VectorPair3 &l, const trace::Ray &ray)
    {
      gdImageSetThickness(_im, 1);

      //      gdImageAlphaBlending(_im, 2); // no blending on white background

      math::Vector2 v2da = project_scale(l[0]);
      math::Vector2 v2db = project_scale(l[1]);

      gdImageLine(_im,
                  (int)v2da.x(), (int)v2da.y(),
                  (int)v2db.x(), (int)v2db.y(), 
                  ray_to_gdcolor(ray));
    }

    /**********************************************************************
     * Misc shapes drawing
     */

    void RendererGd::draw_point(const math::Vector2 &v, const Rgb &rgb, enum PointStyle s)
    {
      math::Vector2 v2d(trans_pos(v));
      const unsigned int t = _thickness * 2 + 1;

      gdImageAlphaBlending(_im, 0);

      switch (s)
        {
        case PointStyleDot:
          gdImageSetPixel(_im, (int)v2d.x(), (int)v2d.y(),
                          rgb_to_gdcolor(rgb));
          break;

        case PointStyleCross:

          gdImageLine(_im,
                      (int)v2d.x() - t, (int)v2d.y(),
                      (int)v2d.x() + t, (int)v2d.y(),
                      rgb_to_gdcolor(rgb));
          gdImageLine(_im,
                      (int)v2d.x(), (int)v2d.y() - t,
                      (int)v2d.x(), (int)v2d.y() + t,
                      rgb_to_gdcolor(rgb));
          break;

        case PointStyleRound:
          gdImageArc(_im, (int)v2d.x(), (int)v2d.y(), t*2, t*2, 0, 360, rgb_to_gdcolor(rgb));
          break;

        case PointStyleSquare:
          gdImageLine(_im,
                      (int)v2d.x() - t, (int)v2d.y() + t,
                      (int)v2d.x() + t, (int)v2d.y() + t,
                      rgb_to_gdcolor(rgb));
          gdImageLine(_im,
                      (int)v2d.x() + t, (int)v2d.y() - t,
                      (int)v2d.x() + t, (int)v2d.y() + t,
                      rgb_to_gdcolor(rgb));
          gdImageLine(_im,
                      (int)v2d.x() - t, (int)v2d.y() - t,
                      (int)v2d.x() + t, (int)v2d.y() - t,
                      rgb_to_gdcolor(rgb));
          gdImageLine(_im,
                      (int)v2d.x() - t, (int)v2d.y() - t,
                      (int)v2d.x() - t, (int)v2d.y() + t,
                      rgb_to_gdcolor(rgb));
          break;

        case PointStyleTriangle:
          gdImageLine(_im,
                      (int)v2d.x(), (int)v2d.y() - t,
                      (int)v2d.x() + t, (int)v2d.y() + t,
                      rgb_to_gdcolor(rgb));
          gdImageLine(_im,
                      (int)v2d.x() - t, (int)v2d.y() + t,
                      (int)v2d.x() + t, (int)v2d.y() + t,
                      rgb_to_gdcolor(rgb));
          gdImageLine(_im,
                      (int)v2d.x(), (int)v2d.y() - t,
                      (int)v2d.x() - t, (int)v2d.y() + t,
                      rgb_to_gdcolor(rgb));
          break;

        }
    }

    void RendererGd::draw_segment(const math::VectorPair2 &l, const Rgb &rgb)
    {
      gdImageAlphaBlending(_im, 0);
      gdImageSetThickness(_im, _thickness);

      math::Vector2 v2da = trans_pos(l[0]);
      math::Vector2 v2db = trans_pos(l[1]);

      gdImageLine(_im,
                  (int)v2da.x(), (int)v2da.y(),
                  (int)v2db.x(), (int)v2db.y(), 
                  rgb_to_gdcolor(rgb));
    }

    void RendererGd::draw_circle(const math::Vector2 &c, double r, const Rgb &rgb, bool filled)
    {
      math::Vector2 v2d(trans_pos(c));

      gdImageAlphaBlending(_im, 0);
      gdImageSetThickness(_im, _thickness);
      if (filled)
        gdImageFilledEllipse(_im, (int)v2d.x(), (int)v2d.y(),
                             (int)x_scale(r * 2), (int)y_scale(r * 2),
                             rgb_to_gdcolor(rgb));
      else
        gdImageArc(_im, (int)v2d.x(), (int)v2d.y(),
                   (int)x_scale(r * 2), (int)y_scale(r * 2), 0, 360,
                       rgb_to_gdcolor(rgb));
    }

    void RendererGd::draw_text(const math::Vector2 &c, const math::Vector2 &dir,
                               const std::string &str, TextAlignMask a, int size, const Rgb &rgb)
    {
      const int margin = size / 2;
      math::Vector2 v2d = trans_pos(c);
      int x = (int)v2d.x();
      int y = (int)v2d.y();
      int brect[8];

      gdImageSetThickness(_im, 1);
      gdImageAlphaBlending(_im, 0);

      // try gdImageStringFT

      if (_fontlist)
        {
          double ra = atan2(dir.y(), dir.x());
          void *r;

          r = gdImageStringFT(NULL, brect, rgb_to_gdcolor(rgb), (char*)_fontlist,
                              size, 0, x, y, (char*)str.c_str());

          if (!r)
            {
              double width = brect[2] - brect[0];
              double height = brect[1] - brect[5];

              if (a & TextAlignCenter)
                {
                  x -= width / 2. * dir.x();
                  y -= width / 2. * -dir.y();
                }
              else if (a & TextAlignRight)
                {
                  x -= (width + margin) * dir.x();
                  y -= (width + margin) * -dir.y();
                }
              else
                {
                  x += (margin) * dir.x();
                  y += (margin) * -dir.y();
                }

              if (a & TextAlignMiddle)
                {
                  x += height * -dir.y() / 2.;
                  y += height * dir.x() / 2.;
                }
              else if (a & TextAlignTop)
                {
                  x += height * -dir.y();
                  y += height * dir.x();
                }

              r = gdImageStringFT(_im, 0, rgb_to_gdcolor(rgb), (char*)_fontlist,
                                  size, ra, x, y, (char*)str.c_str());

              if (!r)
                return;
            }
        }

      // fallback to gdImageString

      if (fabs(dir.y()) > fabs(dir.x()))
        {
          if (a & TextAlignLeft)
            x += margin;
          else if (a & TextAlignRight)
            x -= _font->h + margin;
          else
            x -= (_font->h) / 2;

          if (a & TextAlignBottom)
            y -= margin;
          else if (a & TextAlignTop)
            y += _font->w * str.size() + margin;
          else
            y += (_font->w * str.size()) / 2;

          gdImageStringUp(_im, _font, x, y, (unsigned char*)str.c_str(), rgb_to_gdcolor(rgb));
        }
      else
        {
          if (a & TextAlignLeft)
            x += margin;
          else if (a & TextAlignRight)
            x -= _font->w * str.size() + margin;
          else
            x -= (_font->w * str.size()) / 2;

          if (a & TextAlignTop)
            y += margin;
          else if (a & TextAlignBottom)
            y -= _font->h - margin;
          else
            y -= (_font->h) / 2;

          gdImageString(_im, _font, x, y, (unsigned char*)str.c_str(), rgb_to_gdcolor(rgb));
        }
    }

    void RendererGd::draw_polygon(const math::Vector2 *array, unsigned int count,
                                  const Rgb &rgb, bool filled, bool closed)
    {
      if (count < 3)
        return;

      closed |= filled;
      gdPoint p[count];

      for (unsigned int i = 0; i < count; i++)
        {
          math::Vector2 v2d = trans_pos(array[i]);

          p[i].x = (int)v2d.x();
          p[i].y = (int)v2d.y();
        }

      if (filled)
        gdImageFilledPolygon(_im, p, count, rgb_to_gdcolor(rgb));
      else if (closed)
        gdImagePolygon(_im, p, count, rgb_to_gdcolor(rgb));
      else
        gdImageOpenPolygon(_im, p, count, rgb_to_gdcolor(rgb));
    }

    /**********************************************************************
     * Global image operations
     */

    void RendererGd::clear()
    {
      gdImageAlphaBlending(_im, 0);
      gdImageFilledRectangle(_im, 0, 0, (int)_2d_output_res.x() - 1, (int)_2d_output_res.y() - 1,
                             rgb_to_gdcolor(rgb_background()));
    }

    void RendererGd::write(const std::string & filename)
    {
      FILE *out = fopen(filename.c_str(), "wb");
      gdImagePng(_im, out);
      fclose(out);
      _filename = 0;
    }

  }

}

