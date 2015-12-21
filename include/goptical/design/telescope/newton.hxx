/*

      This file is part of the <goptical/core Design library.
  
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

#ifndef GOPTICAL_DESIGN_TELESCOPE_NEWTON_HXX_
#define GOPTICAL_DESIGN_TELESCOPE_NEWTON_HXX_

#include <algorithm>

#include <goptical/core/sys/mirror.hxx>
#include <goptical/core/shape/disk.hxx>
#include <goptical/core/shape/ellipse.hxx>
#include <goptical/core/curve/flat.hxx>
#include <goptical/core/curve/conic.hxx>

#include "telescope.hxx"

namespace _goptical {

  namespace Design {

    namespace telescope {

      double Newton::get_focal() const
      {
        return _focal;
      }

      double Newton::get_diameter() const
      {
        return _diameter;
      }

      double Newton::get_bwd() const
      {
        return _bwd;
      }

      double Newton::get_field_angle() const
      {
        return _field_angle;
      }

      sys::Mirror & Newton::get_primary()
      {
        return _primary;
      }

      const sys::Mirror & Newton::get_primary() const
      {
        return _primary;
      }

      sys::Mirror & Newton::get_secondary()
      {
        return _secondary;
      }

      const sys::Mirror & Newton::get_secondary() const
      {
        return _secondary;
      }

      double Newton::get_secondary_offset() const
      {
        return _offset;
      }

      double Newton::get_secondary_minor_axis() const
      {
        return _minor_axis;
      }

      double Newton::get_secondary_major_axis() const
      {
        return _major_axis;
      }

      double Newton::get_unvignetted_image_diameter() const
      {
        return _unvignetted_image_size;
      }

    }
  }
}

#endif

