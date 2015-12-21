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

#include <goptical/core/data/Grid>
#include <goptical/core/Error>

namespace _goptical {

  namespace data {

    Grid::Grid(unsigned int n1, unsigned int n2,
                           const math::Vector2 & origin,
                           const math::Vector2 & step)
      : Set(),
        _y_data(),
        _d_data(),
        _poly(),
        _update(&Grid::update_linear),
        _lookup(&Grid::update_linear),
        _resize(&Grid::resize_y),
        _origin(origin),
        _step(step)
    {
      _origin = origin;
      _step = step;
      resize(n1, n2);
    }

    Grid::~Grid()
    {
    }

    void Grid::set_all_y(double y)
    {
      for (auto& i : _y_data) {
        i = y;
      }
    }

    void Grid::set_all_d(const math::Vector2 & deriv)
    {
      for (auto& i : _d_data) {
        i = deriv;
      }
    }

    double Grid::get_x_value(unsigned int x, unsigned int dimension) const
    {
      assert(dimension < 2);
      return _origin[dimension] + _step[dimension] * x;
    }

    double Grid::get_y_value(const unsigned int n[]) const
    {
      return get_y_value(n[0], n[1]);
    }

    double Grid::interpolate(const double x[]) const
    {
      return interpolate(math::Vector2(x[0], x[1]));
    }

    double Grid::interpolate(const double x[], unsigned int deriv, unsigned int dimension) const
    {
      switch (deriv)
        {
        case 0:
          assert(dimension < 1);
          return interpolate(math::Vector2(x[0], x[1]));
        case 1:
          assert(dimension < 2);
          return interpolate_deriv(math::Vector2(x[0], x[1]))[dimension];
        default:
          assert(deriv < 2);
          return 0;
        }
    }

    math::range_t Grid::get_x_range(unsigned int d) const
    {
      if (_size[0] < 1 || _size[1] < 1)
        throw Error("data set contains no data");

      return math::range_t(_origin[d], _origin[d] + _step[d] * (_size[d] - 1));
    }

    void Grid::set_interpolation(Interpolation i)
    {
      switch (i)
        {
        case Nearest:     
          _update = &Grid::update_nearest;
          _resize = &Grid::resize_y;
          _d_data.clear();
          _poly.clear();
          break;

        case Linear:
          _update = &Grid::update_linear;
          _resize = &Grid::resize_y;
          _d_data.clear();
          _poly.clear();
          break;

        case Bicubic:
          _update = &Grid::update_bicubic;
          _resize = &Grid::resize_y;
          _d_data.clear();
          break;

        case BicubicDiff:
          _update = &Grid::update_bicubic_diff;
          _resize = &Grid::resize_y;
          _d_data.clear();
          break;

        case BicubicDeriv:
          _update = &Grid::update_bicubic_deriv;
          _resize = &Grid::resize_yd;
          _d_data.resize(_size[0] * _size[1], math::Vector2(0, 0));
          break;

        default:
          throw Error("invalid interpolation selected");
        }

      _interpolation = i;
      _lookup = _update;
    }

    // **********************************************************************

    void Grid::update_nearest(unsigned int x[2], const math::Vector2 & v) const
    {
      Grid * this_ = const_cast<Grid *>(this);

      if (_size[0] < 1 || _size[1] < 1)
        throw Error("data set doesn't contains enough data");

      this_->_lookup = &Grid::lookup_nearest;
      this_->_interpolate_y = &Grid::interpolate_nearest_y;
      this_->_interpolate_d = &Grid::interpolate_nearest_d;

      return lookup_nearest(x, v);
    }

    double Grid::interpolate_nearest_y(const unsigned int x[2], const math::Vector2 & v) const
    {
      return _y_data[x[0] + x[1] * _size[0]];
    }

    void Grid::interpolate_nearest_d(const unsigned int x[2], math::Vector2 & d, const math::Vector2 & v) const
    {
      d.set(0);
    }

    // **********************************************************************

