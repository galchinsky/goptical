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

#ifndef GOPTICAL_DESIGN_TELESCOPE_CASSEGRAIN_HXX_
#define GOPTICAL_DESIGN_TELESCOPE_CASSEGRAIN_HXX_

#include <goptical/core/sys/mirror.hxx>
#include <goptical/core/shape/disk.hxx>
#include <goptical/core/shape/ring.hxx>
#include <goptical/core/curve/conic.hxx>

#include "telescope.hxx"

namespace _goptical {

  namespace Design {

    namespace telescope {

      template <CassegrainType type>
      double Cassegrain<type>::get_focal() const
      {
        return _f1;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_diameter() const
      {
        return _d1;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_bwd() const
      {
        return _bwd;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_field_angle() const
      {
        return _fa;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_secondary_diameter() const
      {
        return _d2;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_magnification() const
      {
        return _mag;
      }

      template <CassegrainType type>
      const sys::Mirror & Cassegrain<type>::get_primary() const
      {
        return _primary;
      }

      template <CassegrainType type>
      sys::Mirror & Cassegrain<type>::get_primary()
      {
        return _primary;
      }

      template <CassegrainType type>
      const sys::Mirror & Cassegrain<type>::get_secondary() const
      {
        return _secondary;
      }

      template <CassegrainType type>
      sys::Mirror & Cassegrain<type>::get_secondary()
      {
        return _secondary;
      }

      template <CassegrainType type>
      math::VectorPair3 Cassegrain<type>::get_focal_plane() const
      {
        return _focal_plane;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_unvignetted_image_diameter() const
      {
        return _id;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_mirrors_separation() const
      {
        return _ms;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_primary_conic() const
      {
        return _k1;
      }

      template <CassegrainType type>
      double Cassegrain<type>::get_secondary_conic() const
      {
        return _k2;
      }

    }
  }
}

#endif

