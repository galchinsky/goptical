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
  
      Copyright (C) 2011 Free Software Foundation, Inc
      Author: Alexandre Becoulet

*/

#include <goptical/core/material/Abbe>

namespace _goptical {

  namespace material {

    template <enum AbbeFormula m>
    Abbe<m>::Abbe(double n, double v, double dpgF)
    {
      _n = n;
      _q = (n - 1.) / v;
      _a = (v * -0.001682) + 0.6438 + dpgF;
    }

    template <enum AbbeFormula m>
    double Abbe<m>::get_measurement_index(double wavelen) const
    {
      double wl = wavelen / 1000.;
      double w2 = wl * wl;
      double w3 = w2 * wl;
      double f;

      switch (m)
        {
          // fitting code can be found is /extra/abbe_model_fit/ directory

        case AbbeVdFormula:
          f =
            (_a * -6.11873891971188577088  +  1.17752614766485175224) +
            (_a *  18.27315722388047447566 + -8.93204522498095698779) / wl +
            (_a * -14.55275321129051135927 +  7.91015964461522003148) / w2 +
            (_a *  3.48385106908642905310  + -1.80321117937358499361) / w3;
          break;

        case AbbeVeFormula:
          f =
            (_a * -5.70205459879993181715  +  0.73560912822245871912) +
            (_a *  17.84619335902774039937 + -8.71504708663084315390) / wl +
            (_a * -14.30050903441605747446 +  7.77787634432116181671) / w2 +
            (_a *  3.41225047218704347074  + -1.76619259848202947438) / w3;
          break;
        }

      return _n + _q * f;
    }

    template class Abbe<AbbeVdFormula>;
    template class Abbe<AbbeVeFormula>;

  }

}

