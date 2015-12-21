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


#ifndef GOPTICAL_MATERIAL_DISPERSIONTABLE_HH_
#define GOPTICAL_MATERIAL_DISPERSIONTABLE_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/material/dielectric.hpp"

namespace _goptical {

  namespace material {

    /**
       @short Interpolated refractive index data set model
       @header <goptical/core/material/DispersionTable
       @module {Core}
       @main

       This class models refractive index of optical material using
       user provided data table.

       Known refractive index values are stored and interpolated by a
       @ref data::DiscreteSet object. Default interpolation is @ref
       data::Cubic.
     */

    class DispersionTable : public Dielectric
    {
    public:

      DispersionTable();

      /** Add refractive index data */
      inline void set_refractive_index(double wavelen, double index);

      /** Clear all refractive index data */
      inline void clear_refractive_index_table();

      /** Get refractive index dataset object */
      inline data::DiscreteSet & get_refractive_index_dataset();

      /** @override */
      double get_measurement_index(double wavelen) const;
    private:

      data::DiscreteSet _refractive_index;
    };

  }
}

#endif

