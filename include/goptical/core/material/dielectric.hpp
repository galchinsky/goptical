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


#ifndef GOPTICAL_MATERIAL_DIELECTRIC_HH_
#define GOPTICAL_MATERIAL_DIELECTRIC_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/data/discrete_set.hpp"
#include "goptical/core/material/solid.hpp"
#include "goptical/core/light/spectral_line.hpp"

namespace _goptical {

  namespace material {

    /**
       @short Dielectric optical material model base class.
       @header <goptical/core/material/Dielectric
       @module {Core}       get_refractive_index function when no temperature model is in use.

       Temperature coeffiecients can be defined to take current
       material temperature into account when computing absolute
       refractive index:

       @list
       @item The @ref set_temperature_dndt function enables use of
         refractive index temperature deviation factor.

       @item The @ref set_temperature_schott function enables Schott
         temperature model as described in Schott "@em{TIE-19:
         Temperature Coefficient of the Refractive Index}" technical
         information document. It uses the following formula:

         @math $ n_t = \frac{{n}^{2}-1.0}{2\times n} \times \Delta t
                 \times \left( \frac{\Delta t\times E_1+E_0}{{\lambda}^{2}-{\lambda_{tk}}^{2}} +
                 D_2\times {\Delta t}^{2}+D_1\times \Delta t+D_0 \right) $

         with @math $ \Delta t = t - t_{ref} $
         and @math $\lambda$ the micrometer wavelength.

       @end list
     */

    class Dielectric : public Solid
    {
    public:

      Dielectric();

      /** Get internal tranmittance dataset object.
          @see clear_internal_transmittance */
      inline data::DiscreteSet & get_transmittance_dataset();

      /** Add transmittance data, wavelen in nm */
      void set_internal_transmittance(double wavelen, double thickness,
                                      double transmittance);

      /** Clear all transmittance data. Returned transmittance value
          is always 1.0 when the data set is empty.
          @see get_transmittance_dataset */
      inline void clear_internal_transmittance();

      /** Compute glass principal dispersion. */
      double get_principal_dispersion() const;

      /** Compute glass Abbe number from refractive index model.
          Use d spectral line. Wavelen is in @em nm */
      double get_abbe_vd() const;

      /** Compute glass Abbe number from refractive index model.
          Use d spectral line. Wavelen is in @em nm */
      double get_abbe_ve() const;

      /** Compute glass partial dispersion between two wavelen in @em nm */
      double get_partial_dispersion(double wavelen1, double wavelen2) const;

      /** Set and enable Schott thermal coefficients. wl_tk in @em um. */
      inline void set_temperature_schott(double d0, double d1, double d2,
                                         double e0, double e1, double wl_tk);

      /** Set and enable dn/dt thermal coefficient. */
      inline void set_temperature_dndt(double dndt);

      /** Disable use of temperature coefficients */
      inline void disable_temperature_coeff();

      /** Set glass measurement medium material. */
      inline void set_measurement_medium(const const_ref<Base> &medium);

      /** Set wavelen validity range in @em nm */
      inline void set_wavelen_range(double low, double high);

      /** Get material relative refractive index in measurment medium
          at specified wavelen in @em nm. */
      virtual double get_measurement_index(double wavelen) const = 0;

      /** @override */
      bool is_opaque() const;
      /** @override */
      bool is_reflecting() const;
      /** @override */
      double get_internal_transmittance(double wavelen, double thickness) const;
      /** @override */
      double get_internal_transmittance(double wavelen) const;
      /** @override */
      double get_refractive_index(double wavelen) const;

    private:

      /** Get temperature coeffiecient of refractive index using
          absloute reference refractive index */
      double get_schott_temp(double wavelen, double ref_index) const;

      /** normalized 1mm thickness transmittance data */
      data::DiscreteSet _transmittance; 

      /** refractive index thermal data */
      enum thermal_model_e
        {
          ThermalNone,
          ThermalSchott,
          ThermalDnDt
        };

      enum thermal_model_e _temp_model;
      double    _temp_d0, _temp_d1, _temp_d2;
      double    _temp_e0, _temp_e1;
      double    _temp_wl_tk;

      /** wavelen data validity range (nm) */
      double    _low_wavelen;
      double    _high_wavelen;

      /** medium used during refractive index measurement */
      const_ref<Base> _measurement_medium;
    };

  }
}

#endif

