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


#ifndef GOPTICAL_RENDERER_VIEWPORT_HXX_
#define GOPTICAL_RENDERER_VIEWPORT_HXX_

#include "goptical/core/math/transform.hxx"
#include "goptical/core/io/renderer.hxx"

namespace _goptical {

  namespace io {

    void RendererViewport::set_numeric_format(std::ios_base::fmtflags format, unsigned int precision)
    {
      _precision = precision;
      _format = format;
    }

    const math::VectorPair2 & RendererViewport::get_window() const
    {
      return _window2d_fit;
    }

    double RendererViewport::x_scale(double x) const
    {
      return ((x / (_page[1].x() - _page[0].x())) * _2d_output_res.x());
    }

    double RendererViewport::y_scale(double y) const
    {
      return ((y / (_page[1].y() - _page[0].y())) * _2d_output_res.y());
    }

    double RendererViewport::x_trans_pos(double x) const
    {
      return x_scale(x - _page[0].x());
    }

    double RendererViewport::y_trans_pos(double y) const
    {
      return y_scale(y - _page[0].y());
    }

    void RendererViewport::set_margin_output(double width, double height)
    {
      set_margin_output(width, height, width, height);
    }

    void RendererViewport::set_margin(double width, double height)
    {
      set_margin(width, height, width, height);
    }

    void RendererViewport::set_margin_ratio(double width, double height)
    {
      set_margin_ratio(width, height, width, height);
    }

  }
}

#endif

