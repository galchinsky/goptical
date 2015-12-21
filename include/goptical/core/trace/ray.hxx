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


#ifndef GOPTICAL_TRACEDRAY_HXX_
#define GOPTICAL_TRACEDRAY_HXX_

#include <limits>

#include "goptical/core/light/ray.hxx"
#include "goptical/core/math/vector.hxx"

#include "goptical/core/sys/element.hpp"
#include "goptical/core/sys/element.hxx"
#include "goptical/core/sys/image.hpp"

#include "goptical/core/math/transform.hpp"
#include "goptical/core/math/transform.hxx"

namespace _goptical {

  namespace trace {

    Ray::Ray()
      : light::Ray(),
        _len(std::numeric_limits<double>::max()),
        _creator(0),
        _parent(0),
        _child(0),
        _lost(true)
    {
    }

    Ray::Ray(const light::Ray &r)
      : light::Ray(r),
        _len(std::numeric_limits<double>::max()),
        _creator(0),
        _parent(0),
        _child(0),
        _lost(true)
    {
    }

    void Ray::add_generated(Ray *r)
    {
      assert(!r->_parent);
      r->_parent = this;
      r->_next = _child;
      _child = r;
    }

    void Ray::set_intercept(const sys::Element &e, const math::Vector3 &point)
    {
      _i_element = (sys::Element*)&e;
      _point = point;

      _lost = false;
    }

    Ray * Ray::get_parent() const
    {
      return _parent;
    }

    Ray * Ray::get_next_child() const
    {
      return _next;
    }

    Ray * Ray::get_first_child() const
    {
      return _child;
    }

    const math::Vector3 & Ray::get_intercept_point() const
    {
      return _point;
    }

    bool Ray::is_lost() const
    {
      return _lost;
    }

    sys::Element & Ray::get_intercept_element() const
    {
      return *_i_element;
    }

    math::Vector3 Ray::get_position(const sys::Element &e) const
    {
      return _creator->get_transform_to(e).transform(origin());
    }

    math::Vector3 Ray::get_direction(const sys::Element &e) const
    {
      return _creator->get_transform_to(e).transform_linear(direction());
    }

    math::Vector3 Ray::get_position() const
    {
      return _creator->get_global_transform().transform(origin());
    }

    math::Vector3 Ray::get_direction() const
    {
      return _creator->get_global_transform().transform_linear(direction());
    }

  }
}

#endif

