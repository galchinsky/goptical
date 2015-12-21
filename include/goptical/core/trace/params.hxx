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


#ifndef GOPTICAL_TRACER_PARAMS_HXX_
#define GOPTICAL_TRACER_PARAMS_HXX_

#include "goptical/core/trace/result.hxx"
#include "goptical/core/trace/distribution.hxx"
#include "goptical/core/trace/sequence.hxx"

namespace _goptical {

  namespace trace {

    Params::Params()
      : _default_distribution(),
        _s_distribution(),
        _max_bounce(50),
        _intensity_mode(Simpletrace),
        _sequential_mode(false),
        _propagation_mode(RayPropagation),
        _unobstructed(false),
        _lost_ray_length(1000)
    {
    }

    void Params::set_nonsequential_mode()
    {
      _sequential_mode = false;
    }

    void Params::set_sequential_mode(const const_ref<Sequence> &seq)
    {
      _sequential_mode = true;
      _sequence = seq;
    }

    bool Params::is_sequential() const
    {
      return _sequential_mode;
    }

    void Params::set_distribution(const sys::Surface &s, const Distribution &dist)
    {
      _s_distribution[&s] = dist;
    }

    void Params::reset_distribution()
    {
      _s_distribution.clear();
    }

    const Distribution & Params::get_distribution(const sys::Surface &s) const
    {
      _s_distribution_map_t::const_iterator i = _s_distribution.find(&s);

      return i == _s_distribution.end() ? _default_distribution : i->second;
    }

  }
}

#endif