    void Grid::update_linear(unsigned int x[2], const math::Vector2 & v) const
    {
      Grid * this_ = const_cast<Grid *>(this);

      if (_size[0] < 2 || _size[1] < 2)
        throw Error("data set doesn't contains enough data");

      this_->_lookup = &Grid::lookup_interval;
      this_->_interpolate_y = &Grid::interpolate_linear_y;
      this_->_interpolate_d = &Grid::interpolate_linear_d;

      return lookup_interval(x, v);
    }

    double Grid::interpolate_linear_y(const unsigned int x[2], const math::Vector2 & v) const
    {
      unsigned int s = _size[0];
      unsigned int idx = x[0] + s * x[1];

      double mu1 = v.x() / _step.x() - (double)x[0];

      double a = _y_data[idx] * (1.0 - mu1) + _y_data[idx + 1] * mu1;
      double b = _y_data[idx + s] * (1.0 - mu1) + _y_data[idx + s + 1] * mu1;

      double mu2 = v.y() / _step.y() - (double)x[1];

      return a * (1.0 - mu2) + b * mu2;
    }

    void Grid::interpolate_linear_d(const unsigned int x[2], math::Vector2 & d, const math::Vector2 & v) const
    {
      unsigned int s = _size[0];
      unsigned int idx = x[0] + s * x[1];

      double a1 = (_y_data[idx] - _y_data[idx + 1]) / _step.x();
      double b1 = (_y_data[idx + s] - _y_data[idx + s + 1]) / _step.x();
      double mu2 = v.y() / _step.y() - (double)x[1];

      d.x() = a1 * (1.0 - mu2) + b1 * mu2;

      double a2 = (_y_data[idx] - _y_data[idx + s]) / _step.y();
      double b2 = (_y_data[idx + 1] - _y_data[idx + s + 1]) / _step.y();
      double mu1 = v.x() / _step.x() - (double)x[0];

      d.y() = a2 * (1.0 - mu1) + b2 * mu1;
    }

    // **********************************************************************

    void Grid::set_poly(poly_t &p, const double t[16])
    {
      /*
           0    1
        0 d00  d01
        1 d10  d11
      */

      p.p[ 0] =    1.0 * t[0];
      p.p[ 1] =    1.0 * t[8];
      p.p[ 2] =   -3.0 * t[0] + 3.0 * t[2] + -2.0 * t[8] + -1.0 * t[10];
      p.p[ 3] =    2.0 * t[0] + -2.0 * t[2] + 1.0 * t[8] + 1.0 * t[10];

      p.p[ 4] =    1.0 * t[4];
      p.p[ 5] =    1.0 * t[12];
      p.p[ 6] =   -3.0 * t[4] + 3.0 * t[6] + -2.0 * t[12] + -1.0 * t[14];
      p.p[ 7] =    2.0 * t[4] + -2.0 * t[6] + 1.0 * t[12] + 1.0 * t[14];

      p.p[ 8] =   -3.0 * t[0] + 3.0 * t[1] + -2.0 * t[4] + -1.0 * t[5];
      p.p[ 9] =   -3.0 * t[8] + 3.0 * t[9] + -2.0 * t[12] + -1.0 * t[13];
      p.p[10] =  ( 9.0 * t[0] + -9.0 * t[1] + 9.0 * t[3] + -9.0 * t[2] +
                   6.0 * t[4] + 3.0 * t[5] + -3.0 * t[7] + -6.0 * t[6] +
                   6.0 * t[8] + -6.0 * t[9] + -3.0 * t[11] + 3.0 * t[10] +
                   4.0 * t[12] + 2.0 * t[13] + 1.0 * t[15] + 2.0 * t[14]);
      p.p[11] =  (-6.0 * t[0] + 6.0 * t[1] + -6.0 * t[3] + 6.0 * t[2] +
                  -4.0 * t[4] + -2.0 * t[5] + 2.0 * t[7] + 4.0 * t[6] +
                  -3.0 * t[8] + 3.0 * t[9] + 3.0 * t[11] + -3.0 * t[10] +
                  -2.0 * t[12] + -1.0 * t[13] + -1.0 * t[15] + -2.0 * t[14]);

      p.p[12] =    2.0 * t[0] + -2.0 * t[1] + 1.0 * t[4] + 1.0 * t[5];
      p.p[13] =    2.0 * t[8] + -2.0 * t[9] + 1.0 * t[12] + 1.0 * t[13];
      p.p[14] =  (-6.0 * t[0] + 6.0 * t[1] + -6.0 * t[3] + 6.0 * t[2] +
                  -3.0 * t[4] + -3.0 * t[5] + 3.0 * t[7] + 3.0 * t[6] +
                  -4.0 * t[8] + 4.0 * t[9] + 2.0 * t[11] + -2.0 * t[10] +
                  -2.0 * t[12] + -2.0 * t[13] + -1.0 * t[15] + -1.0 * t[14]);
      p.p[15] =  ( 4.0 * t[0] + -4.0 * t[1] + 4.0 * t[3] + -4.0 * t[2] +
                   2.0 * t[4] + 2.0 * t[5] + -2.0 * t[7] + -2.0 * t[6] +
                   2.0 * t[8] + -2.0 * t[9] + -2.0 * t[11] + 2.0 * t[10] +
                   1.0 * t[12] + 1.0 * t[13] + 1.0 * t[15] + 1.0 * t[14]);
    }

