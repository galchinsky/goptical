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
      Boston, MA 02111-1307 USAh
  
      Copyright (C) 2010-2011 Free Software Foundation, Inc
      Author: Alexandre Becoulet

*/


#ifndef GOPTICAL_DATA_SAMPLESET_1D_HH_
#define GOPTICAL_DATA_SAMPLESET_1D_HH_

#include <vector>

#include "goptical/core/common.hpp"

#include "goptical/core/data/set1d.hpp"
#include "data_interpolate_1d.hpp"

namespace _goptical {

  namespace data {

    /**
       @short Base class for SampleSet
       @header <goptical/core/data/SampleSet
       @module {Core}
       @internal
     */
    class SampleSetBase : public Set1d
    {
    public:
      SampleSetBase();
      ~SampleSetBase();

      /** Set y value and its derivative at index x */
      void set_value(unsigned int x, double y, double d = 0.0);

      /** Set y value and its derivative at nearest x value */
      void set_value_near(double x, double y, double d = 0.0);

      /** Clear all data */
      void clear();

      /** Set number of samples */
      void resize(unsigned int n);

      /** Set x origin and step values */
      inline void set_metrics(double origin, double step);

      /** Get step size between consecutive x values */
      inline double get_step() const;

      /** Get x value of first sample */
      inline double get_origin() const;

      /** Get stored derivative value at index x */
      inline double get_d_value(unsigned int x) const;
      /** Get modifiable reference to stored derivative value at index x */
      inline double & get_d_value(unsigned int x);

      // inherited from Set1d
      inline unsigned int get_count() const;
      inline double get_x_value(unsigned int x) const;
      inline double get_y_value(unsigned int x) const;
      inline double & get_y_value(unsigned int x);
      math::range_t get_x_range() const;

    protected:
      /* y and prescribed first derivative values */
      struct entry_s
      {
        double y, d;
      };

      /** find lower bound index of interval containing value */
      unsigned int get_interval(double x) const;
      /** find nearest value index */
      unsigned int get_nearest(double x) const;

      virtual void invalidate() = 0;

      inline double get_x_interval(unsigned int x) const;
      inline double get_x_interval(unsigned int x1, unsigned int x2) const;

      double            _origin, _step;
      std::vector<struct entry_s> _data;
    };

    /**
       @short 1d fixed interval numerical data set with interpolation
       @header <goptical/core/data/SampleSet
       @module {Core}
       @main

       This class provides a numerical data container
       where sample values (y) are defined for uniformly
       distributed (x) values.

       Severals interpolation algorithms are available to guess
       values between defined knots, see @ref Interpolation.

       @see DiscreteSet
    */
    class SampleSet : public Interpolate1d<SampleSetBase>
    {
    public:

      SampleSet()
        : Interpolate1d<SampleSetBase>() { }
    };

  }
}

#endif

