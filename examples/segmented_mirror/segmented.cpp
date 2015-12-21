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

#include <goptical/core/sys/System>
#include <goptical/core/sys/Mirror>
#include <goptical/core/sys/Image>
#include <goptical/core/sys/Stop>
#include <goptical/core/sys/Group>
#include <goptical/core/sys/SourcePoint>

#include <goptical/core/shape/Base>
#include <goptical/core/shape/Disk>
#include <goptical/core/shape/Ring>
#include <goptical/core/shape/RegularPolygon>

#include <goptical/core/curve/Base>
#include <goptical/core/curve/Composer>
#include <goptical/core/curve/Conic>

#include <goptical/core/trace/Tracer>
#include <goptical/core/trace/Distribution>
#include <goptical/core/trace/Params>
#include <goptical/core/trace/Result>

#include <goptical/core/io/RendererX3d>
#include <goptical/core/io/Renderer>

#include <goptical/core/math/VectorPair>
#include <goptical/core/math/Vector>

#include <goptical/core/Error>

using namespace goptical;

/* anchor hexseg1 */
class HexSegMirror : public sys::Group
{
public:

  HexSegMirror(const math::VectorPair3 &pos,
               const const_ref<curve::Base> &curve,
               const const_ref<shape::Base> &shape,
               double seg_radius, double separation)
    : sys::Group(pos)
  {
/* anchor hexseg2 */
    if (seg_radius > separation)
      throw(Error("overlapping segments"));

    // sqrt(3)/2
    static const double sqrt_3_2 = 0.86602540378443864676;

    // hexagonal tessellation
    int x_count = ceil(shape->max_radius() / (separation * 1.5));
    int y_count = ceil(shape->max_radius() / (separation * 2 * sqrt_3_2));

    for (int x = -x_count; x <= x_count ; x++)
      {
        for (int y = -y_count; y <= y_count ; y++)
          {
            // find segment mirror 2d position
            double yoffset = x % 2 ? separation * sqrt_3_2 : 0;
            math::Vector2 p(x * separation * 1.5,
                              yoffset + y * separation * 2 * sqrt_3_2 );
/* anchor hexseg3 */
            // skip if segment center is outside main shape
            if (!shape->inside(p))
              continue;
/* anchor hexseg4 */
            // find curve z offset at segment center to shift both
            // curve and segment in opposite directions.
            double z_offset = curve->sagitta(p);

            // create a composer curve for this segment and use it to translate main curve
            ref<curve::Composer> seg_curve = ref<curve::Composer>::create();

            seg_curve->add_curve(curve).xy_translate(-p).z_offset(-z_offset);
/* anchor hexseg5 */
            // create a segment mirror with hexagonal shape and translated curve
            ref<sys::Mirror> seg = ref<sys::Mirror>::create(math::Vector3(p, z_offset), seg_curve,
                                             ref<shape::RegularPolygon>::create(seg_radius, 6));

            // attach the new segment to our group component
            add(seg);
/* anchor hexseg6 */
            // keep a pointer to this new segment
            _segments.push_back(seg.ptr());
          }
      }
  }

  size_t get_segments_count() const
  {
    return _segments.size();
  }

  sys::Mirror & get_segment(size_t i) const
  {
    return *_segments.at(i);
  }

private:
  std::vector<sys::Mirror *> _segments;
};
/* anchor end */

int main()
{

  // system focal length                           2400
  // Back focal length                              100

  // Primary mirror diameter                        300
  // Primary mirror focal length                    800
  // Primary mirror conic constant              -1.0869

  // Distance secondary from system focus           675
  // Distance secondary from prime focus            225
  // Distance secondary from primary                575
  // Radius of curvature of secondary mirror        675
  // Secondary mirror conic constant            -5.0434

  /* anchor rc */
  sys::system             sys;

  // Ring shaped segmented mirror with conic curve
  HexSegMirror            primary(math::Vector3(0, 0, 800),
                                  ref<curve::Conic>::create(-1600, -1.0869),
                                  ref<shape::Ring>::create(300, 85),
                                  28, 30);
  sys.add(primary);

  sys::Mirror             secondary(math::VectorPair3(0, 0, 225, 0, 0, -1), 675, -5.0434, 100);
  sys.add(secondary);

  sys::Image              image(math::VectorPair3(0, 0, 900), 15);
  sys.add(image);

  sys::Stop               stop(math::vector3_0, 300);
  sys.add(stop);
  sys.set_entrance_pupil(stop);

  sys::SourcePoint        source(sys::SourceAtInfinity, math::vector3_001);
  sys.add(source);
  /* anchor end */

//  primary.get_segment(5).rotate(0.001, 0, 0);

  /* anchor layout */
  trace::tracer         tracer(sys);

  // trace rays through the system
  tracer.get_trace_result().set_generated_save_state(source);
  tracer.trace();

  io::RendererX3d       x3d_renderer("layout.x3d");
  io::Renderer          &renderer = x3d_renderer;

  sys.draw_3d(renderer);
  tracer.get_trace_result().draw_3d(renderer, true);
  /* anchor end */

  return 0;
}

