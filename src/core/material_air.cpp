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


#include <goptical/core/material/Air>

namespace _goptical {

  namespace material {

    template <enum AirFormula m>
    Air<m>::Air(double pressure)
      : Base("air"),
        _pressure(pressure)
    {
    }

    template <enum AirFormula m>
    bool Air<m>::is_opaque() const
    {
      return false;
    }

    template <enum AirFormula m>
    bool Air<m>::is_reflecting() const
    {
      return false;
    }

    template <enum AirFormula m>
    double Air<m>::get_internal_transmittance(double wavelen, double thickness) const
    {
      // FIXME find a formula
      return 1.0;
    }

    template <enum AirFormula m>
    double Air<m>::get_extinction_coef(double wavelen) const
    {
      // FIXME find a formula
      return 0.0;
    }

    template <enum AirFormula m>
    double Air<m>::get_refractive_index(double wavelen) const
    {
      switch (m)
        {
        case AirBirch94Formula: {
          // Birch, Metrologia, 1994, 31, 315

          // _temperature in celsius
          // _pressure in pascal

          double s2 = math::square(1 / (wavelen / 1000.0));

          double ref = /*1.0 +*/ 1e-8
            * (+ 8342.54
               + 2406147.0 / (130.0 - s2)
               + 15998.0 / (38.9 - s2)
               );

          return 1.0 + (ref /*- 1.0*/)
            * (_pressure * (1.0 + _pressure * (60.1 - 0.972 * _temperature) * 1e-10))
            / (96095.43 * (1.0 + 0.003661 * _temperature));
        }

        case AirKohlrausch68Formula: {
          // F. Kohlrausch, Praktische Physik, 1968, 1, 408

          double w2 = math::square(wavelen / 1000.0);

          double nref = 1.0 + (+ 6432.8
                               + (2949810.0 * w2) / (146.0 * w2 - 1.0)
                               + (25540.0 * w2) / (41.0 * w2 - 1.0)) * 1e-8;

          return 1.0 + ( ((nref - 1.0) * (_pressure / std_pressure))
                         / (1.0 + (_temperature - 15.0) * 0.0034785));

        }
        }
    }

    template <enum AirFormula m>
    const double Air<m>::std_pressure = 101325.;

    template class Air<AirBirch94Formula>;
    template class Air<AirKohlrausch68Formula>;

    const AirBirch94 std_air;
    AirBirch94 air;

  }
}

