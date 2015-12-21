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

#include <goptical/design/telescope/Cassegrain>

namespace _goptical {

  namespace Design {

    namespace telescope {

      template <CassegrainType type>
      Cassegrain<type>::Cassegrain(const math::VectorPair3 &p, double focal,
                                   double magnification, double diameter,
                                   double bwd, double field_angle)
        : telescope(p),
          _f(focal),
          _d1(diameter),
          _bwd(bwd),
          _fa(field_angle),
          _mag(magnification),

          _ms(calc()),

          _primary_shape(_d1 / 2., _hd / 2),
          _primary_curve(_f1 * 2., _k1),
          _primary(math::VectorPair3(0, 0, _ms, 0, 0, -1.),
                   _primary_curve, _primary_shape, false),
          _secondary_shape(_d2 / 2.),
          _secondary_curve(_f2 * 2., _k2),
          _secondary(math::VectorPair3(0, 0, 0, 0, 0, -1),
                     _secondary_curve, _secondary_shape),

          _focal_plane(0, 0, _ms + _bwd, 0, 0, 1)
      {
        add(_primary);
        add(_secondary);
      }

      template <CassegrainType type>
      double Cassegrain<type>::calc()
      {
        // Cassegrain geometry based on Texreau formula
        _f1 = _f / _mag;
        double A = (_f1 + _bwd) / (_mag + 1);
        _f2 = A * _mag / (_mag - 1);

        _ms = _f1 - A;
        double t = tan(math::degree2rad(_fa / 2.0));
        _id = 2 * t * _f;
        _d2 = _d1 * A / _f1 + 2 * t * _ms;
        _hd = _id + (_d2 - _id) * _bwd / (_ms + _bwd);

        // Cassegrain curvature math from telescope Optics, Chapter 21
        double E = _bwd / _f1;
        double R = _ms / _f1;
        double M = (E + R)/(1-R);
        double alpha = math::square((M+1)/(M-1));
        double beta = pow(M-1, 3)*(1-R)/(M*M*M);
        double gamma = pow(M-1, 3)*R/(M*M*M);
        double delta = 2/(M*M);

        switch (type)
          {
          case ClassicalCassegrain:
            _k1 = -1;
            _k2 = -alpha;
            break;
          case RitcheyChretien:
            _k1 = -1 - beta*delta/gamma;
            _k2 = -alpha - delta/gamma;
            break;
          case DallKirkham:
            _k1 = alpha * beta - 1;
            _k2 = 0;
            break;
          case PressmannCamichel:
            _k1 = 0;
            _k2 = 1/beta - alpha;
            break;
          }

        return _ms;
      }

      template <CassegrainType type>
      void Cassegrain<type>::update()
      {
        calc();

        _primary_shape.set_radius(_d1 / 2, _hd / 2);
        _primary_curve.set_roc(_f1 * 2);
        _primary_curve.set_schwarzschild(_k1);
        _primary.set_local_position(math::Vector3(0, 0, _ms));

        _secondary_shape.set_radius(_d2 / 2);
        _secondary_curve.set_roc(_f2 * 2);
        _secondary_curve.set_schwarzschild(_k2);

        _focal_plane = math::VectorPair3(0, 0, _ms + _bwd, 0, 0, 1);
      }

      template <CassegrainType type>
      void Cassegrain<type>::set_focal(double f)
      {
        _f = f;
        update();
      }

      template <CassegrainType type>
      void Cassegrain<type>::set_diameter(double d1)
      {
        _d1 = d1;
        update();
      }

      template <CassegrainType type>
      void Cassegrain<type>::set_bwd(double bwd)
      {
        _bwd = bwd;
        update();
      }

      template <CassegrainType type>
      void Cassegrain<type>::set_field_angle(double fa)
      {
        _fa = fa;
        update();
      }

      template <CassegrainType type>
      void Cassegrain<type>::set_secondary_diameter(double d2)
      {
        _d2 = d2;
        update();
      }

      template <CassegrainType type>
      void Cassegrain<type>::set_magnification(double m)
      {
        _mag = m;
        update();
      }

      template class Cassegrain<ClassicalCassegrain>;
      template class Cassegrain<RitcheyChretien>;
      template class Cassegrain<DallKirkham>;
      template class Cassegrain<PressmannCamichel>;

    }

  }
}


