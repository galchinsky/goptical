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

#include <goptical/core/sys/System>
#include <goptical/core/sys/Surface>
#include <goptical/core/sys/Image>

#include <goptical/core/analysis/RayFan>

#include <goptical/core/data/Plot>
#include <goptical/core/data/PlotData>
#include <goptical/core/data/DiscreteSet>

#include <goptical/core/io/RendererViewport>
#include <goptical/core/io/RendererAxes>

#include <goptical/core/trace/Tracer>
#include <goptical/core/trace/Distribution>
#include <goptical/core/trace/Result>
#include <goptical/core/trace/Params>
#include <goptical/core/trace/Ray>

#include <goptical/core/light/SpectralLine>

namespace _goptical
{

  namespace analysis
  {

    RayFan::RayFan(const sys::system &system, enum rayfan_plane_e plane)
      : _tracer(system),
        _processed_trace(false),
        _entrance(0),
        _exit(0),
        _dist(trace::SagittalDist, 15)
    {
      set_plane(plane);
    }

    void RayFan::set_plane(enum rayfan_plane_e plane)
    {
      if (plane == SagittalAberration)
        _dist.set_pattern(trace::SagittalDist);
      else
        _dist.set_pattern(trace::TangentialDist);

      _dist_plane = _ab_plane = plane;

      invalidate();
    }

    void RayFan::process_trace()
    {
      if (!_processed_trace)
        {
          trace::Result &result = _tracer.get_trace_result();
          const sys::system &sys = _tracer.get_system();

          if (!_entrance)
            _entrance = &sys.get_entrance_pupil();
          else
            throw Error("no suitable entrance pupil found for analysis");

          if (!_exit && sys.has_exit_pupil())
            _exit = &sys.get_exit_pupil();
          else
            _exit = sys.find<const sys::Image>();

          if (!_exit)
            throw Error("no suitable exit surface found for analysis");

          result.clear_save_states();
          result.set_intercepted_save_state(*_exit, true);

          _tracer.get_params().set_distribution(*_entrance, _dist);
          _tracer.get_params().set_unobstructed(true);
          _tracer.trace();

          _processed_trace = true;
        }
    }

    void RayFan::invalidate()
    {
      _processed_trace = false;
    }


    ////////////////////////////////////////////////////////////////////////
    // Aberrations evaluation functions

    double RayFan::get_entrance_height(const trace::Ray &r, const trace::Ray &chief) const
    {
      const trace::Ray *ray = &r;

      // walk up to entrance pupil generated ray
      while (ray && ray->get_creator() != _entrance)
        ray = ray->get_parent();

      if (!ray)
        throw Error();

      return ray->origin()[_dist_plane] / _entrance->get_shape()
        .get_outter_radius(math::Vector2(1-_dist_plane, _dist_plane));
    }

    double RayFan::get_entrance_angle(const trace::Ray &r, const trace::Ray &chief) const
    {
      const trace::Ray *ray = &r;

      // walk up to entrance pupil generated ray
      while (ray && ray->get_creator() != _entrance)
        ray = ray->get_parent();

      // find parent ray once again
      if (ray && (ray = ray->get_parent()))
        return math::rad2degree(atan(ray->direction()[_dist_plane] / ray->direction().z()));

      throw Error();
    }

    double RayFan::get_transverse_distance(const trace::Ray &r, const trace::Ray &chief) const
    {
      return r.get_intercept_point()[_ab_plane] - chief.get_intercept_point()[_ab_plane];
    }

    double RayFan::get_longitudinal_distance(const trace::Ray &r, const trace::Ray &chief) const
    {
      if (&r == &chief)
        throw Error();

      return chief.ln_ln_clst_pt_scale(r) - chief.get_len();
    }

    double RayFan::get_exit_angle(const trace::Ray &r, const trace::Ray &chief) const
    {
      const trace::Ray *ray = r.get_first_child();

      if (!ray)
        throw Error();

      return math::rad2degree(atan(ray->direction()[_ab_plane] / ray->direction().z()));
    }

    double RayFan::get_image_angle(const trace::Ray &r, const trace::Ray &chief) const
    {
      return math::rad2degree(atan(r.direction()[_ab_plane] / r.direction().z()));
    }

    double RayFan::get_optical_path_len(const trace::Ray &r, const trace::Ray &chief) const
    {
      const double wl = r.get_wavelen();
      double dist = 0.0;

      for (const trace::Ray *ray = &r; ray; ray = ray->get_parent())
        dist += ray->get_len() * ray->get_material()->get_refractive_index(wl);

      return dist / (wl * 1e-6); // opl in wave unit
    }


    ////////////////////////////////////////////////////////////////////////
    // Aberrations plot generation

    const trace::Ray & RayFan::find_chief_ray(const trace::rays_queue_t &intercepts, double wavelen)
    {
      for(auto& i : intercepts)
        {
          trace::Ray & ray = *i;

          if (ray.get_wavelen() == wavelen && fabs(get_entrance_height(ray, /* dummy */ ray)) < 1e-8)
            return ray;
        }

      throw Error("unable to find chief ray intercept");
    }

