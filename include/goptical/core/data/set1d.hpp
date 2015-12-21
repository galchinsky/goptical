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


#ifndef GOPTICAL_DATA_SET1D_HH_
#define GOPTICAL_DATA_SET1D_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/data/set.hpp"

namespace _goptical {

  namespace data {

    /**
       @short Base class for 1d y = f(x) numerical data set
       @header <goptical/core/data/Set1d
       @module {Core}
    */
    class Set1d : public Set
    {

    public:
      virtual inline ~Set1d();

      /** Get total number of data stored in data set */
      virtual unsigned int get_count() const = 0;

      /** Get x data at index n in data set */
      virtual double get_x_value(unsigned int n) const = 0;
      /** Get y data stored at index n in data set */
      virtual double get_y_value(unsigned int n) const = 0;
      /** Get modifiable reference to y data stored at index n in data set */
      virtual double & get_y_value(unsigned int n) = 0;

      /** Interpolate y value corresponding to given x value in data set. */
      virtual double interpolate(const double x) const = 0;
      /** Interpolate y value corresponding to given x value in data
          set. data may be differentiated several times.
          @param deriv Differentiation count, 0 means y value, 1 means 1st derivative...
      */
      virtual double interpolate(const double x, unsigned int deriv) const = 0;

      /** Get minimal and maximal x values on found in data set */
      virtual math::range_t get_x_range() const = 0;

      // inherited from Set
      inline unsigned int get_dimensions() const;
      inline unsigned int get_count(unsigned int dim) const;
      inline double get_x_value(unsigned int n, unsigned int dim) const;
      inline double get_y_value(const unsigned int x[]) const;
      inline double interpolate(const double x[]) const;
      inline double interpolate(const double x[], unsigned int deriv, unsigned int dimension) const;
      inline math::range_t get_x_range(unsigned int dimension) const;

    protected:
      inline Set1d();
    };

  }
}

#endif