    void Grid::get_cross_deriv_diff(double cd[]) const
    {
      unsigned int x0, x1;

      const unsigned int w = _size[0];

      for (x0 = 1; x0 < _size[0] - 1; x0++)
        for (x1 = 1; x1 < _size[1] - 1; x1++)
          {
            const unsigned int idx = x0 + w * x1;

            cd[idx] = (_y_data[idx + w + 1] + _y_data[idx - w - 1] -
                       _y_data[idx + w - 1] - _y_data[idx - w + 1]) / 4.0;
          }

      const unsigned int w1 = _size[0] * (_size[1] - 1);

      for (x0 = 0; x0 < _size[0]; x0++)
        {
          cd[x0] = 0.0;
          cd[w1 + x0] = 0.0;
        }

      for (x1 = 1; x1 < _size[1] - 1; x1++)
        {
          cd[x1 * _size[0]] = 0.0;
          cd[x1 * _size[0] + _size[0] - 1] = 0.0;
        }
    }

    void Grid::get_deriv_diff(math::Vector2 d[]) const
    {
      unsigned int x0, x1;

      const unsigned int w = _size[0];

      for (x0 = 0; x0 < _size[0]; x0++)
        for (x1 = 0; x1 < _size[1]; x1++)
          {
            const unsigned int idx = x0 + w * x1;

            if (x0 > 0 && x0 < _size[0] - 1)
              d[idx].x() = (_y_data[idx + 1] - _y_data[idx - 1]) / 2.0;
            else
              d[idx].x() = 0.0;

            if (x1 > 0 && x1 < _size[1] - 1)
              d[idx].y() = (_y_data[idx + w] - _y_data[idx - w]) / 2.0;
            else
              d[idx].y() = 0.0;
          }
    }

