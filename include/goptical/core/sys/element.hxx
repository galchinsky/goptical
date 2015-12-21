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


#ifndef GOPTICAL_ELEMENT_HXX_
#define GOPTICAL_ELEMENT_HXX_

#include "goptical/core/math/transform.hxx"
#include "goptical/core/math/vector_pair.hxx"
#include "goptical/core/math/vector_pair.hxx"

namespace _goptical {

  namespace sys {

    void Element::set_local_position(const math::Vector3 &v)
    {
      _transform.set_translation(v);
      system_moved();
    }

    void Element::rotate(double x, double y, double z)
    {
      _transform.linear_rotation(math::Vector3(x, y, z));
      system_moved();
    }

    const math::Vector3 & Element::get_local_position() const
    {
      return _transform.get_translation();
    }

    const math::Transform<3> & Element::get_transform() const
    {
      return _transform;
    }

    void Element::set_transform(const math::Transform<3> &t)
    {
      _transform = t;
      system_moved();
    }

    unsigned int Element::id() const
    {
      return _system_id;
    }

    system * Element::get_system() const
    {
      return _system;
    }

    unsigned int Element::get_version() const
    {
      return _version;
    }

    bool Element::is_enabled() const
    {
      return _enabled;
    }

    void Element::set_enable_state(bool enabled)
    {
      if (_enabled != enabled)
        update_version();
      _enabled = enabled;
    }

    template <trace::IntensityMode m>
    inline void Element::process_rays(trace::Result &result,
                                     trace::rays_queue_t *input) const
    {
      switch (m)
        {
        case trace::Simpletrace:
          process_rays_simple(result, input);
          break;

        case trace::Intensitytrace:
          process_rays_intensity(result, input);
          break;

        case trace::Polarizedtrace:
          process_rays_polarized(result, input);
          break;
        }
    }

    std::ostream & operator<<(std::ostream &o, const Element &e)
    {
      e.print(o);
      return o;
    }

  }
}

#endif

