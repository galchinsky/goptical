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


#ifndef GOPTICAL_MATERIAL_METAL_HH_
#define GOPTICAL_MATERIAL_METAL_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/data/discrete_set.hpp"
#include "goptical/core/material/solid.hpp"

namespace _goptical {

  namespace material {

    /**
       @short Metal optical material model.
       @header <goptical/core/material/Metal
       @module {Core}
       @main

       This class models optical properties of metal
       materials. Extinction and refractive index are stored as
       interpolated data sets.
     */

    class Metal : public Solid
    {
    public:
      Metal();

      bool is_opaque() const;
      bool is_reflecting() const;

      double get_absolute_refractive_index(double wavelen) const;
      double get_refractive_index(double wavelen) const;
      double get_extinction_coef(double wavelen) const;

      /** Get refractive index dataset object */
      inline data::DiscreteSet & get_refractive_index_dataset();
      /** Get extinction dataset object */
      inline data::DiscreteSet & get_extinction_coef_dataset();

    protected:
      data::DiscreteSet _extinction;
      data::DiscreteSet _refractive_index;
    };

  }
}

#endif

