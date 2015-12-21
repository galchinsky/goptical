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


#ifndef GOPTICAL_DATA_PLOT_HXX_
#define GOPTICAL_DATA_PLOT_HXX_

#include <cassert>

namespace _goptical {

  namespace data {

    const std::string & Plot::get_title() const
    {
      return _title;
    }

    unsigned int Plot::get_plot_count() const
    {
      return _plots.size();
    }

    io::RendererAxes & Plot::get_axes()
    {
      return _axes;
    }

    const io::RendererAxes & Plot::get_axes() const
    {
      return _axes;
    }

    Plotdata & Plot::get_plot_data(unsigned int index)
    {
      assert(index < _plots.size());
      return _plots[index];
    }

    const Plotdata & Plot::get_plot_data(unsigned int index) const
    {
      assert(index < _plots.size());
      return _plots[index];
    }

    void Plot::set_xy_swap(bool doswap)
    {
      _xy_swap = doswap;
    }

    bool Plot::get_xy_swap() const
    {
      return _xy_swap;
    }

  }

}

#endif

