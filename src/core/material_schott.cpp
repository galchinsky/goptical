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

#include <goptical/core/material/Schott>

namespace _goptical {

  namespace material {

    Schott::Schott()
      : _coeff(),
        _first(0)
    {
    }

    Schott::Schott(double A, double B,
                   double C, double D,
                   double E, double F)
      : _coeff(),
        _first(-8)
    {
      _coeff.reserve(6);
      _coeff.push_back(F);
      _coeff.push_back(E);
      _coeff.push_back(D);
      _coeff.push_back(C);
      _coeff.push_back(A);
      _coeff.push_back(B);
    }

    void Schott::set_terms_range(int first, int last)
    {
      unsigned int c = last - first;

      assert(first % 2 == 0);
      assert(last % 2 == 0);

      _coeff.resize(c / 2 + 1, 0.0);
      _first = first;
    }

    double Schott::get_measurement_index(double wavelen) const
    {
      double wl = wavelen / 1000.0;
      double n = 0;
      double x = (double)_first;

      for (unsigned int i = 0; i < _coeff.size(); i++)
        {
          n += _coeff[i] * pow(wl, x);
          x += 2.0;
        }

      return sqrt(n);
    }

  }

}

