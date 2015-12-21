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


#ifndef GOPTICAL_TRACER_PARAMS_HH_
#define GOPTICAL_TRACER_PARAMS_HH_

#include <map>

#include "goptical/core/common.hpp"

#include "goptical/core/trace/result.hpp"
#include "goptical/core/trace/distribution.hpp"
#include "goptical/core/trace/sequence.hpp"

namespace _goptical {

  namespace trace {

    /**
       @short light propagation parameters descriptor
       @header <goptical/core/trace/Params
       @module {Core}
       @main

       This class is used to store light progation parameters.  This
       includes sequential / non-sequential mode, light intensity
       computation mode and propagation mode (raytracing, diffraction, ...).

       @xsee {tuto_seqtrace}
     */
    class Params
    {
      friend class tracer;

    public:
      inline Params();

      GOPTICAL_NOCONST_REF_ACCESSORS(Distribution, default_distribution,
       "default rays distribution pattern");

      GOPTICAL_ACCESSORS(unsigned int, max_bounce,
       "maximum ray bounce count, default is 50");

      GOPTICAL_ACCESSORS(double, lost_ray_length,
       "lost ray length");

      GOPTICAL_ACCESSORS(IntensityMode, intensity_mode,
       "raytracing intensity mode");

      GOPTICAL_ACCESSORS(bool, unobstructed,
        "unobstructed raytracing mode. Surface shapes are ignored, no rays are stopped");

      GOPTICAL_ACCESSORS(PropagationMode, propagation_mode,
        "physical light propagation mode. @experimental @hidden");

      /** Set sequential ray tracing mode */
      inline void set_sequential_mode(const const_ref<Sequence> &seq);

      /** Set non sequential ray tracing mode (default) */
      inline void set_nonsequential_mode();

      /** Test if in sequential ray tracing mode */
      inline bool is_sequential() const;

      /** Set distribution pattern for a given surface */
      inline void set_distribution(const sys::Surface &s, const Distribution &dist);

      /** Reset all surface specific distribution settings to default */
      inline void reset_distribution();

      /** Get distribution pattern for a given surface */
      inline const Distribution & get_distribution(const sys::Surface &s) const;

    private:
      typedef std::map<const sys::Surface *, Distribution> _s_distribution_map_t;

      const_ref<Sequence>       _sequence;
      Distribution              _default_distribution;
      _s_distribution_map_t     _s_distribution;
      unsigned int              _max_bounce;
      IntensityMode             _intensity_mode;
      bool                      _sequential_mode;
      PropagationMode           _propagation_mode;
      bool                      _unobstructed;
      double                    _lost_ray_length;
    };
  }
}

#endif

