/*

      This file is part of the <goptical/core Design library.
  
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

#include <goptical/core/math/Vector>

#include <goptical/core/sys/system>
#include <goptical/core/sys/SourcePoint>
#include <goptical/core/Design/telescope/Cassegrain>
#include <goptical/core/sys/Image>

#include <goptical/core/trace/Tracer>
#include <goptical/core/trace/Result>
#include <goptical/core/trace/Distribution>
#include <goptical/core/trace/Params>
#include <goptical/core/trace/Sequence>

#include <goptical/core/io/RendererSvg>
#include <goptical/core/io/RendererViewport>

using namespace goptical;

#define COMPARE(a, b)                                                   \
  if (fabs((a)-(b)) > 1e-4)                                             \
    {                                                                   \
      std::cerr << "at " << __LINE__ << a << " " << b << std::endl;     \
      exit(1);                                                          \
    }

int main()
{
#if 0
  sys::system             sys;

  sys::SourcePoint        source(sys::SourceAtInfinity, math::vector3_001);
  sys.add(source);
  //  source.rotate(1.4, 0, 0);

  Design::telescope::Cassegrain<Design::telescope::RitcheyChretien>
    telescope(math::vector3_0, 2000, 8./3., 250, 100, 2);
  sys.add(telescope);

  sys::Image              image(telescope.get_focal_plane(), telescope.get_unvignetted_image_diameter() / 2);
  sys.add(image);

  trace::Sequence seq(sys);
  std::cout << seq;

  trace::tracer         tracer(sys);

  sys.set_entrance_pupil(telescope.get_primary());

  tracer.get_params().set_default_distribution(trace::Distribution(trace::CrossDist, 5));
  tracer.get_trace_result().set_generated_save_state(source);
  tracer.trace();

  io::RendererSvg       svg_renderer("cassegrain_layout.svg", 700, 300);
  io::RendererViewport  &renderer = svg_renderer;

  sys.draw_2d_fit(renderer);
  sys.draw_2d(renderer);

  tracer.get_trace_result().draw_2d(renderer);
#endif

  {
    Design::telescope::Cassegrain<Design::telescope::RitcheyChretien>
      telescope(math::vector3_0, 2000, 8./3., 250, 100, 2);

    COMPARE(telescope.get_primary_conic(), -1.12582);
    COMPARE(telescope.get_secondary_conic(), -6.50737);
  }

  {
    Design::telescope::Cassegrain<Design::telescope::ClassicalCassegrain>
      telescope(math::vector3_0, 2000, 8./3., 250, 100, 2);

    COMPARE(telescope.get_primary_conic(), -1);
    COMPARE(telescope.get_secondary_conic(), -4.84);
  }

  {
    Design::telescope::Cassegrain<Design::telescope::DallKirkham>
      telescope(math::vector3_0, 2000, 8./3., 250, 100, 2);

    COMPARE(telescope.get_primary_conic(), -0.6347);
    COMPARE(telescope.get_secondary_conic(), 0);
  }

  {
    Design::telescope::Cassegrain<Design::telescope::PressmannCamichel>
      telescope(math::vector3_0, 2000, 8./3., 250, 100, 2);

    COMPARE(telescope.get_primary_conic(), 0);
    COMPARE(telescope.get_secondary_conic(), 8.4117);
  }

  return 0;
}

