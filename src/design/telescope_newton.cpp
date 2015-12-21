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

#include <goptical/core/math/VectorPair>

#include <goptical/design/telescope/Newton>

namespace _goptical {

  namespace Design {

    namespace telescope {

      Newton::Newton(const math::VectorPair3 &p,
                                       double focal, double diameter,
                                       double bwd, double field_angle)
      : telescope(p),
        _focal(focal),
        _diameter(diameter),
        _bwd(_diameter / 2.0 + bwd),
        _field_angle(field_angle),
        _unvignetted_image_size(calc_unvignetted_image_size()),
        _offset(calc_secondary()),
        _primary_shape(diameter / 2.0),
        _primary_curve(_focal * 2.0, -1.0),
        _primary(math::VectorPair3(0, 0, _focal - _bwd, 0, 0, -1.0),
                 _primary_curve, _primary_shape, false /* FIXME */),
        _secondary_shape(_minor_axis / 2.0, _major_axis / 2.0),
        _secondary(math::Vector3(0, -_offset, _offset), curve::flat,
                   _secondary_shape),
        _focal_plane(math::Vector3(0, _bwd, 0), math::Vector3(0, 1.0, 0))
    {
      _secondary.rotate(-135, 0, 0);
      add(_primary);
      add(_secondary);
    }

    void Newton::update()
    {
      _primary_curve.set_roc(-_focal * 2.0);
      _primary.set_local_position(math::Vector3(0, 0, _focal - _bwd));
      _secondary_shape.set_radius(_minor_axis / 2.0, _major_axis / 2.0);
      _secondary.set_local_position(math::Vector3(0, -_offset, _offset));
    }

    double Newton::calc_secondary()
    {
      // formula from http://www.astro-electronic.de/faq2.html

      double e = math::square(_diameter) / (16.0 * _focal);
      double c = _focal - e;
      double b = _diameter - _unvignetted_image_size;
      double l = _unvignetted_image_size * c + _bwd * b;
      double m = 2.0 * c - b;
      double n = 2.0 * c + b;
      double a = l / m + l / n;
      double o = (l / m - l / n) / 2.0;

      _offset = o;
      _minor_axis = sqrt(math::square(a) - 4.0 * math::square(_offset));
      _major_axis = sqrt(2.0) * a;

      return _offset;
    }

    double Newton::calc_field_angle()
    {
      return _field_angle = math::rad2degree(atan(_unvignetted_image_size / 2.0 / _focal) * 2.0);
    }

    double Newton::calc_unvignetted_image_size()
    {
      return _unvignetted_image_size = tan(math::degree2rad(_field_angle / 2.0)) * _focal * 2;
    }

    void Newton::set_focal(double focal)
    {
      _focal = focal;
      calc_unvignetted_image_size();
      calc_secondary();
      update();
    }

    void Newton::set_diameter(double diameter)
    {
      _diameter = diameter;
      calc_secondary();
      update();
    }

    void Newton::set_secondary_minor_axis(double diameter)
    {
      _minor_axis = diameter;
      _unvignetted_image_size = (_minor_axis * _focal - _diameter * _bwd) / (_focal - _bwd);
      calc_secondary();
      update();
    }

    void Newton::set_field_angle(double fa)
    {
      _field_angle = fa;
      calc_unvignetted_image_size();
      calc_secondary();
      update();
    }

    void Newton::set_bwd(double bwd)
    {
      _bwd = bwd;
      calc_secondary();
      update();
      _focal_plane = math::VectorPair3(math::Vector3(0, _bwd, 0), math::Vector3(0, 1.0, 0));
    }

    math::VectorPair3 Newton::get_focal_plane() const
    {
      return get_transform().transform_line(_focal_plane);
    }

    }

  }

}

