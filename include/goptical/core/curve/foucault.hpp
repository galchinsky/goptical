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


#ifndef GOPTICAL_CURVE_FOUCAULT_HH_
#define GOPTICAL_CURVE_FOUCAULT_HH_

#include <vector>

#include <gsl/gsl_odeiv.h>

#include "goptical/core/common.hpp"

#include "goptical/core/curve/rotational.hpp"
#include "curve_roc.hpp"
#include "goptical/core/data/discrete_set.hpp"

namespace _goptical {

  namespace curve {

    /**
       @short Define surface curve from Foucault test data.
       @header <goptical/core/curve/Foucault
       @module {Core}
       @main

       This class allows definition of a rotationally symmetric curve
       from a set of Foucault test data. Fixed and moving light
       sources are supported.

       curve fitting can be used to find Foucault test readings from
       any other rotationally symmetric curve.

       Foucault test data are reduced using numerical integration of a
       differential equation to compute sagitta from slopes.

       Foucault data reduction algorithm based on "ATM Mirror mathematics"
       @url http://home.earthlink.net/~burrjaw/atm/atm_math.lwp/atm_math.htm

       Couder mask theory and formulas developed by Nils Olof Carlin
       @url http://www.atmsite.org/contrib/Carlin/couder/
    */
    class Foucault : public Rotational, public curveRoc
    {
    public:
      /** Create an empty foucault curve with no reading with the
          given radius of curvature */
      Foucault(double roc);

      ~Foucault();

      /** Set surface radius. Effective surface radius must be known
          to let the surface integration algorithm run up to a given
          limit. Sagitta and gradient values above the define radius
          won't be accurate.

          Radius is adjusted to max zone radius + 10% when adding
          readings.
      */
      inline void set_radius(double radius);

      /** Get surface radius. */
      inline double get_radius() const;

      /** Define a new zone at given zone radius and update knife edge
          reading (offset from radius of curvature) */
      void add_reading(double zone_radius, double knife_offset = 0.0);

      /** Clear current zones and add equally spaced zones
          readings. Knife edge reading (offset from radius of
          curvature) is set to 0 for all zones.
          @return New zones count
      */
      unsigned int add_uniform_zones(double hole_radius, unsigned int count);

      /** Clear current zones and add zones readings to locations
          suited for testing with a couder mask (as described on Nils
          Olof Carlin page). Zone count may be guessed if zero,
          provided that radius has been properly set before. Knife
          edge reading (offset from radius of curvature) is set to 0
          for all zones.
          @param edge Couder zones edges will be saved here if != NULL.
          @return Zone count
      */
      unsigned int add_couder_zones(double hole_radius, unsigned int count = 0, std::vector<double> *edge = 0);

      /** Get currently defined zones/readings count */
      inline unsigned int get_zones_count() const;

      /** Set knife edge reading of a previously defined zone */
      inline void set_knife_offset(unsigned int zone_number, double  knife_offset);

      /** Get reading data for a given zone number.
          @return std::pair with zone radius and knife offset
      */
      inline const std::pair<double, double> get_reading(unsigned int zone_number) const;

      /** Switch to moving source test and update knife edge readings
          of previously defined zone to fit provided rotationally
          symmetric curve. This means simulating a foucault test of
          the given curve using existing zones. */
      void fit(const Rotational &c);

      /** Clear all readings and zones */
      void clear();

      /** Set moving light source foucault test. This clear all
          readings and zones.
          @param source_offset Distance between knife edge and light source along optical axis. */
      inline void set_moving_source(double source_offset = 0.0);

      /** Set fixed light source foucault test. This clear all
          readings and zones.
          @param source_to_surface Distance between surface and light source along the optical axis. */
      inline void set_fixed_source(double source_to_surface);

      /** Set surface integration (ODE) algorithm step size, default is 1mm */
      inline void set_ode_stepsize(double step);

      double sagitta(double r) const;
      double derivative(double r) const;

    private:

      void update();
      void init();

      static int gsl_func(double t, const double y[], double f[], void *params);
      gsl_odeiv_step *gsl_st;
      gsl_odeiv_system gsl_sys;

      bool _moving_source;
      double _offset;
      double _radius;
      double _ode_step;
      data::DiscreteSet _reading;
      data::DiscreteSet _sagitta;
      bool _updated;
    };

  }
}

#endif

