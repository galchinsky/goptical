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

#ifndef GOPTICAL_IO_IMPORT
#define GOPTICAL_IO_IMPORT

#include <string>

#include "goptical/core/common.hpp"

namespace _goptical {

  namespace io {

    /**
       @short File import class interface
       @header <goptical/core/io/Import
       @module {Core}
     */
    class Import
    {
    public:
      /** Import optical design from file */
      virtual sys::system & import_design(const std::string &filename) = 0;

      /** Import material catalog from file */
      virtual material::Catalog & import_catalog(const std::string &filename) = 0;

      /** Import dataset from file */
      virtual Set & import_dataset(const std::string &filename) = 0;
    };

  }

}

#endif

