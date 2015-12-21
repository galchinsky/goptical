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


#include <strings.h>

#include <goptical/core/io/RendererAxes>
#include <goptical/core/math/Vector>

namespace _goptical {

  namespace io {

    RendererAxes::Axis::Axis()
      : _axis(true),
        _tics(true),
        _values(true),
        _step_mode(step_base),
        _count(5),
        _step_base(10.0),
        _si_prefix(false),
        _pow10_scale(true),
        _pow10(0),
        _unit(),
        _label(),
        _range(0, 0)
    {
    }

    RendererAxes::RendererAxes()
      : _axes(),
        _grid(false),
        _frame(true),
        _pos(math::Vector3(0, 0, 0)),
        _origin(math::Vector3(0, 0, 0))
    {
    }

    void RendererAxes::set_tics_step(double step, AxisMask a_)
    {
      for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
        {
          _axes[i]._step_base = step;
          _axes[i]._step_mode = step_interval;
        }
    }

    void RendererAxes::set_tics_count(unsigned int count, AxisMask a_)
    {
      for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
        {
          _axes[i]._count = count;
          _axes[i]._step_mode = step_count;
        }
    }

    void RendererAxes::set_tics_base(unsigned int min_count, double base, AxisMask a_)
    {
      for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
        {
          _axes[i]._count = min_count;
          _axes[i]._step_base = base;
          _axes[i]._step_mode = step_base;
        }
    }

    void RendererAxes::set_origin(const math::Vector3 &origin)
    {
      _origin = origin;
    }

    void RendererAxes::set_position(const math::Vector3 &position)
    {
      _pos = position;
    }

    void RendererAxes::set_show_grid(bool show)
    {
      _grid = show;
    }

    void RendererAxes::set_show_frame(bool show)
    {
      _frame = show;
    }

    void RendererAxes::set_show_axes(bool show, AxisMask a_)
    {
      for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
        _axes[i]._axis = show;
    }

    void RendererAxes::set_show_tics(bool show, AxisMask a_)
    {
      for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
        {
          _axes[i]._tics = show;
          _axes[i]._axis |= show;
        }
    }

    void RendererAxes::set_show_values(bool show, AxisMask a_)
    {
      for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
        {
          _axes[i]._values = show;
          _axes[i]._tics |= show;
          _axes[i]._axis |= show;
        }
    }

    void RendererAxes::set_label(const std::string &label,
                                 AxisMask a_)
    {
      for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
        _axes[i]._label = label;
    }

    void RendererAxes::set_unit(const std::string &unit, bool pow10_scale,
                                bool si_prefix, int pow10,
                                AxisMask a_)
    {
      for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
        {
          _axes[i]._si_prefix = si_prefix;
          _axes[i]._unit = unit;
          _axes[i]._pow10_scale = pow10_scale;
          _axes[i]._pow10 = pow10;
        }
    }


    void RendererAxes::set_range(const math::range_t &r,
                                 AxisMask a_)
    {
      for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
        _axes[i]._range = r;
    }

    double RendererAxes::get_tics_step(int index, const math::range_t &r) const
    {
      assert(index < 3);

      const Axis *a = _axes + index;
      double d = r.second - r.first;

      switch (a->_step_mode)
        {
        case step_interval:
          return d > 0 ? a->_step_base : -a->_step_base;

        case step_count:
          return d / (double)a->_count;

        case step_base: {
          if (d == 0.0)
            return 1;

          double da = fabs(d);
          double p = floor(log(da) / log(a->_step_base));
          double n = pow(a->_step_base, p);
          unsigned int f = 1;

          while ((unsigned int)(da / n * f) < a->_count)
            {
              if ((unsigned int)(da / n * f * 2) >= a->_count)
                {
                  f *= 2;
                  break;
                }
              else if ((unsigned int)(da / n * f * 5) >= a->_count)
                {
                  f *= 5;
                  break;
                }
              else
                {
                  f *= 10;
                }
            }

          n /= f;

          return d > 0 ? n : -n;
        }

          default:
            abort();
        }
    }

} 
}

