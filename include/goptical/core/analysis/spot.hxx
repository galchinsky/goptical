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


#ifndef GOPTICAL_ANALYSIS_SPOT_HXX_
#define GOPTICAL_ANALYSIS_SPOT_HXX_

#include "goptical/core/io/renderer_axes.hxx"
#include "goptical/core/math/vector.hxx"
#include "goptical/core/data/plot.hxx"

#include "goptical/core/analysis/pointimage.hxx"

  namespace _goptical
  {

  namespace analysis
  {

    void Spot::invalidate()
    {
      _processed_trace = false;
      _processed_analysis = false;
    }

    double Spot::get_max_radius()
    {
      process_analysis();

      return _max_radius;
    }

    double Spot::get_rms_radius()
    {
      process_analysis();

      return _rms_radius;
    }

    double Spot::get_total_intensity()
    {
      process_analysis();

      return _tot_intensity;
    }

    const math::Vector3 & Spot::get_centroid()
    {
      process_analysis();

      return _centroid;
    }

    void Spot::set_useful_radius(double radius)
    {
      _useful_radius = radius;
    }

    io::RendererAxes & Spot::get_diagram_axes()
    {
      return _axes;
    }

  }
  }

#endif

