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

#include <goptical/core/sys/System>
#include <goptical/core/sys/SourcePoint>
#include <goptical/core/sys/Image>
#include <goptical/core/sys/Surface>

#include <goptical/core/trace/Tracer>
#include <goptical/core/trace/Distribution>
#include <goptical/core/trace/Params>
#include <goptical/core/trace/Result>
#include <goptical/core/trace/Sequence>

#include <goptical/core/math/Vector>
#include <goptical/core/math/Quaternion>
#include <goptical/core/math/Transform>

#include <goptical/design/telescope/Newton>

using namespace goptical;

template <class X>
class Newton : public X
{

public:
  Newton();

  void redraw();
  void resize(int width, int height);

  sys::system sys;
  trace::tracer tracer;
};

template <class X>
Newton<X>::Newton()
  : X(),
    sys(),
    tracer(sys)
{
  // optical system

  sys::SourcePoint      source(sys::SourceAtInfinity, math::vector3_001);
  sys.add(source);

  Design::telescope::Newton     newton(math::vector3_0, 750, 250);
  sys.add(newton);

  sys::Image            image(newton.get_focal_plane(), 15);
  sys.add(image);

  exit(-1);

  // ray trace

  tracer.get_params().set_default_distribution(
    trace::Distribution(trace::HexaPolarDist, 3));

  tracer.get_trace_result().set_generated_save_state(source);
  tracer.trace();

  // viewport setup

  sys.draw_3d_fit(*X::renderer);

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
  sys.draw_3d(*X::renderer);

  X::renderer->flush();
}

template <class X>
void Newton<X>::resize(int width, int height)
{
  // set output window size
  X::renderer->set_2d_size(width, height);

#if 0
  // use perspective projection
  X::renderer->set_perspective();
#else
  // use orthographic projection
  sys.draw_2d_fit(*X::renderer);
#endif
}

