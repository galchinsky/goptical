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


#ifndef GOPTICAL_DATA_DISCRETESET_1D_HH_
#define GOPTICAL_DATA_DISCRETESET_1D_HH_

#include <vector>

#include "goptical/core/common.hpp"

#include "goptical/core/data/set1d.hpp"
#include "goptical/core/data/set1d.hxx"
#include "data_interpolate_1d.hpp"

namespace _goptical {

  namespace data {

    /**
       @short Base class for DiscreteSet
       @header <goptical/core/data/DiscreteSet
       @module {Core}
       @internal
    */
    class DiscreteSetBase : public Set1d
    {

    public:

      DiscreteSetBase();
      ~DiscreteSetBase();

      /** Insert data pair in data set. If a pair with the same x
          value exists, it will be replaced by the new
          value. Derivative value may be provided as well. */
      void add_data(double x, double y, double yp = 0.0);

      /** Reserve data entries */
      inline void reserve(size_t n);

      /** Clear all data */
      void clear();

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
      /** x, y and user prescribed first derivative values */
      struct entry_s
      {
        double x, y, d;
      };

      /** find lower bound index of interval containing value */
      unsigned int get_interval(double x) const;
      /** find nearest value index */
      unsigned int get_nearest(double x) const;

      virtual void invalidate() = 0;

      inline double get_x_interval(unsigned int x) const;
      inline double get_x_interval(unsigned int x1, unsigned int x2) const;

      std::vector<struct entry_s>       _data;
    };

    /** 
        @short 1d numerical data set with interpolation
        @header <goptical/core/data/DiscreteSet
        @module {Core}
        @main

        This class provides a numerical data container
        where value pairs can be defined at arbitrary positions (x, y).

        Severals interpolation algorithms are available to guess
        values between defined knots, see @ref Interpolation.

        @see SampleSet
    */
    class DiscreteSet : public Interpolate1d<DiscreteSetBase>
    {
    public:

      DiscreteSet()
        : Interpolate1d<DiscreteSetBase>() { }
    };

  }
}

#endif

