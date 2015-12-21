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


#ifndef GOPTICAL_DATA_SET_HH_
#define GOPTICAL_DATA_SET_HH_

#include <vector>
#include <string>
#include <ostream>

#include "goptical/core/common.hpp"

namespace _goptical {

  namespace data {

  std::ostream & operator<<(std::ostream &o, const Set &s);

    /**
       @short Base class for numerical data sets.
       @header <goptical/core/data/Set
       @module {Core}

       This class is a base class for all numerical data sets
       implementations. It defines an interface to access data in a
       way independent from data storage and dimensions count.

       Each data set implementation may define a alternative specific
       interface to access their data.

       Here x is seen as value used to access the container.
       Containers with more than one dimension will require x0, x1,
       ..., xn known parameters to access the stored y value.
    */
    class Set : public ref_base<Set>
    {
      friend std::ostream & operator<<(std::ostream &o, const Set &s);

    public:
      virtual ~Set();

      /** Get total number of dimensions */
      virtual unsigned int get_dimensions() const = 0;

      /** Get total number of data stored for dimension n in data set */
      virtual unsigned int get_count(unsigned int dim = 0) const = 0;

      /** Get data stored at position n on dimension dim in data set */
      virtual double get_x_value(unsigned int n, unsigned int dim = 0) const = 0;

      /** Get y data stored at position (x0, x1, ...) in data set */
      virtual double get_y_value(const unsigned int x[]) const = 0;

      /** Interpolate y value corresponding to given x value(s) in data set. */
      virtual double interpolate(const double x[]) const = 0;

      /** Interpolate y value corresponding to given x value in data
          set. data may be differentiated several times along the requested dimension.
          @param deriv Differentiation count, 0 means y value, 1 means 1st derivative...
          @param dim Differentiation dimension
      */
      virtual double interpolate(const double x[], unsigned int deriv, unsigned int dim) const = 0;

      /** Get minimal and maximal x values on dimension n found in data set */
      virtual math::range_t get_x_range(unsigned int dim = 0) const = 0;

      /** Get minimal and maximal y values found in data set */
      virtual math::range_t get_y_range() const;

      /** Select interpolation method */
      virtual void set_interpolation(Interpolation i) = 0;

      /** Get current interpolation method */
      inline Interpolation get_interpolation();

      // FIXME dataset version number
      /** Return version number which is incremented on each data set change/clear */
      inline unsigned int get_version() const;

    protected:
      Set();

      unsigned int      _version;
      Interpolation     _interpolation;
    };

  }
}

#endif

