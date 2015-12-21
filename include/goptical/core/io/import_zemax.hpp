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

#ifndef GOPTICAL_IO_IMPORT_ZEMAX_HH_
#define GOPTICAL_IO_IMPORT_ZEMAX_HH_

#include <string>
#include <map>

#include "goptical/core/common.hpp"

#include "goptical/core/shape/base.hpp"
#include "goptical/core/material/base.hpp"
#include "goptical/core/material/catalog.hpp"
#include "goptical/core/material/dielectric.hpp"
#include "goptical/core/sys/system.hpp"

namespace _goptical {

  namespace io {

    struct zemax_surface_s;

    /**
       @short Zemax files loader
       @header <goptical/core/io/ImportZemax
       @module {Core}
       @main
       @experimental

       This class implements a zemax optical design file and glass catalog loader.
     */
    class ImportZemax
    {
    public:
      /** @experimental */
      ref<sys::system> import_design(const std::string &filename);

      /** Set glass catalogs default path */
      inline ImportZemax& set_catalog_path(const std::string &path);

      /** Import Zemax ascii glass catalog, guess filename from default path and name */
      ref<material::Catalog> import_catalog(const std::string &name);

      /** Import Zemax ascii glass catalog file (@tt .agf). Guess catalog
          name from file name */
      ref<material::Catalog> import_catalog_file(const std::string &path);

      /** Import Zemax ascii glass catalog file (@tt .agf) */
      ref<material::Catalog> import_catalog(const std::string &path,
                                            const std::string &name);

      /** Get already imported catalog */
      ref<material::Catalog> get_catalog(const std::string &name);

      /** Import Zemax table glass material file (@tt .ztg) */
      ref<material::Dielectric> import_table_glass(const std::string &filename);

    private:

      static std::string basename(const std::string &path);

      const_ref<shape::Base> get_ap_shape(const struct zemax_surface_s &surf, double unit_factor) const;
      const_ref<material::Base> get_glass(sys::system &sys, const struct zemax_surface_s &surf) const;

      typedef std::map<std::string, ref<material::Catalog> > cat_map_t;

      cat_map_t         _cat_list;
      std::string       _cat_path;
    };

  }

}

#endif

