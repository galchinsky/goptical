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

#include <iostream>

#include <goptical/core/sys/System>
#include <goptical/core/sys/SourceDisk>
#include <goptical/core/sys/Image>
#include <goptical/core/sys/Surface>
#include <goptical/core/sys/OpticalSurface>

#include <goptical/core/trace/Tracer>
#include <goptical/core/trace/Distribution>
#include <goptical/core/trace/Params>
#include <goptical/core/trace/Result>
#include <goptical/core/trace/Sequence>

#include <goptical/core/math/Vector>
#include <goptical/core/math/Quaternion>
#include <goptical/core/math/Transform>

#include <goptical/core/io/RendererSvg>
#include <goptical/core/io/ImportZemax>

#include <goptical/core/light/SpectralLine>

#include <opencv2/opencv.hpp>

using namespace goptical;

template <class X>
class Newton : public X
{

public:
  Newton();

  void redraw();
  void resize(int width, int height);

  io::ImportZemax zemax;
  ref<sys::system> system;
  trace::tracer tracer;

};

template <class X>
Newton<X>::Newton()
  : X(),
    zemax(io::ImportZemax()
          .set_catalog_path(std::string("/home/dmitry/zemax/Glasscat"))),
    system(zemax.import_design("/home/dmitry/geo/347.ZMX")),
    tracer(system)
{

  //  sys::SourceDisk source(sys::SourceAtInfinity, math::vector3_001, math::Vector2(45.0, 45.0));
  sys::SourceDisk source(sys::SourceAtInfinity, math::Vector3(0, 0, 1),
                         math::Vector2(45.0, 45.0));
  source.set_material( dynamic_cast<sys::OpticalSurface*>(&system->get_element(1))->get_material(0) );
  source.single_spectral_line(light::SpectralLine(860.005));

  system->add(source);

  trace::Sequence seq(*system);
  std::cout << *system << std::endl;
  std::cout << seq << std::endl;

  sys::Surface* img;
  int i;
  for (i = 1; i < system->get_element_count(); ++i) {
      try {
      if (img = dynamic_cast<sys::Image*>(&system->get_element(i))) {
          break;
      } }
      catch(...) {
      }
  }

  // first determine entrance pupil
  tracer.get_params().set_sequential_mode(seq);
  tracer.get_trace_result().clear();
  tracer.get_params().set_default_distribution(
      trace::Distribution(trace::HexaPolarDist, 120));
  tracer.get_trace_result().set_generated_save_state(source);
  tracer.get_trace_result().set_intercepted_save_state(*img);
  tracer.trace();
  
  auto pupil_result = tracer.get_trace_result().
      get_intercepted((const sys::Surface&)system->get_element(system->get_element_count()-1));

  for (auto & r : pupil_result) {
      if (r->is_lost()) {
          std::cout << "rays shouldn't be lost" << std::endl;
          exit(-1);
      }
  }

  double min_x = 999999;
  double max_x = -999999;
  double min_y = 999999;
  double max_y = -999999;

  for (auto & r : pupil_result) {
      auto parent = r->get_parent();
      auto current = r;
      while (parent != nullptr) {
          current = parent;
          parent = parent->get_parent();
      }

      auto p = current->get_intercept_point();

      if (p[0] < min_x) {
          min_x = p[0];
      }

      if (p[0] > max_x) {
          max_x = p[0];
      }

      if (p[1] < min_y) {
          min_y = p[1];
      }

      if (p[1] > max_y) {
          max_y = p[1];
      }
  }

  std::cout << min_x << " " << max_x << " " << min_y << " " << max_y << std::endl;

  source.set_limits(math::Vector2(min_x, min_y),
                    math::Vector2(max_x, max_y));

  cv::Mat accum = cv::Mat::zeros(1024, 1024, CV_32F);

  double accum_sum = 0;
  double result_sum = 0;
  double error = 99999;

  int count = 0;

  cv::Point min_loc;
  cv::Point max_loc;

  while (error > 0) {
      tracer.get_trace_result().clear();

      tracer.get_params().set_default_distribution(
          trace::Distribution(trace::RandomDist, 1000));

      tracer.get_trace_result().set_generated_save_state(source);
      tracer.get_trace_result().set_intercepted_save_state(*img);
      tracer.trace();
            
      auto result = tracer.get_trace_result().pixelate(*img);
      
      count = count + 1;

      double min, max;
      if (count == 2) {
          cv::minMaxLoc(accum, &min, &max, &min_loc, &max_loc);
      }

      max = accum.at<float>(max_loc.y, max_loc.x);

      double m1024 = max / 1024.0;

      error = 0;
      for (int i = 512-4; i < 512+4/*result.size()*/; ++i) {
          float* p = accum.ptr<float>(i);
          p += 512 - 4;
          for (int j = 512-4; j < 512+4/*result[0].size()*/; ++j) {
              *p += result[i][j];
              ++p;
              double e = floor(result[i][j] / m1024 - 1);
              if (e < 0) {
                  e = 0;
              }
              error = error + e;
          }
      }

      if (count < 2) {
          error = 9999;
      }

      cv::Mat zoom;
      cv::normalize(accum(cv::Rect(512-4, 512-4, 8, 8)), zoom, 1, 0, cv::NORM_MINMAX);

      cv::resize(zoom, zoom, cv::Size(zoom.rows*64, zoom.cols*64), 0, 0, cv::INTER_NEAREST);

      cv::imshow("test", zoom);
      cv::waitKey(1);

      std::cout << "!" << error << std::endl;

  }

  cv::normalize(accum, accum, 1, 0, cv::NORM_MINMAX);


  {
      io::RendererSvg renderer("layout.svg", 800*2, 400*2);
      system->draw_2d_fit(renderer);
      system->draw_2d(renderer);
      tracer.get_trace_result().draw_2d(renderer);
      std::cout << "done" << std::endl;
  } 

  cv::waitKey(0);
  exit(-1);


  // viewport setup

  system->draw_3d_fit(*X::renderer);

  X::translation = X::renderer->get_camera_transform().get_translation();
  X::rotation = math::Vector3(0, 90, 0);

  X::main_loop();
}

template <class X>
void Newton<X>::redraw()
{
  X::renderer->clear();

  math::Transform<3> t(
         math::Quaternion::angle(math::vector3_100, X::rotation.x()) * 
         math::Quaternion::angle(math::vector3_010, X::rotation.y()) *
         math::Quaternion::angle(math::vector3_001, X::rotation.z()),
         X::translation);

  X::renderer->set_camera_transform(t);

  // draw rays
  tracer.get_trace_result().draw_3d(*X::renderer);
  // draw system
  system->draw_3d(*X::renderer);

  X::renderer->flush();
}

template <class X>
void Newton<X>::resize(int width, int height)
{
  // set output window size
  X::renderer->set_2d_size(width, height);

#if 0
  // uииse perspective projection
  X::renderer->set_perspective();
#else
  // use orthographic projection
  system->draw_2d_fit(*X::renderer);
#endif
}

