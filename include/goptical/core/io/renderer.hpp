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


#ifndef GOPTICAL_RENDERER_HH_
#define GOPTICAL_RENDERER_HH_

#include <string>

#include "goptical/core/common.hpp"

#include "goptical/core/math/vector.hpp"
#include "goptical/core/math/triangle.hpp"
#include "goptical/core/math/vector_pair.hpp"
#include "goptical/core/io/rgb.hpp"

namespace _goptical {

  namespace io {

    /**
       @short Base class for rendering drivers
       @header <goptical/core/io/Renderer
       @module {Core}
       @main

       This class define the interface for graphical rendering drivers
       and provide a default implementation for some functions.
     */

    class Renderer
    {
    public:

      Renderer();

      virtual inline ~Renderer();

      GOPTICAL_ACCESSORS(double, feature_size, "size of lines and triangles used to render curved shapes.");

      /** Set color mode for light ray drawing. Default is @ref
          RayColorWavelen. */
      inline void set_ray_color_mode(RayColorMode m);

      /** Set light ray intensity display mode */
      inline void set_ray_intensity_mode(IntensityMode m);

      /** Get foreground rgb color */
      inline const Rgb & rgb_foreground() const;

      /** Get foreground rgb color */
      inline const Rgb & rgb_background() const;

      /** Get rgb color from style specifier */
      inline const Rgb & get_style_color(Style s) const;

      /** Set rgb color for given style specifier. @pb Some drivers do not
          support changing background color after initialization.  */
      inline void set_style_color(Style s, const Rgb &rgb);

      ////////////// Internal stuff

      /** @internal Draw tangential rays found in trace result using 2d primitives. */
      virtual void draw_trace_result_2d(const trace::Result &result, bool hit_image, const sys::Element *ref);
      /** @internal Draw rays found in trace result using 3d primitives. */
      virtual void draw_trace_result_3d(const trace::Result &result, bool hit_image, const sys::Element *ref);
      /** @internal Draw all optical elements in container using 2d primitives. */
      virtual void draw_element_2d(const sys::Element &e, const sys::Element *ref);
      /** @internal Draw all optical elements in container using 3d primitives. */
      virtual void draw_element_3d(const sys::Element &e, const sys::Element *ref);
      /** @internal Draw point corresponding to ray intercepts on a surface */
      virtual void draw_intercepts(const trace::Result &result, const sys::Surface &s);

      /** @internal ray line drawing in global coordinate, called by @ref draw_traced_ray_recurs */
      virtual void draw_ray_line(const math::VectorPair3 &l, const trace::Ray &ray);
      /** @internal ray line drawing in global coordinate, called by @ref draw_traced_ray_recurs */
      virtual void draw_ray_line(const math::VectorPair2 &l, const trace::Ray &ray);

      /** @internal Draw a point in 2d */
      virtual void draw_point(const math::Vector2 &p, const Rgb &rgb = rgb_gray, enum PointStyle s = PointStyleDot) = 0;
      /** @internal Draw a line segment in 2d */
      virtual void draw_segment(const math::VectorPair2 &s, const Rgb &rgb = rgb_gray) = 0;
      /** @internal Draw a line segment in 2d */
      inline void draw_segment(const math::Vector2 &a, const math::Vector2 &b, const Rgb &rgb = rgb_gray);
      /** @internal Draw a circle in 2d */
      virtual void draw_circle(const math::Vector2 &v, double r, const Rgb &rgb = rgb_gray, bool filled = false);
      /** @internal Draw text in 2d */
      virtual void draw_text(const math::Vector2 &pos, const math::Vector2 &dir, const std::string &str,
                             TextAlignMask a, int size, const Rgb &rgb = rgb_gray) = 0;
      /** @internal Draw polygon in 2d */
      virtual void draw_polygon(const math::Vector2 *array, unsigned int count, const Rgb &rgb = rgb_gray, bool filled = false, bool closed = true);
      /** @internal Draw a box in 2d */
      void draw_box(const math::VectorPair2 &corners, const Rgb &rgb = rgb_gray);
      /** @internal Draw a triangle in 2d */
      void draw_triangle(const math::Triangle<2> &t, bool filled = false, const Rgb &rgb = rgb_gray);

      /** @internal Draw a point in 3d */
      virtual void draw_point(const math::Vector3 &p, const Rgb &rgb = rgb_gray, enum PointStyle s = PointStyleDot) = 0;
      /** @internal Draw a line segment in 3d */
      virtual void draw_segment(const math::VectorPair3 &l, const Rgb &rgb = rgb_gray) = 0;
      /** @internal Draw a line segment in 3d */
      inline void draw_segment(const math::Vector3 &a, const math::Vector3 &b, const Rgb &rgb = rgb_gray);
      /** @internal Draw polygon in 3d */
      virtual void draw_polygon(const math::Vector3 *array, unsigned int count, const Rgb &rgb = rgb_gray, bool filled = false, bool closed = true);
      /** @internal Draw text in 3d */
      virtual void draw_text(const math::Vector3 &pos, const math::Vector3 &dir,
                             const std::string &str, TextAlignMask a, int size, const Rgb &rgb = rgb_gray) = 0;

      /** @internal Draw filled triangle in 3d */
      virtual void draw_triangle(const math::Triangle<3> &t, const Rgb &rgb);
      /** @internal Draw filled triangle in 3d */
      virtual void draw_triangle(const math::Triangle<3> &t, const math::Triangle<3> &gradient, const Rgb &rgb);

      /** @internal Get alpha channel from ray intensity and intensity mode */
      float ray_to_alpha(const light::Ray & ray) const;
      /** @internal Get rgb color from ray wavalen and ray color mode */
      const Rgb ray_to_rgb(const light::Ray & ray);

      /** @internal Begin group */
      virtual void group_begin(const std::string &name = "");
      /** @internal End group */
      virtual void group_end();

    protected:

      void init_styles();

      double _feature_size;

      Rgb               _styles_color[StyleLast];
      RayColorMode      _ray_color_mode;
      IntensityMode     _intensity_mode;
      float             _max_intensity; // max ray intensity updated on

    private:

      template <unsigned D>
      void draw_trace_result(const trace::Result &result, const sys::Element *ref,
                             bool hit_image);
      template <unsigned D, bool draw_lost>
      bool draw_traced_ray_recurs(const trace::Ray &ray, double lost_len,
                                  const sys::Element *ref, bool hit_image);
    };

  }
}

#endif

