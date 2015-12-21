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


#ifndef GOPTICAL_SHAPE_COMPOSER_HXX_
#define GOPTICAL_SHAPE_COMPOSER_HXX_

#include "goptical/core/math/transform.hxx"
#include "base.hxx"

namespace _goptical {

  namespace shape {

    Composer::Attributes & Composer::Attributes::scale(const math::Vector2 &factor)
    {
      _transform.affine_scaling(factor);
      _inv_transform = _transform.inverse();

      return *this;
    }

    Composer::Attributes & Composer::Attributes::rotate(double angle)
    {
      _transform.affine_rotation(0, angle);
      _inv_transform = _transform.inverse();

      return *this;
    }

    Composer::Attributes & Composer::Attributes::translate(const math::Vector2 &offset)
    {
      _transform.apply_translation(offset);
      _inv_transform = _transform.inverse();

      return *this;
    }

    void Composer::use_global_distribution(bool use_global)
    {
      _global_dist = use_global;
    }

  }
}

#endif

