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


#ifndef GOPTICAL_ANALYSIS_POINTIMAGE_HXX_
#define GOPTICAL_ANALYSIS_POINTIMAGE_HXX_

#include "goptical/core/trace/tracer.hxx"
#include "goptical/core/sys/system.hxx"

namespace _goptical
{

  namespace analysis
  {

    void PointImage::set_image(sys::Image *image)
    {
      _image = image;
    }

    trace::tracer & PointImage::get_tracer()
    {
      invalidate();
      return _tracer;
    }

    /** return tracer object used for ray tracing */
    const trace::tracer & PointImage::get_tracer() const
    {
      return _tracer;
    }

  }

}

#endif