    template <int v>
    void Grid::get_deriv_smooth(math::Vector2 d[], unsigned int w, unsigned int o) const
    {
      unsigned int n = _size[v];

      double eq[n][3];
      double dd[n];

      // tridiag system equations
      dd[0  ] = dd[n-1] = 0.0;
      eq[0  ][1] = eq[n-1][1] = 1.0;
      eq[1  ][0] = eq[n-2][2] = -1.0;

      int i, j = 0;

      for (i = 1; i < (int)n - 1; i++)
        {
          j = o + i * w;

          eq[i-1][2] = _step[v] / 6.0;
          eq[i  ][1] = _step[v] * 2.0 / 3.0;
          eq[i+1][0] = _step[v] / 6.0;
          dd[i] = (_y_data[j+w] - _y_data[j]) / _step[v]
                - (_y_data[j] - _y_data[j-w]) / _step[v];
        }

      // solve tridiag system
      // forward substitution
      for (i = 1; i < (int)n; i++)
        {
          double f = eq[i-1][2] / eq[i-1][1];
          eq[i][1] -= f * eq[i][0];
          dd[i]    -= f * dd[i-1];
        }

      // backward substitution
      double k = 0;
      for (i = n - 1; i >= 0; i--)
        {
          double ddi = (dd[i] - k) / eq[i][1];
          dd[i] = ddi;
          k = eq[i][0] * ddi;
        }

      // here with have the continuous second derivative in dd[],
      // compute smooth 1st derivative
      double dt = _step[v];

      for (i = 0; i < (int)n - 1; i++)
        {
          j = o + i * w;
          d[j][v] = (_y_data[j+w] - _y_data[j]) - (dd[i+1] / 6.0 + dd[i] / 3.0) * (dt * dt);
        }

      i--;

      d[j + w][v] =
        + (_y_data[j+w] - _y_data[j])
        + (dt * dt) * ((dd[i] + dt * (0.5 * (dd[i+1] - dd[i]) / dt)) - (dd[i+1] / 6.0 + dd[i] / 3.0));
    }

    void Grid::update_bicubic(unsigned int x[2], const math::Vector2 & v) const
    {
      Grid * this_ = const_cast<Grid *>(this);

      if (_size[0] < 2 || _size[1] < 2)
        throw Error("data set doesn't contains enough data");

      const unsigned int s0 = _size[0] - 1;
      this_->_poly.resize(s0 * (_size[1] - 1));

      double cd[_size[0] * _size[1]];
      get_cross_deriv_diff(cd);

      const unsigned int w = _size[0];
      unsigned int x0, x1;

      DPP_VLARRAY(math::Vector2, _size[0] * _size[1], d);

      for (x1 = 0; x1 < _size[1]; x1++)
        get_deriv_smooth<0>(&d[0], 1, x1 * w);

      for (x0 = 0; x0 < _size[0]; x0++)
        get_deriv_smooth<1>(&d[0], w, x0);

      for (unsigned int x0 = 0; x0 < _size[0] - 1; x0++)
        for (unsigned int x1 = 0; x1 < _size[1] - 1; x1++)
          {
            const unsigned int idx = x0 + _size[0] * x1;
            double t[16];

            t[0] = _y_data[idx];
            t[1] = _y_data[idx + 1];
            t[2] = _y_data[idx + w];
            t[3] = _y_data[idx + w + 1];

            t[4+0] = d[idx].x();
            t[4+1] = d[idx + 1].x();
            t[4+2] = d[idx + w].x();
            t[4+3] = d[idx + w + 1].x();
            t[8+0] = d[idx].y();
            t[8+1] = d[idx + 1].y();
            t[8+2] = d[idx + w].y();
            t[8+3] = d[idx + w + 1].y();

            t[12+0] = cd[idx];
            t[12+1] = cd[idx + 1];
            t[12+2] = cd[idx + w];
            t[12+3] = cd[idx + w + 1];

            set_poly(this_->_poly[x0 + s0 * x1], t);
          }

      this_->_lookup = &Grid::lookup_interval;
      this_->_interpolate_y = &Grid::interpolate_bicubic_y;
      this_->_interpolate_d = &Grid::interpolate_bicubic_d;

      return lookup_interval(x, v);
    }

