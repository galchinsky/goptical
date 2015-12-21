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

#include <goptical/core/sys/Stop>

#include <goptical/core/trace/Result>
#include <goptical/core/trace/Params>
#include <goptical/core/trace/Ray>

#include <goptical/core/math/Vector>
#include <goptical/core/math/VectorPair>

#include <goptical/core/shape/Base>
#include <goptical/core/shape/Disk>
#include <goptical/core/curve/Base>
#include <goptical/core/curve/Flat>

#include <goptical/core/io/Renderer>

namespace _goptical {

  namespace sys {

    Stop::Stop(const math::VectorPair3 &p, const const_ref<shape::Base> &shape)
      : Surface(p, curve::flat, shape),
        _intercept_reemit(false)
    {
      _external_radius = shape->max_radius() * 2.0;
    }

    Stop::Stop(const math::VectorPair3 &p, double r)
      : Surface(p, curve::flat, ref<shape::Disk>::create(r)),
        _intercept_reemit(false)
    {
      _external_radius = r * 2.0;
    }

    bool Stop::intersect(const trace::Params &params,
                         math::VectorPair3 &intersect,
                         const math::VectorPair3 &ray) const
    {
      if (!get_curve().intersect(intersect.origin(), ray))
        return false;

      math::Vector2 v(intersect.origin().project_xy());

      if (v.len() > _external_radius)
        return false;

      bool ir = _intercept_reemit || params.is_sequential();

      if (!ir && get_shape().inside(v))
        return false;

      get_curve().normal(intersect.normal(), intersect.origin());
      if (ray.direction().z() < 0)
        intersect.normal() = -intersect.normal();

      return true;
    }

    inline void Stop::trace_ray_simple(trace::Result &result, trace::Ray &incident,
                                       const math::VectorPair3 &local, const math::VectorPair3 &intersect) const
    {
      math::Vector2 v = intersect.origin().project_xy();

      bool ir = _intercept_reemit || result.get_params().is_sequential();

      if (ir && get_shape().inside(v))
        {
          // reemit incident ray
          trace::Ray &r = result.new_ray();

          r.set_wavelen(incident.get_wavelen());
          r.set_intensity(incident.get_intensity());
          r.set_material(incident.get_material());
          r.origin() = intersect.origin();
          r.direction() = incident.direction();
          r.set_creator(this);

          incident.add_generated(&r);
        }
    }

    void Stop::trace_ray_intensity(trace::Result &result, trace::Ray &incident,
                                   const math::VectorPair3 &local, const math::VectorPair3 &intersect) const
    {
      trace_ray_simple(result, incident, local, intersect);
    }

    template <trace::IntensityMode m>
    inline void Stop::process_rays_(trace::Result &result,
                                    trace::rays_queue_t *input) const
    {
      for (auto&i : *input)
        {
          math::VectorPair3 intersect;
          trace::Ray  &ray = *i;

          const math::Transform<3> &t = ray.get_creator()->get_transform_to(*this);
          math::VectorPair3 local(t.transform_line(ray));

          if (get_curve().intersect(intersect.origin(), local))
            {
              if (intersect.origin().project_xy().len() < _external_radius)
                {
                  get_curve().normal(intersect.normal(), intersect.origin());

                  if (local.direction().z() < 0)
                    intersect.normal() = -intersect.normal();

                  result.add_intercepted(*this, ray);

                  trace_ray<m>(result, ray, local, intersect);
                }
            }
        }
    }

    void Stop::process_rays_simple(trace::Result &result,
                                   trace::rays_queue_t *input) const
    {
      process_rays_<trace::Simpletrace>(result, input);
    }

    void Stop::process_rays_intensity(trace::Result &result,
                                      trace::rays_queue_t *input) const
    {
      process_rays_<trace::Intensitytrace>(result, input);
    }

    void Stop::process_rays_polarized(trace::Result &result,
                                      trace::rays_queue_t *input) const
    {
      process_rays_<trace::Polarizedtrace>(result, input);
    }

    void Stop::draw_3d_e(io::Renderer &r, const Element *ref) const
    {
      r.group_begin();
      std::vector<math::Vector3 > poly;

      auto d = [&](const math::Vector2 &t) {
        poly.push_back(get_transform_to(ref).transform(math::Vector3(t, 0.)));
      };

      for (unsigned int i = 0; i < get_shape().get_contour_count(); i++)
        {
          get_shape().get_contour(i, d, r.get_feature_size());
          r.draw_polygon(&poly[0], poly.size(), get_color(r));
          poly.clear();
        }

      r.group_end();
    }

    void Stop::draw_2d_e(io::Renderer &r, const Element *ref) const
    {
      math::Vector3 mr(0, _external_radius, 0);
      math::Vector3 top(0, get_shape().get_outter_radius(math::vector2_01), 0);
      math::Vector3 bot(0, -get_shape().get_outter_radius(-math::vector2_01), 0);

      const math::Transform<3> t(get_transform_to(ref));

      r.group_begin();
      r.draw_segment(t.transform(top), t.transform(mr), get_color(r));
      r.draw_segment(t.transform(bot), t.transform(-mr), get_color(r));
      r.group_end();
    }

  }

}

