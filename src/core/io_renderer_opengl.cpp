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

#include <goptical/core/math/Triangle>
#include <goptical/core/io/RendererOpengl>
#include <goptical/core/math/Transform>

extern "C" {
#include <GL/glu.h>
}

namespace _goptical {

  namespace io {

    RendererOpengl::RendererOpengl(double near, double far,
                                   double width, double height,
                                   const Rgb &bg)
      : _near(near),
        _far(far)
    {
      set_2d_size(width, height);

      _styles_color[StyleBackground] = bg;
      _styles_color[StyleForeground] = ~bg;
    }

    void RendererOpengl::set_2d_size(double width, double height)
    {
      RendererViewport::set_2d_size(width, height);
      glViewport(0, 0, width, height);
    }

    void RendererOpengl::set_camera_transform(const math::Transform<3> & t)
    {
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      apply_transform(t);
    }

    math::Transform<3> RendererOpengl::get_camera_transform() const
    {
      math::Transform<3> t;
      get_transform(GL_MODELVIEW_MATRIX, t);
      return t;
    }

    void RendererOpengl::set_orthographic()
    {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(_window2d[0].x(), _window2d[1].x(),
              _window2d[0].y(), _window2d[1].y(), _near, _far);
    }

    void RendererOpengl::set_perspective()
    {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(_fov, _2d_output_res.x() / _2d_output_res.y(), _near, _far);
    }

    void RendererOpengl::set_z_range(double near, double far)
    {
      _near = near;
      _far = far;
    }

    void RendererOpengl::draw_point(const math::Vector2 &p,
                                    const Rgb &rgb, enum PointStyle s)
    {
      glBegin(GL_POINTS);
      glColor(rgb);
      glVertex(p);
      glEnd();
    }

    void RendererOpengl::draw_segment(const math::VectorPair2 &l, const Rgb &rgb)
    {
      glBegin(GL_LINES);
      glColor(rgb);
      glVertex(l[0]);
      glVertex(l[1]);
      glEnd();
    }

    void RendererOpengl::draw_point(const math::Vector3 &p,
                                    const Rgb &rgb, enum PointStyle s)
    {
      glBegin(GL_POINTS);
      glColor(rgb);
      glVertex(p);
      glEnd();
    }

    void RendererOpengl::draw_segment(const math::VectorPair3 &l, const Rgb &rgb)
    {
      glBegin(GL_LINES);
      glColor(rgb);
      glVertex(l[0]);
      glVertex(l[1]);
      glEnd();
    }

    void RendererOpengl::draw_text(const math::Vector2 &c, const math::Vector2 &dir,
                                   const std::string &str, TextAlignMask a, int size, const Rgb &rgb)
    {
      // FIXME
    }

    void RendererOpengl::draw_text(const math::Vector3 &c, const math::Vector3 &dir,
                                   const std::string &str, TextAlignMask a, int size, const Rgb &rgb)
    {
      // FIXME
    }

    void RendererOpengl::draw_polygon(const math::Vector3 *array,
                                      unsigned int count, const Rgb &rgb,
                                      bool filled, bool closed)
    {
      if (count < 3)
        return;

      if (filled)
        {
          glBegin(GL_POLYGON);
          glColor(rgb);
          for (unsigned int i = 0; i < count; i++)
            glVertex(array[i]);
          glEnd();
        }
      else
        {
          glBegin(GL_LINE_STRIP);
          for (unsigned int i = 0; i < count; i++)
            glVertex(array[i]);
          if (closed)
            glVertex(array[0]);
          glEnd();
        }
    }

    void RendererOpengl::draw_triangle(const math::Triangle<3> &t, const Rgb &rgb)
    {
      glBegin(GL_TRIANGLES);
      glColor(rgb);
      glNormal(t.normal());

      for (unsigned int i = 0; i < 3; i++)
        glVertex(t[i]);

      glEnd();

#ifdef CONFIG_OPENGL_DRAWNORMAL
      glBegin(GL_LINES);
      for (unsigned int i = 0; i < 3; i++)
        {
          glVertex(t[i]);
          glVertex(t[i] + t.normal() * 20.0);
        }
      glEnd();
#endif
    }

    void RendererOpengl::draw_triangle(const math::Triangle<3> &t, const math::Triangle<3> &gradient, const Rgb &rgb)
    {
      glBegin(GL_TRIANGLES);
      glColor(rgb);

      for (unsigned int i = 0; i < 3; i++)
        {
          glNormal(gradient[i]);
          glVertex(t[i]);
        }

      glEnd();

#ifdef CONFIG_OPENGL_DRAWNORMAL
      glBegin(GL_LINES);
      for (unsigned int i = 0; i < 3; i++)
        {
          glVertex(t[i]);
          glVertex(t[i] + gradient[i] * 20.0);
        }
      glEnd();
#endif
    }

    void RendererOpengl::clear()
    {
      const Rgb & rgb = _styles_color[StyleBackground];
      glClearColor(rgb.r, rgb.g, rgb.b, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererOpengl::flush()
    {
      glFlush();
    }

    void RendererOpengl::apply_transform(const math::Transform<3> &t)
    {
      GLdouble m[16];

      m[ 0] = t.get_linear().value(0, 0);
      m[ 1] = t.get_linear().value(1, 0);
      m[ 2] = t.get_linear().value(2, 0);
      m[ 3] = 0.0;

      m[ 4] = t.get_linear().value(0, 1);
      m[ 5] = t.get_linear().value(1, 1);
      m[ 6] = t.get_linear().value(2, 1);
      m[ 7] = 0.0;

      m[ 8] = t.get_linear().value(0, 2);
      m[ 9] = t.get_linear().value(1, 2);
      m[10] = t.get_linear().value(2, 2);
      m[11] = 0.0;

      m[12] = t.get_translation().x();
      m[13] = t.get_translation().y();
      m[14] = t.get_translation().z();
      m[15] = 1.0;

      glMultMatrixd(m);
    }

    void RendererOpengl::get_transform(GLenum name, math::Transform<3> &t)
    {
      GLdouble m[16];

      glGetDoublev(name, m);

      t.get_linear().value(0, 0) = m[ 0];
      t.get_linear().value(1, 0) = m[ 1];
      t.get_linear().value(2, 0) = m[ 2];

      t.get_linear().value(0, 1) = m[ 4];
      t.get_linear().value(1, 1) = m[ 5];
      t.get_linear().value(2, 1) = m[ 6];

      t.get_linear().value(0, 2) = m[ 8];
      t.get_linear().value(1, 2) = m[ 9];
      t.get_linear().value(2, 2) = m[10];

      t.get_translation().x()    = m[12];
      t.get_translation().y()    = m[13];
      t.get_translation().z()    = m[14];
    }

  }
}