    void Grid::update_bicubic_diff(unsigned int x[2], const math::Vector2 & v) const
    {
      Grid * this_ = const_cast<Grid *>(this);

      if (_size[0] < 2 || _size[1] < 2)
        throw Error("data set doesn't contains enough data");

      const unsigned int s0 = _size[0] - 1;
      this_->_poly.resize(s0 * (_size[1] - 1));

      double cd[_size[0] * _size[1]];
      get_cross_deriv_diff(cd);

      DPP_VLARRAY(math::Vector2, _size[0] * _size[1], d);
      get_deriv_diff(&d[0]);

      const unsigned int w = _size[0];

      for (unsigned int x0 = 0; x0 < _size[0] - 1; x0++)
        for (unsigned int x1 = 0; x1 < _size[1] - 1; x1++)
          {
            const unsigned int idx = x0 + _size[0] * x1;
            double t[16];

            t[0] = _y_data[idx];
            t[1] = _y_data[idx + 1];
            t[2] = _y_data[idx + w];
            t[3] = _y_data[idx + w + 1];

            t[4+0] = d[idx].x();
            t[4+1] = d[idx + 1].x();
            t[4+2] = d[idx + w].x();
            t[4+3] = d[idx + w + 1].x();
            t[8+0] = d[idx].y();
            t[8+1] = d[idx + 1].y();
            t[8+2] = d[idx + w].y();
            t[8+3] = d[idx + w + 1].y();

            t[12+0] = cd[idx];
            t[12+1] = cd[idx + 1];
            t[12+2] = cd[idx + w];
            t[12+3] = cd[idx + w + 1];

            set_poly(this_->_poly[x0 + s0 * x1], t);
          }

      this_->_lookup = &Grid::lookup_interval;
      this_->_interpolate_y = &Grid::interpolate_bicubic_y;
      this_->_interpolate_d = &Grid::interpolate_bicubic_d;

      return lookup_interval(x, v);
    }

    void Grid::update_bicubic_deriv(unsigned int x[2], const math::Vector2 & v) const
    {
      Grid * this_ = const_cast<Grid *>(this);

      if (_size[0] < 2 || _size[1] < 2)
        throw Error("data set doesn't contains enough data");

      const unsigned int s0 = _size[0] - 1;
      this_->_poly.resize(s0 * (_size[1] - 1));

      double cd[_size[0] * _size[1]];
      get_cross_deriv_diff(cd);

      const unsigned int w = _size[0];

      for (unsigned int x0 = 0; x0 < _size[0] - 1; x0++)
        for (unsigned int x1 = 0; x1 < _size[1] - 1; x1++)
          {
            const unsigned int idx = x0 + _size[0] * x1;
            double t[16];

            t[0] = _y_data[idx];
            t[1] = _y_data[idx + 1];
            t[2] = _y_data[idx + w];
            t[3] = _y_data[idx + w + 1];

            t[4+0] = _d_data[idx].x() * _step.x();
            t[4+4] = _d_data[idx].y() * _step.y();
            t[4+1] = _d_data[idx + 1].x() * _step.x();
            t[4+5] = _d_data[idx + 1].y() * _step.y();
            t[4+2] = _d_data[idx + w].x() * _step.x();
            t[4+6] = _d_data[idx + w].y() * _step.y();
            t[4+3] = _d_data[idx + w + 1].x() * _step.x();
            t[4+7] = _d_data[idx + w + 1].y() * _step.y();

            t[12+0] = cd[idx];
            t[12+1] = cd[idx + 1];
            t[12+2] = cd[idx + w];
            t[12+3] = cd[idx + w + 1];

            set_poly(this_->_poly[x0 + s0 * x1], t);
          }

      this_->_lookup = &Grid::lookup_interval;
      this_->_interpolate_y = &Grid::interpolate_bicubic_y;
      this_->_interpolate_d = &Grid::interpolate_bicubic_d;

      return lookup_interval(x, v);
    }

