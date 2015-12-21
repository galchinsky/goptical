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


#ifndef GOPTICAL_RENDERER_X3D_HH_
#define GOPTICAL_RENDERER_X3D_HH_

#include <sstream>

#include "goptical/core/common.hpp"

#include "goptical/core/io/renderer.hpp"

namespace _goptical {

  namespace io {

    /**
       @short X3d rendering driver
       @header <goptical/core/io/RendererX3d
       @module {Core}
       @experimental
       @main

       This class implements an @url {http://en.wikipedia.org/wiki/X3D}
       {X3d} graphic output driver.
     */
    class RendererX3d : public Renderer
    {
    public:

      /** Create a new X3d renderer. The @ref write function must be
          used to write x3d to output stream. */
      RendererX3d(const Rgb &background = rgb_white);

      /** Create a new X3d renderer. X3d output will be written to
          given filename when the renderer object is destroyed. */
      RendererX3d(const char *filename, const Rgb &background = rgb_white);

      ~RendererX3d();

      /** Write svg output to given stream. */
      void write(std::ostream &s);

      void clear();
      void flush();

    private:

      /** @override */
      void draw_point(const math::Vector2 &p, const Rgb &rgb, enum PointStyle s);
      /** @override */
      void draw_segment(const math::VectorPair2 &l, const Rgb &rgb);
      /** @override */
      void draw_circle(const math::Vector2 &v, double r, const Rgb &rgb, bool filled);
      /** @override */
      void draw_polygon(const math::Vector2 *array, unsigned int count, const Rgb &rgb, bool filled, bool closed);

      /** @override */
      void draw_point(const math::Vector3 &p, const Rgb &rgb, enum PointStyle s);
      /** @override */
      void draw_segment(const math::VectorPair3 &l, const Rgb &rgb);
      /** @override */
      void draw_polygon(const math::Vector3 *array, unsigned int count, const Rgb &rgb, bool filled, bool closed);
      /** @override */
      void draw_triangle(const math::Triangle<3> &t, const Rgb &rgb);
      /** @override */
      void draw_triangle(const math::Triangle<3> &t, const math::Triangle<3> &gradient, const Rgb &rgb);

      /** @override */
      void draw_text(const math::Vector3 &pos, const math::Vector3 &dir,
                     const std::string &str, TextAlignMask a, int size, const Rgb &rgb);

      /** @override */
      void draw_text(const math::Vector2 &pos, const math::Vector2 &dir,
                     const std::string &str, TextAlignMask a, int size, const Rgb &rgb);

      /** @override */
      void group_begin(const std::string &name);
      /** @override */
      void group_end();

      void write_appearance(const Rgb &rgb, const char *type);

      std::ostringstream _out;
      const char *_filename;

      bool _xml_header;
      bool _x3d_header;
    };

  }
}

#endif

