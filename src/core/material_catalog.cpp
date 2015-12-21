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

#include <goptical/core/material/Catalog>
#include <goptical/core/material/Base>

namespace _goptical {

  namespace material {

    Catalog::Catalog(const std::string & name)
      : _name(name),
        _list()
    {
    }

    void Catalog::add_material(const std::string &material_name, const const_ref<Base> &material)
    {
      if (!_list.insert(catalog_map_t::value_type(material_name, material)).second)
        throw Error("material already present in catalog");
    }

    void Catalog::del_material(const std::string &material_name)
    {
      _list.erase(material_name);
    }

  }

}

