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

#include <algorithm>

#include <goptical/core/trace/Sequence>

#include <goptical/core/sys/System>
#include <goptical/core/sys/Element>

namespace _goptical {

  namespace trace {

    Sequence::Sequence()
      : _list()
    {
    }

    Sequence::Sequence(const sys::system &system)
      : _list()
    {
      add(system);
    }

    static bool seq_sort(const const_ref<sys::Element> &a,
                         const const_ref<sys::Element> &b)
    {
      return a->get_position().z() < b->get_position().z();
    }

    void Sequence::add(const sys::system &system)
    {
      _list.clear();
      add(static_cast<const sys::Container&>(system));
      std::sort(_list.begin(), _list.end(), seq_sort);
    }

    void Sequence::add(const sys::Container &c)
    {
      GOPTICAL_FOREACH(i, c.get_element_list())
        {
          if (const sys::Container *cc = dynamic_cast<const sys::Container*>(i->ptr()))
            add(*cc);
          else
            _list.push_back(*i);
        }
    }

    std::ostream & operator<<(std::ostream &o, const Sequence &s)
    {
      GOPTICAL_FOREACH(i, s._list)
        o << "  " << **i << std::endl;

      return o;
    }

  }
}

