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


#ifndef GOPTICAL_CURVE_FOUCAULT_HXX_
#define GOPTICAL_CURVE_FOUCAULT_HXX_

#include "goptical/core/curve/rotational.hxx"
#include "curve_roc.hxx"
#include "goptical/core/data/discrete_set.hxx"

namespace _goptical {

  namespace curve {

    void Foucault::set_moving_source(double offset)
    {
      _updated = false;
      _moving_source = true;
      _offset = offset;
      clear();
    }

    void Foucault::set_fixed_source(double source_to_surface)
    {
      _updated = false;
      _moving_source = false;
      _offset = source_to_surface;
      clear();
    }

    void Foucault::set_radius(double radius)
    {
      _updated = false;
      _radius = radius;
    }

    double Foucault::get_radius() const
    {
      return _radius;
    }

    void Foucault::set_ode_stepsize(double step)
    {
      _updated = false;
      _ode_step = step;
    }

    unsigned int Foucault::get_zones_count() const
    {
      return _reading.get_count();
    }

    void Foucault::set_knife_offset(unsigned int zone_number, double  knife_offset)
    {
      _reading.get_y_value(zone_number) = knife_offset;
    }

    const std::pair<double, double> Foucault::get_reading(unsigned int index) const
    {
      std::pair<double, double> r;

      r.first = _reading.get_x_value(index);
      r.second = _reading.get_y_value(index) - _roc;

      return r;
    }

  }
}

#endif

