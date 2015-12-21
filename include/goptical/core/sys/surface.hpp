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


#ifndef GOPTICAL_SURFACE_HH_
#define GOPTICAL_SURFACE_HH_

#include <iostream>

#include "goptical/core/common.hpp"

#include "goptical/core/sys/element.hpp"
#include "goptical/core/shape/base.hpp"
#include "goptical/core/curve/base.hpp"

namespace _goptical {

  namespace sys {

    /**
       @short Base class for surfaces
       @header <goptical/core/sys/Surface
       @module {Core}

       This is the base class for surface element. A surface element
       is defined by its position, surface curve and contour shape.

       This class can handle incoming rays and is responsible for
       finding rays intersection with the surface.

       Depending on ray trace intensity mode, it will potentially
       adjust light absorption and then call @ref trace_ray_simple,
       @ref trace_ray_intensity or @ref trace_ray_polarized. These
       functions must be reimplemented in sub classes, default
       implementations will throw.
    */

    class Surface : public Element
    {
    public:
      /** Create a surface element at given location. */
      Surface(const math::VectorPair3 &p,
              const const_ref<curve::Base> &curve,
              const const_ref<shape::Base> &shape);

      virtual ~Surface();

      /** Set surface curve */
      inline void set_curve(const const_ref<curve::Base> &c);
      /** Get surface curve */
      inline const curve::Base & get_curve() const;

      /** Set surface shape */
      inline void set_shape(const const_ref<shape::Base> &s);
      /** Get surface shape */
      inline const shape::Base & get_shape() const;

      /** Get intersection point and normal to surface at
          intersection. Return false if no intersection occured.
      */
      virtual bool intersect(const trace::Params &params,
                             math::VectorPair3 &pt,
                             const math::VectorPair3 &ray) const;

      /** Get distribution pattern points projected on the surface */
      void get_pattern(const math::Vector3::put_delegate_t &f,
                       const trace::Distribution &d,
                       bool unobstructed = false) const;

      /** trace a single ray through the surface */
      template <trace::IntensityMode m>
      void trace_ray(trace::Result &result, trace::Ray &incident,
                     const math::VectorPair3 &local, const math::VectorPair3 &intersect) const;

      /** Get surface apparent color */
      virtual io::Rgb get_color(const io::Renderer &r) const;

      /** Set minimal ray intensity. Incident rays with less intensity
          will be discarded */
      inline void set_discard_intensity(double intensity);
      /** Get minimal ray intensity. */
      inline double get_discard_intensity() const;

      math::VectorPair3 get_bounding_box() const;

    protected:

      /** This function must be reimplemented by subclasses to handle
          incoming rays and generate new ones when in simple ray trace mode. */
      virtual void trace_ray_simple(trace::Result &result, trace::Ray &incident,
                                    const math::VectorPair3 &local, const math::VectorPair3 &intersect) const;

      /** This function must be reimplemented by subclasses to handle
          incoming rays and generate new ones when in intensity ray trace mode. */
      virtual void trace_ray_intensity(trace::Result &result, trace::Ray &incident,
                                       const math::VectorPair3 &local, const math::VectorPair3 &intersect) const;

      /** This function must be reimplemented by subclasses to handle
          incoming rays and generate new ones when in polarized ray trace mode. */
      virtual void trace_ray_polarized(trace::Result &result, trace::Ray &incident,
                                       const math::VectorPair3 &local, const math::VectorPair3 &intersect) const;

      /** @override */
      void draw_2d_e(io::Renderer &r, const Element *ref) const;
      /** @override */
      void draw_3d_e(io::Renderer &r, const Element *ref) const;

    private:

      /** @internal */
      void get_2d_points(math::Vector2 *array,
                         unsigned int count, double start,
                         double end, const Element *ref) const;

      template <trace::IntensityMode m>
      inline void process_rays_(trace::Result &result,
                               trace::rays_queue_t *input) const;

      virtual void process_rays_simple(trace::Result &result,
                                       trace::rays_queue_t *input) const;

      virtual void process_rays_intensity(trace::Result &result,
                                          trace::rays_queue_t *input) const;

      virtual void process_rays_polarized(trace::Result &result,
                                          trace::rays_queue_t *input) const;

      double                    _discard_intensity;
      const_ref<curve::Base>   _curve;
      const_ref<shape::Base>   _shape;
    };

  }
}

#endif

