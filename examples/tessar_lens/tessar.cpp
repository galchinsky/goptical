/*

      This file is part of the <goptical/core library.
  
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

/* -*- indent-tabs-mode: nil -*- */

#include <iostream>
#include <fstream>

#include <goptical/core/math/Vector>

#include <goptical/core/material/Abbe>

#include <goptical/core/sys/System>
#include <goptical/core/sys/Lens>
#include <goptical/core/sys/Source>
#include <goptical/core/sys/SourceRays>
#include <goptical/core/sys/SourcePoint>
#include <goptical/core/sys/Image>
#include <goptical/core/sys/Stop>

#include <goptical/core/curve/Sphere>
#include <goptical/core/shape/Disk>

#include <goptical/core/trace/Tracer>
#include <goptical/core/trace/Result>
#include <goptical/core/trace/Distribution>
#include <goptical/core/trace/Sequence>
#include <goptical/core/trace/Params>

#include <goptical/core/light/SpectralLine>

#include <goptical/core/analysis/RayFan>
#include <goptical/core/analysis/Spot>
#include <goptical/core/data/Plot>

#include <goptical/core/io/RendererSvg>
#include <goptical/core/io/Rgb>

using namespace goptical;

int main()
{
  //**********************************************************************
  // Optical system definition

  sys::system   sys;

  /* anchor lens */
  sys::Lens     lens(math::Vector3(0, 0, 0));

  //               roc,            ap.radius, thickness,

  lens.add_surface(1/0.031186861,  14.934638, 4.627804137,
                   ref<material::AbbeVd>::create(1.607170, 59.5002));

  lens.add_surface(0,              14.934638, 5.417429465);

  lens.add_surface(1/-0.014065441, 12.766446, 3.728230979,
                   ref<material::AbbeVd>::create(1.575960, 41.2999));

  lens.add_surface(1/0.034678487,  11.918098, 4.417903733);

  lens.add_stop   (                12.066273, 2.288913925);

  lens.add_surface(0,              12.372318, 1.499288597,
                   ref<material::AbbeVd>::create(1.526480, 51.4000));

  lens.add_surface(1/0.035104369,  14.642815, 7.996205852,
                   ref<material::AbbeVd>::create(1.623770, 56.8998));

  lens.add_surface(1/-0.021187519, 14.642815, 85.243965130);

  sys.add(lens);
  /* anchor end */

  sys::Image      image(math::Vector3(0, 0, 125.596), 5);
  sys.add(image);

  /* anchor sources */
  sys::SourceRays  source_rays(math::Vector3(0, 27.5, -1000));

  sys::SourcePoint source_point(sys::SourceAtFiniteDistance,
                                math::Vector3(0, 27.5, -1000));

  // add sources to system
  sys.add(source_rays);
  sys.add(source_point);

  // configure sources
  source_rays.add_chief_rays(sys);
  source_rays.add_marginal_rays(sys, 14);

  source_point.clear_spectrum();
  source_point.add_spectral_line(light::SpectralLine::C);
  source_point.add_spectral_line(light::SpectralLine::e);
  source_point.add_spectral_line(light::SpectralLine::F);
  /* anchor end */

  /* anchor seq */
  trace::Sequence seq(sys);

  sys.get_tracer_params().set_sequential_mode(seq);
  std::cout << "system:" << std::endl << sys;
  std::cout << "sequence:" << std::endl << seq;
  /* anchor end */

  //**********************************************************************
  // Drawing rays and layout

  {
    /* anchor layout */
    io::RendererSvg renderer("layout.svg", 800, 400);

#if 1
    // draw 2d system layout
    sys.draw_2d_fit(renderer);
    sys.draw_2d(renderer);
#else
    // draw 2d layout of lens only
    lens.draw_2d_fit(renderer);
    lens.draw_2d(renderer);
#endif

    trace::tracer tracer(sys);

    // trace and draw rays from rays source
    sys.enable_single<sys::Source>(source_rays);
    tracer.get_trace_result().set_generated_save_state(source_rays);

    tracer.trace();
    tracer.get_trace_result().draw_2d(renderer);
    /* anchor end */
  }

  {
    /* anchor spot */
    sys.enable_single<sys::Source>(source_point);

    sys.get_tracer_params().set_default_distribution(
      trace::Distribution(trace::HexaPolarDist, 12));

    analysis::Spot spot(sys);

    /* anchor end */
    {
    /* anchor spot */
      io::RendererSvg renderer("spot.svg", 300, 300, io::rgb_black);

      spot.draw_diagram(renderer);
    /* anchor end */
    }

    {
    /* anchor spot_plot */
      io::RendererSvg renderer("spot_intensity.svg", 640, 480);

      ref<data::Plot> plot = spot.get_encircled_intensity_plot(50);

      plot->draw(renderer);
    /* anchor end */
    }
  }

  {
    /* anchor opd_fan */
    sys.enable_single<sys::Source>(source_point);

    analysis::RayFan fan(sys);

    /* anchor end */
    {
    /* anchor opd_fan */
      io::RendererSvg renderer("opd_fan.svg", 640, 480);

      ref<data::Plot> fan_plot = fan.get_plot(analysis::RayFan::EntranceHeight,
                                              analysis::RayFan::OpticalPathDiff);

      fan_plot->draw(renderer);

    /* anchor end */
    }

    {
    /* anchor transverse_fan */
      io::RendererSvg renderer("transverse_fan.svg", 640, 480);

      ref<data::Plot> fan_plot = fan.get_plot(analysis::RayFan::EntranceHeight,
                                              analysis::RayFan::TransverseDistance);

      fan_plot->draw(renderer);

    /* anchor end */
    }

    {
    /* anchor longitudinal_fan */
      io::RendererSvg renderer("longitudinal_fan.svg", 640, 480);

      ref<data::Plot> fan_plot = fan.get_plot(analysis::RayFan::EntranceHeight,
                                              analysis::RayFan::LongitudinalDistance);

      fan_plot->draw(renderer);

    /* anchor end */
    }
  }

  return 0;
}

