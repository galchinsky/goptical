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


#include <goptical/core/sys/System>
#include <goptical/core/sys/Element>

#include <goptical/core/trace/Ray>
#include <goptical/core/trace/Result>

#include <goptical/core/math/Vector>
#include <goptical/core/math/VectorPair>

#include <goptical/core/io/Renderer>

namespace _goptical {

  namespace trace {

    Result::Result()
      : _rays(),
        _elements(),
        _wavelengths(),
        _generated_queue(0),
        _sources(),
        _bounce_limit_count(0),
        _system(0)
    {
    }

    Result::~Result()
    {
      clear();
    }

    void Result::clear_save_states()
    {
      GOPTICAL_FOREACH(i, _elements)
        {
          i->_save_intercepted_list = false;
          i->_save_generated_list = false;
        }
    }

    void Result::clear()
    {
      GOPTICAL_FOREACH(i, _elements)
        {
          if (i->_intercepted)
            {
              delete i->_intercepted;
              i->_intercepted = 0;
            }

          if (i->_generated)
            {
              delete i->_generated;
              i->_generated = 0;
            }
        }

      _rays.clear();
      _sources.clear();
      _wavelengths.clear();

      _bounce_limit_count = 0;
    }

    void Result::prepare()
    {
      clear();

      GOPTICAL_FOREACH(i, _elements)
        {
          if (i->_save_intercepted_list)
            i->_intercepted = new rays_queue_t;

          if (i->_save_generated_list)
            i->_generated = new rays_queue_t;
        }
    }

    void Result::init(const sys::system &system)
    {
      static const struct element_result_s er = { 0 };

      if (!_system)
        _system = &system;

      if(_system != &system)
        throw Error("trace::Result used with multiple sys::system objects");

      _elements.resize(system.get_element_count(), er);
    }

    void Result::init(const sys::Element &element)
    {
      const sys::system *system = element.get_system();

      assert(system != 0);

      init(*system);
    }

    void Result::set_intercepted_save_state(const sys::Element &e, bool enabled)
    {
      init(e);
      get_element_result(e)._save_intercepted_list = enabled;
    }

    void Result::set_generated_save_state(const sys::Element &e, bool enabled)
    {
      init(e);
      get_element_result(e)._save_generated_list = enabled;
    }

    bool Result::get_intercepted_save_state(const sys::Element &e)
    {
      return get_element_result(e)._save_intercepted_list;
    }

    bool Result::get_generated_save_state(const sys::Element &e)
    {
      return get_element_result(e)._save_generated_list;
    }


    math::VectorPair3 Result::get_intercepted_window(const sys::Surface &s) const
    {
      const rays_queue_t & intercepts = get_intercepted(s);

      if (intercepts.empty())
        throw Error("no ray intercepts found on the surface");

      const math::Vector3 &first = (*intercepts.begin())->get_intercept_point();
      math::VectorPair3 window(first, first);

      GOPTICAL_FOREACH(i, intercepts)
        {
          const math::Vector3 &ip = (*i)->get_intercept_point();

          if (window[0].x() > ip.x())
            window[0].x() = ip.x();
          else if (window[1].x() < ip.x())
            window[1].x() = ip.x();

          if (window[0].y() > ip.y())
            window[0].y() = ip.y();
          else if (window[1].y() < ip.y())
            window[1].y() = ip.y();

          if (window[0].z() > ip.z())
            window[0].z() = ip.z();
          else if (window[1].z() < ip.z())
            window[1].z() = ip.z();
        }

      return window;
    }

    math::Vector3 Result::get_intercepted_center(const sys::Surface &s) const
    {
      math::VectorPair3 win = get_intercepted_window(s);

      return (win[0] + win[1]) / 2;
    }

    math::Vector3 Result::get_intercepted_centroid(const sys::Surface &s) const
    {
      const rays_queue_t & intercepts = get_intercepted(s);
      unsigned int      count = 0;
      math::Vector3     center(0, 0, 0);

      if (intercepts.empty())
        throw Error("no ray intercepts found on the surface");
      
      GOPTICAL_FOREACH(i, intercepts)
        {
          center += (*i)->get_intercept_point();
          count++;
        }

      center /= count;

      return center;
    }

    double Result::get_max_ray_intensity() const
    {
      double res = 0;

      GOPTICAL_FOREACH(r, _rays)
        {
          double i = r->get_intensity();

          if (i > res)
            res = i;
        }

      return res;
    }

    void Result::draw_2d(io::Renderer &r, bool hit_image, const sys::Element *ref) const
    {
      r.draw_trace_result_2d(*this, hit_image, ref);
    }

    void Result::draw_3d(io::Renderer &r, bool hit_image, const sys::Element *ref) const
    {
      r.draw_trace_result_3d(*this, hit_image, ref);
    }

  }
}

