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

#ifndef GOPTICAL_STOP_HH_
#define GOPTICAL_STOP_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/sys/surface.hpp"

namespace _goptical {

  namespace sys {

    /**
       @short Aperture stop element
       @header <goptical/core/sys/Stop
       @module {Core}

       This class implements an aperture stop surface.

       This surface will block all rays which fall outside the surface
       shape and inside the external radius. (All rays which pass at a
       distance from the axis greater than the external radius are not
       affected.)

       Rays which fall inside the surface shape may either be
       intercepted and reemited or unaffected. Rays are always
       intercepted in sequential raytrace mode. In non-sequential mode
       this can be changed with @ref set_intercept_reemit; default is disabled.
    */

    class Stop : public Surface
    {
    public:
      /** Create an aperture stop surface. External radius is set to
          twice the shape max radius. */
      Stop(const math::VectorPair3 &p, const const_ref<shape::Base> &shape);

      /** Create a circular aperture stop surface with given
          radius. External radius is set to twice the shape max
          radius. */
      Stop(const math::VectorPair3 &p, double radius);

      GOPTICAL_ACCESSORS(double, external_radius,
                         "stop external radius. @see Stop");

      GOPTICAL_ACCESSORS(bool, intercept_reemit,
                         "intercept and reemit enabled. @see Stop");

    private:

      /** @override */
      void draw_2d_e(io::Renderer &r, const Element *ref) const;
      /** @override Only draw stop surface edges */
      void draw_3d_e(io::Renderer &r, const Element *ref) const;

      template <trace::IntensityMode m>
      inline void process_rays_(trace::Result &result,
                               trace::rays_queue_t *input) const;

      /** @override */
      bool intersect(const trace::Params &params,
                     math::VectorPair3 &pt,
                     const math::VectorPair3 &ray) const;

      /** @override */
      void trace_ray_simple(trace::Result &result, trace::Ray &incident,
                            const math::VectorPair3 &local, const math::VectorPair3 &intersect) const;

      /** @override */
      void trace_ray_intensity(trace::Result &result, trace::Ray &incident,
                               const math::VectorPair3 &local, const math::VectorPair3 &intersect) const;

      /** @override */
      void process_rays_simple(trace::Result &result,
                              trace::rays_queue_t *input) const;

      /** @override */
      void process_rays_intensity(trace::Result &result,
                                 trace::rays_queue_t *input) const;

      /** @override */
      void process_rays_polarized(trace::Result &result,
                                 trace::rays_queue_t *input) const;

      double _external_radius;
      bool _intercept_reemit;
    };

  }
}

#endif

