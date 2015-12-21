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


#ifndef GOPTICAL_DISTRIBUTION_HXX_
#define GOPTICAL_DISTRIBUTION_HXX_

#include "goptical/core/error.hpp"

namespace _goptical
{

  namespace trace
  {

    Distribution::Distribution(Pattern pattern,
                               unsigned int radial_density,
                               double scaling)
    : _pattern(pattern),
      _radial_density(radial_density),
      _scaling(scaling)
    {
      if (radial_density < 1)
        throw Error("ray distribution radial density must be greater than 1");
    }

    void Distribution::set_pattern(Pattern p)
    {
      _pattern = p;
    }

    Pattern Distribution::get_pattern() const
    {
      return _pattern;
    }

    unsigned int Distribution::get_radial_density() const
    {
      return _radial_density;
    }

    void Distribution::set_radial_density(unsigned int density)
    {
      _radial_density = density;
    }

    double Distribution::get_scaling() const
    {
      return _scaling;
    }

    void Distribution::set_scaling(double margin)
    {
      _scaling = margin;
    }

    void Distribution::set_uniform_pattern()
    {
      switch (_pattern)
        {
        case SagittalDist:
        case MeridionalDist:
        case CrossDist:
          _pattern = DefaultDist;
        default:
          ;
        }
    }

  }
}

#endif