    double Grid::interpolate_bicubic_y(const unsigned int x[2], const math::Vector2 & v) const
    {
      const poly_t &p = _poly[x[0] + x[1] * (_size[0] - 1)];
      math::Vector2 t((v - _origin) / _step - math::Vector2((double)x[0], (double)x[1]));

      double res;

      res = ((p.p[15] * t.y() + p.p[14]) * t.y() + p.p[13]) * t.y() + p.p[12];
      res = ((p.p[11] * t.y() + p.p[10]) * t.y() + p.p[9]) * t.y() + p.p[8] + t.x() * res;
      res = ((p.p[7] * t.y() + p.p[6]) * t.y() + p.p[5]) * t.y() + p.p[4] + t.x() * res;
      res = ((p.p[3] * t.y() + p.p[2]) * t.y() + p.p[1]) * t.y() + p.p[0] + t.x() * res;

      return res;
    }

    void Grid::interpolate_bicubic_d(const unsigned int x[2], math::Vector2 & d, const math::Vector2 & v) const
    {
      const poly_t &p = _poly[x[0] + x[1] * (_size[0] - 1)];
      math::Vector2 t((v - _origin) / _step - math::Vector2((double)x[0], (double)x[1]));

      d.set(0);

      d.x() = (3.0 * p.p[12+3] * t.x() + 2.0 * p.p[8+3]) * t.x() + p.p[4+3];
      d.x() = (3.0 * p.p[12+2] * t.x() + 2.0 * p.p[8+2]) * t.x() + p.p[4+2] + t.y() * d.x();
      d.x() = (3.0 * p.p[12+1] * t.x() + 2.0 * p.p[8+1]) * t.x() + p.p[4+1] + t.y() * d.x();
      d.x() = (3.0 * p.p[12+0] * t.x() + 2.0 * p.p[8+0]) * t.x() + p.p[4+0] + t.y() * d.x();
      d.x() /= _step.x();

      d.y() = (3.0 * p.p[12+3] * t.y() + 2.0 * p.p[12+2]) * t.y() + p.p[12+1];
      d.y() = (3.0 * p.p[8+3] * t.y() + 2.0 * p.p[8+2]) * t.y() + p.p[8+1] + t.x() * d.y();
      d.y() = (3.0 * p.p[4+3] * t.y() + 2.0 * p.p[4+2]) * t.y() + p.p[4+1] + t.x() * d.y();
      d.y() = (3.0 * p.p[0+3] * t.y() + 2.0 * p.p[0+2]) * t.y() + p.p[0+1] + t.x() * d.y();
      d.y() /= _step.y();
    }

    // **********************************************************************

    void Grid::lookup_nearest(unsigned int x[2], const math::Vector2 & v) const
    {
      int x0 = (int)(round((v.x() - _origin.x()) / _step.x()));
      int x1 = (int)(round((v.y() - _origin.y()) / _step.y()));

      if (x0 < 0)
        x0 = 0;
      else if (x0 >= (int)_size[0] - 1)
        x0 = _size[0] - 2;

      x[0] = x0;

      if (x1 < 0)
        x1 = 0;
      else if (x1 >= (int)_size[1] - 1)
        x1 = _size[1] - 2;

      x[1] = x1;      
    }

    void Grid::lookup_interval(unsigned int x[2], const math::Vector2 & v) const
    {
      int x0 = (int)(floor((v.x() - _origin.x()) / _step.x()));
      int x1 = (int)(floor((v.y() - _origin.y()) / _step.y()));

      if (x0 < 0)
        x0 = 0;
      else if (x0 >= (int)_size[0] - 1)
        x0 = _size[0] - 2;

      x[0] = x0;

      if (x1 < 0)
        x1 = 0;
      else if (x1 >= (int)_size[1] - 1)
        x1 = _size[1] - 2;

      x[1] = x1;      
    }

    // **********************************************************************

    void Grid::resize_y(unsigned int x1, unsigned int x2)
    {
      invalidate();
      assert(x1 >= 2 && x2 >=2);
      _y_data.resize(x1 * x2, 0.0);
    }

    void Grid::resize_yd(unsigned int x1, unsigned int x2)
    {
      invalidate();
      assert(x1 >= 2 && x2 >=2);
      _y_data.resize(x1 * x2, 0.0);
      _d_data.resize(x1 * x2, math::Vector2(0, 0));
    }

  }

}

