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


#include <goptical/core/analysis/Spot>
#include <goptical/core/sys/Image>

#include <goptical/core/trace/Tracer>
#include <goptical/core/trace/Ray>
#include <goptical/core/trace/Result>
#include <goptical/core/trace/Distribution>

#include <goptical/core/io/RendererViewport>
#include <goptical/core/io/RendererAxes>

#include <goptical/core/data/PlotData>
#include <goptical/core/data/Plot>
#include <goptical/core/data/SampleSet>

#include <goptical/core/light/SpectralLine>

namespace _goptical
{

  namespace analysis
  {

    Spot::Spot(sys::system &system)
      : PointImage(system),
        _processed_analysis(false)
    {
      _axes.set_show_axes(false, io::RendererAxes::XY);
      _axes.set_label("Saggital distance", io::RendererAxes::X);
      _axes.set_label("Tangential distance", io::RendererAxes::Y);
      _axes.set_unit("m", true, true, -3, io::RendererAxes::XY);
    }

    void Spot::process_trace()
    {
      if (_processed_trace)
        return;

      trace();

      _centroid = _tracer.get_trace_result().get_intercepted_centroid(*_image);
    }

    void Spot::process_analysis()
    {
      if (_processed_analysis)
        return;

      process_trace();

      double    mean = 0;       // rms radius
      double    max = 0;        // max radius
      double    intensity = 0;  // total intensity

      GOPTICAL_FOREACH(i, *_intercepts)
        {
          double        dist = ((*i)->get_intercept_point() - _centroid).len();

          if (max < dist)
            max = dist;

          mean += math::square(dist);
          intensity += (*i)->get_intensity();
        }

      _useful_radius = _max_radius = max;
      _rms_radius = sqrt(mean / _intercepts->size());
      _tot_intensity = intensity;

      _processed_analysis = true;
    }

    double Spot::get_encircled_intensity(double radius)
    {
      process_trace();

      double    intensity = 0;

      GOPTICAL_FOREACH(i, *_intercepts)
        {
          if (((*i)->get_intercept_point() - _centroid).len() <= radius)
            intensity += (*i)->get_intensity();
        }

      return intensity;
    }

    ref<data::Plot> Spot::get_encircled_intensity_plot(int zones)
    {
      trace::Result &result = _tracer.get_trace_result();
      const trace::rays_queue_t &intercepts = result.get_intercepted(*_image);

      process_analysis();

      if (intercepts.empty())
        throw Error("no ray intercept found for encircled intensity plot");

      typedef std::map<double, ref<data::SampleSet> > data_sets_t;
      data_sets_t data_sets;

      // create plot data for each wavelen

      GOPTICAL_FOREACH(w, result.get_ray_wavelen_set())
        {
          ref<data::SampleSet> s = GOPTICAL_REFNEW(data::SampleSet);

          s->set_interpolation(data::Linear);
          s->set_metrics(0.0, _useful_radius / (double)zones);
          s->resize(zones + 1);

          data_sets.insert(data_sets_t::value_type(*w, s));
        }

      // compute encircled intensity for each radius range

      GOPTICAL_FOREACH(i, *_intercepts)
        {
          double dist = ((*i)->get_intercept_point() - _centroid).len();

          if (dist > _useful_radius)
            continue;

          int n = (unsigned int)((zones - 1) * (dist / _useful_radius));

          assert(n >= 0 && n < zones);

          data_sets[(*i)->get_wavelen()]->get_y_value(n + 1) += (*i)->get_intensity();
        }

      // integrate

      ref<data::Plot> plot = GOPTICAL_REFNEW(data::Plot);

      GOPTICAL_FOREACH(d, data_sets)
        {
          for (int i = 1; i < zones; i++)
            d->second->get_y_value(i + 1) += d->second->get_y_value(i);

          data::Plotdata p(*d->second);

          //      p.set_label("Encircled ray intensity"); FIXME set wavelen
          p.set_color(light::SpectralLine::get_wavelen_color(d->first));
          p.set_style(data::LinePlot);

          plot->add_plot_data(p);
        }

      plot->set_title("Spot diagram encircled rays intensity");
      plot->get_axes().set_label("Distance from spot centroid", io::RendererAxes::X);
      plot->get_axes().set_label("Encircled intensity", io::RendererAxes::Y);
      plot->get_axes().set_unit("m", true, true, -3, io::RendererAxes::X);
      plot->get_axes().set_unit("", false, false, 0, io::RendererAxes::Y);

      return plot;
    }

    math::Vector3 Spot::get_center()
    {
      process_analysis();

      return _tracer.get_trace_result().get_intercepted_center(*_image);
    }

    void Spot::draw_spot(io::RendererViewport &renderer)
    {
      process_analysis();

      trace::Result &result = _tracer.get_trace_result();
      renderer.draw_intercepts(result, *_image);
    }

    void Spot::draw_diagram(io::RendererViewport &renderer, bool centroid_origin)
    {
      process_analysis();

      trace::Result &result = _tracer.get_trace_result();
      math::Vector2 center(result.get_intercepted_center(*_image), 0, 1);
      math::Vector2 radius(_useful_radius, _useful_radius);

      renderer.set_window(math::VectorPair2(center - radius, center + radius));

      _axes.set_position(_centroid);
      _axes.set_origin(centroid_origin ? _centroid : math::vector3_0);
      _axes.set_tics_count(3, io::RendererAxes::XY);
      renderer.draw_axes_2d(_axes);

      renderer.draw_intercepts(result, *_image);
    }

  }
}

