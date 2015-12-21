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

#include <cstdlib>
#include <goptical/core/material/SellmeierMod>

namespace _goptical {

  namespace material {

    template <enum SellmeierModFormula m>
    SellmeierMod<m>::SellmeierMod()
    {
    }

    template <enum SellmeierModFormula m>
    SellmeierMod<m>::SellmeierMod(double A, double B,
                                  double C, double D, double E)
      : _a(A), _b(B), _c(C), _d(D), _e(E)
    {
    }

    template <enum SellmeierModFormula m>
    double SellmeierMod<m>::get_measurement_index(double wavelen) const
    {
      double w2 = math::square(wavelen / 1000.0);

      switch (m)
        {
        case SellmeierMod2Formula:
          return sqrt(_a + (_b*w2)/(w2-math::square(_c)) + _d/(w2-math::square(_e)));

        case Handbook1Formula:
          return sqrt(_a + (_b*w2) + _c/(w2-_d));

        case Handbook2Formula:
          return sqrt(_a + (_b*w2) + (_c*w2)/(w2-_d));
        }

    }

    template class SellmeierMod<SellmeierMod2Formula>;
    template class SellmeierMod<Handbook1Formula>;
    template class SellmeierMod<Handbook2Formula>;

  }

}

