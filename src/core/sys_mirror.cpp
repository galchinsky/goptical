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

#include <goptical/core/shape/Disk>
#include <goptical/core/curve/Conic>
#include <goptical/core/curve/Flat>

#include <goptical/core/sys/Mirror>
#include <goptical/core/sys/OpticalSurface>

namespace _goptical {

  namespace sys {

    Mirror::Mirror(const math::VectorPair3 &p,
                   const const_ref<curve::Base> &curve,
                   const const_ref<shape::Base> &shape,
                   bool light_from_left,
                   const const_ref<material::Base> &metal,
                   const const_ref<material::Base> &air)
      : OpticalSurface(p, curve, shape,
                       light_from_left ? air : metal,
                       light_from_left ? metal : air)
    {
    }

    Mirror::Mirror(const math::VectorPair3 &p,
                   const const_ref<curve::Base> &curve,
                   double ap_radius,
                   bool light_from_left,
                   const const_ref<material::Base> &metal,
                   const const_ref<material::Base> &air)
      : OpticalSurface(p, curve, ap_radius,
                       light_from_left ? air : metal,
                       light_from_left ? metal : air)
    {
    }

    Mirror::Mirror(const math::VectorPair3 &p,
                   double roc, double sc,
                   double ap_radius,
                   bool light_from_left,
                   const const_ref<material::Base> &metal,
                   const const_ref<material::Base> &air)
      : OpticalSurface(p, roc == 0. ? const_ref<curve::Base>(curve::flat)
                                    : const_ref<curve::Base>(ref<curve::Conic>::create(roc, sc)),
                       ref<shape::Disk>::create(ap_radius),
                       light_from_left ? air : metal,
                       light_from_left ? metal : air)
    {
    }

  }

}

