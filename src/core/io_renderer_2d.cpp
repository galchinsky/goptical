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

#include <goptical/core/io/Renderer2d>

namespace _goptical {

  namespace io {

    Renderer2d::Renderer2d()
    {
      _projection = &Renderer2d::projection_ortho;
      _cam_transform.reset();
    }

    void Renderer2d::set_orthographic()
    {
      _projection = &Renderer2d::projection_ortho;
    }

    void Renderer2d::set_perspective()
    {
      double out_ratio = (_2d_output_res.y() / _rows) / (_2d_output_res.x() / _cols);

      if (out_ratio < 1.)
        _window2d = math::VectorPair2(-1. / out_ratio, -1., 1. / out_ratio, 1.);
      else
        _window2d = math::VectorPair2(-1, -out_ratio, 1., out_ratio);

      _window2d_fit = _window2d;
      update_2d_window();
      set_page(_pageid);

      _projection = &Renderer2d::projection_perspective;
      _eye_dist = 1. / tan(math::degree2rad(_fov) / 2.);
    }

    math::Vector2 Renderer2d::projection_ortho(const math::Vector3 &v) const
    {
      math::Vector2 p(_cam_transform.transform(v).project_xy());

      return p;
    }

    math::Vector2 Renderer2d::projection_perspective(const math::Vector3 &v) const
    {
      math::Vector3 t(_cam_transform.transform(v));

      math::Vector2 p(t.x() * _eye_dist / -t.z(),
                        t.y() * _eye_dist / -t.z());

      return p;
    }

    math::Transform<3> Renderer2d::get_camera_transform() const
    {
      return _cam_transform;
    }

    void Renderer2d::set_camera_transform(const math::Transform<3> & t)
    {
      _cam_transform = t;
    }

    void Renderer2d::draw_point(const math::Vector3 &p, const Rgb &rgb, enum PointStyle s)
    {
      draw_point(project(p), rgb, s);
    }

    void Renderer2d::draw_segment(const math::VectorPair3 &l, const Rgb &rgb)
    {
      draw_segment(math::VectorPair2(project(l[0]), project(l[1])), rgb);
    }

    void Renderer2d::draw_text(const math::Vector3 &pos, const math::Vector3 &dir,
                               const std::string &str, TextAlignMask a, int size, const Rgb &rgb)
    {
      draw_text(project(pos), project(dir), str, a, size, rgb);
    }

  }

}

