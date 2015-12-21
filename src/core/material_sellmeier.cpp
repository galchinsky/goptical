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

#include <goptical/core/material/Sellmeier>

namespace _goptical {

  namespace material {

    Sellmeier::Sellmeier()
      : _coeff(),
        _constant(1.0)
    {
    }

    Sellmeier::Sellmeier(double K1, double L1,
                         double K2, double L2,
                         double K3, double L3)
      : _coeff(),
        _constant(1.0)
    {
      _coeff.reserve(6);
      _coeff.push_back(K1);
      _coeff.push_back(L1);
      _coeff.push_back(K2);
      _coeff.push_back(L2);
      _coeff.push_back(K3);
      _coeff.push_back(L3);
    }

    void Sellmeier::set_terms_count(unsigned int c)
    {
      _coeff.resize(c * 2, 0.0);
    }

    double Sellmeier::get_measurement_index(double wavelen) const
    {
      double w2 = math::square(wavelen / 1000.0);
      double n = _constant;

      for (unsigned int i = 0; i < _coeff.size(); i += 2)
        n += (w2 * _coeff[i]) / (w2 - _coeff[i + 1]);

      return sqrt(n);
    }

  }

}

