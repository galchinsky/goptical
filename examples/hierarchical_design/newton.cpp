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

#include <goptical/core/material/Sellmeier>

#include <goptical/core/sys/System>
#include <goptical/core/sys/SourcePoint>
#include <goptical/design/telescope/Newton>
#include <goptical/core/sys/Image>
#include <goptical/core/sys/Lens>

#include <goptical/core/shape/Disk>

#include <goptical/core/trace/Tracer>
#include <goptical/core/trace/Result>
#include <goptical/core/trace/Distribution>
#include <goptical/core/trace/Params>

#include <goptical/core/io/RendererSvg>
#include <goptical/core/io/RendererViewport>

using namespace goptical;

int main()
{
  material::Sellmeier bk7(1.03961212, 6.00069867e-3, 0.231792344,
                          2.00179144e-2, 1.01046945, 1.03560653e2);

  //**********************************************************************
  // Optical system definition

                                                                  /* anchor telescope */
  sys::system             sys;

  // light source
  sys::SourcePoint        source(sys::SourceAtInfinity, math::vector3_001);
  sys.add(source);

  // Newton telescope
  Design::telescope::Newton newton(math::vector3_0, // position
                                   1494.567 / 2.,   // focal len
                                   245.1);          // aperture diameter
  sys.add(newton);
                                                                  /* anchor corrector */

  // Wynne 4 lens corrector for parabolic mirrors
  sys::Lens               wynne(newton.get_focal_plane(),
                                -48.4585);        // z offset of first surface

                //  roc       ap.radius  thickness  material
  wynne.add_surface(21.496,   23.2 / 2., 1.905,     bk7);
  wynne.add_surface(24.787,   22.5 / 2., 1.574         );
  wynne.add_surface(55.890,   22.5 / 2., 1.270,     bk7);
  wynne.add_surface(45.164,   21.8 / 2., 18.504        );
  wynne.add_surface(29.410,   14.7 / 2., 0.45,      bk7);
  wynne.add_surface(13.870,   14.1 / 2., 16.086        );
  wynne.add_surface(23.617,   13.1 / 2., 1.805,     bk7);
  wynne.add_surface(0,        12.8 / 2., 9.003);

  sys.add(wynne);

  // image plane
  sys::Image              image(wynne.get_exit_plane(), 15);
  sys.add(image);

  /* anchor end */

  //**********************************************************************
  // Display some newton telescope parameters

                                                                  /* anchor print */
  std::cout << "unvignetted image diameter: "
            << newton.get_unvignetted_image_diameter() << std::endl;

  std::cout << "secondary minor axis size: "
            << newton.get_secondary_minor_axis() << std::endl;

  std::cout << "secondary offset: "
            << newton.get_secondary_offset() << std::endl;

  std::cout << "field angle: "
            << newton.get_field_angle() << std::endl;
                                                                  /* anchor end */

  //**********************************************************************
  // Render optical layout with rays in svg file


  {
  trace::tracer         tracer(sys);

  // set system entrance pupil (needed by non-sequential ray trace)
  sys.set_entrance_pupil(newton.get_primary());

  // trace rays through the system
  tracer.get_params().set_default_distribution(trace::Distribution(trace::CrossDist, 5)); 
  tracer.get_trace_result().set_generated_save_state(source);
  tracer.trace();

  /* anchor layout */
  io::RendererSvg       svg_renderer("layout.svg", 640, 480);
  io::RendererViewport  &renderer = svg_renderer;

  // horizontal page layout
  renderer.set_page_layout(1, 2);

  // 3d system layout on 1st sub-page
  renderer.set_page(0);
  renderer.set_perspective();

  sys.draw_3d_fit(renderer, 300);
  sys.draw_3d(renderer);

  tracer.get_trace_result().draw_3d(renderer);

  // 2d Wynne corrector layout on 2nd sub-page
  renderer.set_page(1);

  wynne.draw_2d_fit(renderer);
  wynne.draw_2d(renderer);

  tracer.get_trace_result().draw_2d(renderer, false, &wynne);
  /* anchor end */
  }

  return 0;
}

