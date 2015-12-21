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

#include <cassert>
#include <limits>

#include <goptical/core/shape/Polygon>

#include <goptical/core/trace/Distribution>
#include <goptical/core/math/Triangle>
#include <goptical/core/math/VectorPair>

namespace _goptical {

  namespace shape {

    Polygon::Polygon()
      : _updated(false),
        _vertices(),
        _bbox(math::vector2_pair_00),
        _max_radius(0),
        _min_radius(1e100)
    {
    }

    inline void Polygon::update()
    {
      size_t s = _vertices.size();
      if (s < 3)
        throw Error("Polygon shape has less than 3 vertices");

      _max_radius = 0;
      _min_radius = std::numeric_limits<double>::max();
      _bbox = math::VectorPair2(std::numeric_limits<double>::max(),
                                  -std::numeric_limits<double>::max());

      const math::Vector2 *prev = &_vertices[s - 1];
      const math::Vector2 *cur;

      for (unsigned int i = 0; i < s; i++)
        {
          cur = &_vertices[i];

          // update max radius
          double max = cur->len();
          if (max > _max_radius)
            _max_radius = max;

          // update min radius
          double min = math::VectorPair2(*cur, *prev).seg_pt_clst_pt(math::vector2_0).len();
          if (min < _min_radius)
            _min_radius = min;

          // udate bounding box
          for (unsigned int i = 0; i < 2; i++)
            {
              if ((*cur)[i] < _bbox[0][i])
                _bbox[0][i] = (*cur)[i];
              else if ((*cur)[i] > _bbox[1][i])
                _bbox[1][i] = (*cur)[i];
            }

          prev = cur;
        }
    }

    void Polygon::insert_vertex(const math::Vector2 &v, unsigned int id)
    {
      _updated = false;
      assert(id <= _vertices.size());
      _vertices.insert(_vertices.begin() + id, v);
    }

    unsigned int Polygon::add_vertex(const math::Vector2 &v)
    {
      _updated = false;
      unsigned int pos = _vertices.size();
      insert_vertex(v, pos);
      return pos;
    }

    void Polygon::delete_vertex(unsigned int id)
    {
      _updated = false;
      assert(id < _vertices.size());
      _vertices.erase(_vertices.begin() + id);
    }

    bool Polygon::inside(const math::Vector2 &p) const
    {
      unsigned int s = _vertices.size();

      if (s < 3)
        return false;

      unsigned int count = 0;
      const math::Vector2 *w = &_vertices[s - 1];

      // FIXME optimize
      for (unsigned int i = 0; i < s; i++)
        {
          const math::Vector2 *v = &_vertices[i];

          // Algorithm from http://local.wasp.uwa.edu.au/~pbourke/geometry/insidepoly/
          if ((((v->y() <= p.y()) && (p.y() < w->y())) || ((w->y() <= p.y()) && (p.y() < v->y()))) &&
              (p.x() < (w->x() - v->x()) * (p.y() - v->y()) / (w->y() - v->y()) + v->x()))
            count++;
          w = v;
        }

      return (count & 1) != 0;
    }

    void Polygon::get_contour(unsigned int contour, const math::Vector2::put_delegate_t  &f, double resolution) const
    {
      for (auto&v : _vertices)
        f(v);
    }

    void Polygon::get_triangles(const math::Triangle<2>::put_delegate_t &f, double resolution) const
    {
      const math::Vector2 &z = _vertices[0];

      for (unsigned int i = 1; i < _vertices.size() - 1; i++)
        {
          const math::Vector2 &a = _vertices[i];
          const math::Vector2 &b = _vertices[i + 1];

          f(math::Triangle<2>(z, a, b));
        }
    }

    double Polygon::max_radius() const
    {
      if (!_updated)
        const_cast<Polygon*>(this)->update();
      return _max_radius;
    }

    double Polygon::min_radius() const
    {
      if (!_updated)
        const_cast<Polygon*>(this)->update();
      return _min_radius;
    }

    math::VectorPair2 Polygon::get_bounding_box() const
    {
      if (!_updated)
        const_cast<Polygon*>(this)->update();
      return _bbox;
    }

    double Polygon::get_outter_radius(const math::Vector2 &dir) const
    {
      if (!_updated)
        const_cast<Polygon*>(this)->update();

      double r = 0;
      unsigned int s = _vertices.size();

      const math::VectorPair2 d(math::vector2_0, dir * _max_radius * 2.);
      const math::Vector2 *w = &_vertices[s - 1];

      // FIXME optimize
      for (unsigned int i = 0; i < s; i++)
        {
          const math::Vector2 *v = &_vertices[i];

          try {
            double a = math::VectorPair2(*w, *v).seg_intersect_seg<false, false>(d).len();
            r = std::max(r, a);
          } catch (...) {
          }

          w = v;
        }

      return r;
    }

  }

}
