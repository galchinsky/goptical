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


#ifndef GOPTICAL_DATA_PLOTDATA_HXX_
#define GOPTICAL_DATA_PLOTDATA_HXX_

#include "goptical/core/data/set.hxx"

namespace _goptical {

  namespace data {

    Plotdata::Plotdata(const const_ref<Set> &s)
      : _set(s),
        _color(io::rgb_red),
        _style(InterpolatePlot | PointPlot),
        _label("")
    {
    }

    const Set & Plotdata::get_set() const
    {
      return *_set;
    }

    void Plotdata::set_label(const std::string & label)
    {
      _label = label;
    }

    const std::string & Plotdata::get_label() const
    {
      return _label;
    }

    void Plotdata::set_color(const io::Rgb & color)
    {
      _color = color;
    }

    const io::Rgb & Plotdata::get_color() const
    {
      return _color;
    }

    void Plotdata::enable_style(PlotStyleMask style)
    {
      _style = _style | style;
    }

    void Plotdata::disable_style(PlotStyleMask style)
    {
      _style = _style & ~style;
    }

    void Plotdata::set_style(PlotStyleMask style)
    {
      _style = style;
    }

    PlotStyleMask Plotdata::get_style() const
    {
      return _style;
    }
  }
}

#endif

