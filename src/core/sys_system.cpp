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
#include <goptical/core/sys/Group>
#include <goptical/core/sys/Container>

#include <goptical/core/Error>
#include <goptical/core/sys/Surface>
#include <goptical/core/sys/Source>
#include <goptical/core/sys/OpticalSurface>
#include <goptical/core/trace/Params>
#include <goptical/core/math/Transform>
#include <goptical/core/trace/Ray>
#include <goptical/core/material/Air>
#include <goptical/core/material/Proxy>

namespace _goptical {

  namespace sys {

    system::system()
      : _version(0),
        _env_proxy(material::air),
        _tracer_params(),
        _e_count(0),
        _index_map(),
        _transform_cache()
    {
      transform_cache_resize(1);
      // index 0 is reserved for global coordinates transformations
      _index_map[0] = (Element*)1;
    }

    system::~system()
    {
      transform_cache_flush();
      remove_all();
    }

    void system::added(Element &e)
    {
      e.system_register(*this);

      update_version();
    }

    void system::removed(Element &e)
    {
      e.system_unregister();

      if (_entrance.ptr() == &e)
        _entrance.invalidate();

      if (_exit.ptr() == &e)
        _exit.invalidate();

      update_version();
    }

    void system::set_environment(const const_ref<material::Base> &env)
    {
      update_version();
      _env_proxy.set_material(env);
    }

    const math::Transform<3> & system::transform_l2g_cache_update(const Element &element) const
    {
      math::Transform<3> * & e = transform_cache_entry(element.id(), 0);

      if (!e)
        {
          math::Transform<3> t(element._transform);
          const Element *i1 = &element;

          while (const Element *i2 = dynamic_cast<Group *>(i1->_container))
            {
              t.compose(i2->_transform);

              i1 = i2;
            }

          assert(!e);
          e = new math::Transform<3>(t);
        }

      return *e;
    }

    const math::Transform<3> & system::transform_g2l_cache_update(const Element &element) const
    {
      math::Transform<3> * & e = transform_cache_entry(0, element.id());

      if (!e)
        e = new math::Transform<3>(transform_l2g_cache_update(element).inverse());

      return *e;
    }

    const math::Transform<3> & system::transform_cache_update(const Element &from, const Element &to) const
    {
      assert(&from != &to);

      math::Transform<3> * & e = transform_cache_entry(from.id(), to.id());

      if (!e)
        {
          const math::Transform<3> &t1 = transform_l2g_cache_update(from);
          const math::Transform<3> &t2 = transform_l2g_cache_update(to);

          if (!e)
            {
              e = new math::Transform<3>(t1);
              e->compose(t2.inverse());
            }
        }

      return *e;
    }

    void system::transform_cache_flush(const Element &element)
    {
      for (unsigned int x = 0; x < _e_count; x++)
        {
          if (math::Transform<3> * & t = transform_cache_entry(element.id(), x))
            {
              delete t;
              t = 0;
            }

          if (math::Transform<3> * & t = transform_cache_entry(x, element.id()))
            {
              delete t;
              t = 0;
            }
        }
    }

    void system::transform_cache_flush()
    {
      for (unsigned int x = 0; x < _e_count; x++)
        for (unsigned int y = 0; y < _e_count; y++)
          if (math::Transform<3> * & t = transform_cache_entry(x, y))
            {
              delete t;
              t = 0;
            }
    }

    void system::transform_cache_resize(unsigned int newsize)
    {
      unsigned int oldsize = _e_count;

      if (oldsize < newsize)
        {
          _index_map.resize(newsize, 0);
          _transform_cache.resize(newsize * newsize, 0);

          for (int y = oldsize - 1; y > 0; y--)
            {
              // move data
              for (int x = oldsize - 1; x >= 0; x--)
                {
                  _transform_cache[y * newsize + x] =
                    _transform_cache[y * oldsize + x];

                  _transform_cache[y * oldsize + x] = 0;
                }
            }

          _e_count = newsize;
        }
      else
        {
          // FIXME handle cache downsize
        }

    }

    unsigned int system::index_get(Element &element)
    {
      std::vector<Element*>::iterator i;
      unsigned int index;

      i = std::find(_index_map.begin(), _index_map.end(), (Element*)0);

      if (i == _index_map.end())
        {
          index = _e_count;
          transform_cache_resize(index + 1);
        }
      else
        {
          index = _index_map.begin() - i;
        }

      _index_map[index] = &element;

      return index;
    }

    void system::index_put(const Element &element)
    {
      transform_cache_flush(element);
      _index_map[element.id()] = 0;
    }

    void system::transform_cache_dump(std::ostream &o) const
    {
      o << "system transform cache size is " << _e_count << "x" << _e_count << std::endl;

      for (unsigned int from = 0; from < _e_count; from++)
        for (unsigned int to = 0; to < _e_count; to++)
          if (const math::Transform<3> *t = _transform_cache[from * _e_count + to])
            o << "from " << from << " to " << to << ":" << std::endl << *t << std::endl;
    }

    const Surface & system::get_entrance_pupil() const
    {
      const Surface *res = 0;

      if (_entrance.valid())
        return *_entrance;

      res = find<OpticalSurface>();

      if (!res)
        res = find<Surface>();

      if (!res)
        throw Error("No entrance pupil surface defined/guessed");

      return *res;
    }

    /** FIXME write an optimized version of this function which uses
        some kind of data structure (bsp tree?) */
    Surface *system::colide_next(const trace::Params &params,
                                 math::VectorPair3 &intersect,
                                 const trace::Ray &ray) const
    {
      const Element *origin = ray.get_creator();

      // test all elements and keep closest intersection

      Surface *s, *e = 0;
      math::VectorPair3 inter;
      double    min_dist = std::numeric_limits<double>::max();

      for (unsigned int i = 1; i <= get_element_count(); i++)
        {
          Element *j = &get_element(i);

          if (j == origin || !j->is_enabled())
            continue;

          if ((s = dynamic_cast<Surface*>(j)))
            {
              const math::Transform<3> &t = origin->get_transform_to(*s);
              math::VectorPair3 local(t.transform_line(ray));

              if (s->intersect(params, inter, local))
                {
                  double        dist = (inter.origin() - local.origin()).len();

                  if (min_dist > dist)
                    {
                      min_dist = dist;
                      intersect = inter;
                      e = s;
                    }
                }
            }
        }

      return e;
    }


  }

}

