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


#ifndef GOPTICAL_RENDERER_AXES_HXX_
#define GOPTICAL_RENDERER_AXES_HXX_

#include <cassert>

namespace _goptical {

  namespace io {

    const math::Vector3 & RendererAxes::get_position() const
    {
      return _pos;
    }

    const math::Vector3 & RendererAxes::get_origin() const
    {
      return _origin;
    }

    bool RendererAxes::get_show_grid() const
    {
      return _grid;
    }

    bool RendererAxes::get_show_frame() const
    {
      return _frame;
    }

    bool RendererAxes::get_show_axes(unsigned int i) const
    {
      return _axes[i]._axis;
    }

    bool RendererAxes::get_show_tics(unsigned int i) const
    {
      return _axes[i]._tics;
    }

    bool RendererAxes::get_show_values(unsigned int i) const
    {
      return _axes[i]._values;
    }

    const std::string & RendererAxes::get_label(unsigned int i) const
    {
      return _axes[i]._label;
    }

  }

};

#endif

