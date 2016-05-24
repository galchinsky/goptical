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


#ifndef GOPTICAL_ANALYSIS_POINTIMAGE_HH_
#define GOPTICAL_ANALYSIS_POINTIMAGE_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/trace/tracer.hpp"
#include "goptical/core/sys/system.hpp"

namespace _goptical
{

  namespace analysis
  {

    /**
       @short Point image analysis base class
       @header goptical/core/analysis/PointImage
       @module {Core}
       @internal
    */
    class PointImage
    {
    public:
      PointImage(sys::system &system);
      virtual ~PointImage();

      /** set Image which collect rays for analysis */
      inline void set_image(sys::Image *image);

      /** return tracer object used for ray tracing. This will
          invalidate current analysis data */
      inline trace::tracer & get_tracer();

      /** return tracer object used for ray tracing */
      inline const trace::tracer & get_tracer() const;

      /** invalidate current analysis data */
      virtual void invalidate() = 0;

    protected:
      void get_default_image();
      void trace();

      sys::system &     _system;
      trace::tracer     _tracer;
      bool              _processed_trace;
      sys::Image *      _image;
      const trace::rays_queue_t *_intercepts;

    private:
    };

  }
}

#endif

