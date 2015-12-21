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


#ifndef GOPTICAL_RENDERER_2D_HH_
#define GOPTICAL_RENDERER_2D_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/math/transform.hpp"
#include "goptical/core/io/renderer_viewport.hpp"

namespace _goptical {

  namespace io {

    /**
       @short Base class for 2d rendering drivers
       @header <goptical/core/io/Renderer2d
       @module {Core}
       @internal

       This class provide default implementations for 3d projection
       and 3d drawing primitives. It's designed to be used as a base
       class for 2d only renderers so that they can perform 3d
       rendering too.
     */

    class Renderer2d : public RendererViewport
    {
    protected:
      Renderer2d();

      using Renderer::draw_point;
      using Renderer::draw_segment;
      using Renderer::draw_text;

      /** project in 2d space and scale for ploting to 2d output */
      inline math::Vector2 project_scale(const math::Vector3 &v);

      /** project in 2d space */
      inline math::Vector2 project(const math::Vector3 &v);

      /** @override */
      void set_perspective();
      /** @override */
      void set_orthographic();
      /** @override */
      math::Transform<3> get_camera_transform() const;
      /** @override */
      void set_camera_transform(const math::Transform<3> & t);

      /** @override */
      void draw_point(const math::Vector3 &p, const Rgb &rgb, enum PointStyle s);
      /** @override */
      void draw_segment(const math::VectorPair3 &l, const Rgb &rgb);
      /** @override */
      void draw_text(const math::Vector3 &pos, const math::Vector3 &dir,
                     const std::string &str, TextAlignMask a, int size, const Rgb &rgb);

    private:
      math::Vector2 projection_ortho(const math::Vector3 &v) const;
      math::Vector2 projection_perspective(const math::Vector3 &v) const;

      math::Vector2 (Renderer2d::*_projection)(const math::Vector3 &v) const;

      math::Transform<3> _cam_transform;
      double _eye_dist;
    };

  }
}

#endif

