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


#ifndef GOPTICAL_RENDERER_HXX_
#define GOPTICAL_RENDERER_HXX_

#include <cassert>
#include <algorithm>

#include "goptical/core/math/vector.hxx"
#include "goptical/core/math/triangle.hxx"
#include "goptical/core/math/vector_pair.hxx"
#include "goptical/core/io/rgb.hxx"

namespace _goptical {

  namespace io {

    Renderer::~Renderer()
    {
    }

    void Renderer::set_ray_color_mode(RayColorMode m)
    {
      _ray_color_mode = m;
    }

    void Renderer::set_ray_intensity_mode(IntensityMode m)
    {
      _intensity_mode = m;
    }

    const Rgb & Renderer::rgb_foreground() const
    {
      return _styles_color[StyleForeground];
    }

    const Rgb & Renderer::rgb_background() const
    {
      return _styles_color[StyleBackground];
    }

    const Rgb & Renderer::get_style_color(Style s) const
    {
      assert(s < StyleLast);
      return _styles_color[s];
    }

    void Renderer::set_style_color(Style s, const Rgb &rgb)
    {
      assert(s < StyleLast);
      _styles_color[s] = rgb;
    }

    void Renderer::draw_segment(const math::Vector2 &a, const math::Vector2 &b,
                                const Rgb &rgb)
    {
      draw_segment(math::VectorPair2(a, b), rgb);
    }

    void Renderer::draw_segment(const math::Vector3 &a, const math::Vector3 &b,
                                const Rgb &rgb)
    {
      draw_segment(math::VectorPair3(a, b), rgb);
    }

  }

}

#endif

