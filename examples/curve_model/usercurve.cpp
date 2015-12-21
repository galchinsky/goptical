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

#include <goptical/core/sys/System>
#include <goptical/core/sys/SourcePoint>
#include <goptical/core/sys/Mirror>
#include <goptical/core/sys/Image>

#include <goptical/core/shape/Disk>

#include <goptical/core/curve/Rotational>

#include <goptical/core/trace/Tracer>
#include <goptical/core/trace/Sequence>
#include <goptical/core/trace/Distribution>
#include <goptical/core/trace/Params>

#include <goptical/core/analysis/Spot>
#include <goptical/core/analysis/Focus>

#include <goptical/core/io/RendererPlplot>
#include <goptical/core/io/RendererSvg>
#include <goptical/core/io/Rgb>

using namespace goptical;

                                                                  /* anchor mycurve1 */
class MyCatenarycurve : public curve::Rotational
{
public:
  MyCatenarycurve(double a)
    : _a(a)
  {
  }

private:
  double sagitta(double r) const
  {
    return _a * cosh(r / _a) - _a;
  }
                                                                  /* anchor mycurve2 */
  double derivative(double r) const
  {
    return sinh(r / _a);
  }
                                                                  /* anchor mycurve1 */

  double _a;
};
                                                                  /* anchor end */

int main()
{
  //**********************************************************************
  // Optical system definition

                                                                  /* anchor system */
  sys::system             sys;

  // light source
  sys::SourcePoint        source(sys::SourceAtInfinity, math::vector3_001);
  sys.add(source);

  // mirror
  shape::Disk             shape(200);
  MyCatenarycurve         curve(-3000);
  sys::Mirror             primary(math::Vector3(0, 0, 1500), curve, shape);

  sys.add(primary);

  // image plane
  sys::Image              image(math::vector3_0, 15);
  sys.add(image);
                                                                  /* anchor end */

  // set system entrance pupil
  sys.set_entrance_pupil(primary);


  trace::Sequence seq;
  seq.append(source);
  seq.append(primary);
  seq.append(image);

  sys.get_tracer_params().set_sequential_mode(seq);
  sys.get_tracer_params().set_default_distribution(
    trace::Distribution(trace::HexaPolarDist, 8));


  {
                                                                  /* anchor focus */
    analysis::Focus               focus(sys);

    image.set_plane(focus.get_best_focus());
                                                                  /* anchor end */
  }

  std::cout << seq << std::endl;

  {
                                                                  /* anchor spot */
    io::RendererSvg            renderer("spot.svg",        200 * 3, 200 * 2, io::rgb_black);

    renderer.set_margin_ratio(.35, .25, .1, .1);
    renderer.set_page_layout(3, 2);

    for (int i = 0; i < 6; i++)
      {
        analysis::Spot spot(sys);

        renderer.set_page(i);
        spot.draw_diagram(renderer);

        source.rotate(0, .1, 0);
      }
                                                                  /* anchor end */
    return 0;
  }

}