    ref<data::Plot> RayFan::get_plot(enum rayfan_plot_type_e x,
                                     enum rayfan_plot_type_e y)
    {
      ref<data::Plot> plot = GOPTICAL_REFNEW(data::Plot);

      plot->get_axes().set_position(math::vector3_0);

      // select X axis evaluation function

      get_value_t get_x_value;
      bool single_x_reference = true;

      switch (x)
        {
        case EntranceHeight:
          get_x_value = &RayFan::get_entrance_height;
          plot->get_axes().set_range(math::range_t(-1.0, 1.0), io::RendererAxes::X);
          plot->get_axes().set_tics_step(1.0, io::RendererAxes::X);
          break;

        case EntranceAngle:
          get_x_value = &RayFan::get_entrance_angle;
          break;

        case ImageAngle:
          get_x_value = &RayFan::get_image_angle;
          break;

        case ExitAngle:
          get_x_value = &RayFan::get_exit_angle;
          break;

        default:
          throw Error("bad value type for X plot axis");
        }


      // select Y axis evaluation function

      get_value_t get_y_value;
      bool single_y_reference = true;

      switch (y)
        {
        case EntranceHeight:
          get_y_value = &RayFan::get_entrance_height;
          break;

        case EntranceAngle:
          get_y_value = &RayFan::get_entrance_angle;
          break;

        case TransverseDistance:
          get_y_value = &RayFan::get_transverse_distance;
          break;

        case LongitudinalDistance:
          get_y_value = &RayFan::get_longitudinal_distance;
          break;

        case OpticalPathDiff:
          get_y_value = &RayFan::get_optical_path_len;
          single_y_reference = false;
          break;

        case ImageAngle:
          get_y_value = &RayFan::get_image_angle;
          break;

        case ExitAngle:
          get_y_value = &RayFan::get_exit_angle;
          break;

        default:
          throw Error("bad value type for Y plot axis");
        }

      // process ray tracing

      process_trace();

      trace::Result &result = _tracer.get_trace_result();
      const trace::rays_queue_t &intercepts = result.get_intercepted(*_exit);

      if (intercepts.empty() || result.get_ray_wavelen_set().empty())
        throw Error("no raytracing data available for analysis");

      bool first = true;
      double x_ref = 0.0, y_ref = 0.0;

      // extract data for each wavelen

      for(auto& w : result.get_ray_wavelen_set())
        {
          const trace::Ray & chief_ray = find_chief_ray(intercepts, w);

          // get chief ray reference values

          if (!single_x_reference || first)
            try {
              x_ref = (this->*get_x_value)(chief_ray, chief_ray);
            } catch (...) {
              x_ref = 0.0;      // no valid data for chief ray
            }

          if (!single_y_reference || first)
            try {
              y_ref = (this->*get_y_value)(chief_ray, chief_ray);
            } catch (...) {
              y_ref = 0.0;      // no valid data for chief ray
            }

          first = false;

          // extract data for each ray

          ref<data::DiscreteSet> s = GOPTICAL_REFNEW(data::DiscreteSet);
          s->set_interpolation(data::Cubic);

          for(auto& i : intercepts)
            {
              trace::Ray & ray = *i;

              if (ray.get_wavelen() != w)
                continue;

              double x_val, y_val;

              try {
                x_val = (this->*get_x_value)(ray, chief_ray) - x_ref;
                y_val = (this->*get_y_value)(ray, chief_ray) - y_ref;

              } catch (...) {

                // no valid data for this ray
                continue;
              }

              // add data to plot
              s->add_data(x_val, y_val);
            }

          if (!s->get_count())
            continue;

          data::Plotdata p(*s);
          p.set_color(light::SpectralLine::get_wavelen_color(w));
          plot->add_plot_data(p);
        }

      static const struct
      {
        const char *label;
        const char *unit;
        bool prefix;
        int pow10;
      } axis[] =
        {
          { "Entrance ray height (normalized)", "", false },
          { "Entrance ray angle", "degree", false },
          { "Transverse ray aberration", "m", true, -3 },
          { "Longitudinal ray aberration", "m", true, -3 },
          { "Image ray angle", "degree", false },
          { "Exit ray angle", "degree", false },
          { "Optical path difference", "waves", false }
        };

      plot->set_title(axis[y].label + std::string(" fan (") +
                      (_dist_plane == SagittalAberration ?
                       "sagittal)" : "tangential)"));
      plot->get_axes().set_label(axis[x].label, io::RendererAxes::X);
      plot->get_axes().set_label(axis[y].label, io::RendererAxes::Y);
      plot->get_axes().set_unit(axis[x].unit, true, axis[x].prefix,
                                axis[x].pow10, io::RendererAxes::X);
      plot->get_axes().set_unit(axis[y].unit, true, axis[y].prefix,
                                axis[y].pow10, io::RendererAxes::Y);
      plot->set_style(data::InterpolatePlot);

      return plot;
    }

  }

}

