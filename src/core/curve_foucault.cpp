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

      Foucault data reduction algorithm based on "ATM Mirror mathematics"
      http://home.earthlink.net/~burrjaw/atm/atm_math.lwp/atm_math.htm

      Couder mask theory and formulas developed by Nils Olof Carlin
      http://www.atmsite.org/contrib/Carlin/couder/

*/

#include <goptical/core/Error>
#include <goptical/core/curve/Foucault>
#include <goptical/core/math/Vector>

namespace _goptical {

  namespace curve {

    void Foucault::init()
    {
      _moving_source = true;
      _offset = 0;
      _radius = 0;
      _ode_step = 1;
      _updated = false;

      _reading.set_interpolation(data::Cubic);
      _sagitta.set_interpolation(data::CubicDeriv);

      gsl_st = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45, 1);
      gsl_sys.function = gsl_func;
      gsl_sys.jacobian = NULL;
      gsl_sys.dimension = 1;
      gsl_sys.params = this;
    }

    Foucault::Foucault(double roc)
      : curveRoc(roc),
        _reading(),
        _sagitta()
    {
      init();
    }

    Foucault::~Foucault()
    {
      gsl_odeiv_step_free(gsl_st);
    }

    void Foucault::fit(const Rotational &c)
    {
      _offset = 0;
      _moving_source = true;

      for (unsigned int j = 0; j < _reading.get_count(); j++)
        {
          double zn = _reading.get_x_value(j);

          _reading.get_y_value(j) = c.sagitta(zn) + zn / c.derivative(zn);
        }
    }

    void Foucault::add_reading(double zone_radius, double knife_offset)
    {
      _updated = false;

      if (_radius < zone_radius * 1.1)
        _radius = zone_radius * 1.1;

      _reading.add_data(zone_radius, _roc + knife_offset);
    }

    unsigned int Foucault::add_uniform_zones(double hole_radius, unsigned int count = 0)
    {
      assert(hole_radius < _radius);
      assert(count > 0);

      double step = (_radius - hole_radius) / (double)count;

      _reading.clear();

      for (unsigned int i = 0; i < count; i++)
        {
          _reading.add_data(hole_radius + step / 2.0, _roc);
          hole_radius += step;
        }

      return count;
    }

    unsigned int Foucault::add_couder_zones(double hole_radius, unsigned int count, std::vector<double> *edge)
    {
      assert(hole_radius < _radius);

      // see http://www.atmsite.org/contrib/Carlin/couder/

      if (count == 0)
        {
          static const double k = 37e-6;

          count = (unsigned int)ceil(((1.0 - hole_radius / _radius) * math::square(_radius * 2.0))
                                        / (16.0 * sqrt(math::square(_roc) * _roc * k)));
        }

      double out;
      double in = hole_radius;

      _reading.clear();

      for (unsigned int i = 1; i < count + 1; i++)
        {
          double zr = (double)i / (double)count;
          double hr = hole_radius / _radius;    // hole ratio
          double sr = 1.0 - hr; // surface ratio

          out = _radius * sqrt(sr * zr + hr);

          if (edge)
            edge->push_back(in);

          _reading.add_data(sqrt(math::square((out + in) / 2.0) + (math::square((out - in) / 2.0) / 2.0)), _roc);

          in = out;
        }

      if (edge)
        edge->push_back(in);

      return count;
    }

    void Foucault::clear()
    {
      _updated = false;

      _reading.clear();
      _sagitta.clear();
    }

    double Foucault::sagitta(double r) const
    {
      if (!_updated)
        const_cast<Foucault *>(this)->update();

      return _sagitta.interpolate(r);
    }

    double Foucault::derivative(double r) const
    {
      if (!_updated)
        const_cast<Foucault *>(this)->update();

      return _sagitta.interpolate(r, 1);
    }

    void Foucault::update()
    {
      _sagitta.clear();

      double t = 0;
      double y = 0.0, yerr;
      double dydt_in = 0.0, dydt_out;

      // differential equation integration
      while (1)
        {
          _sagitta.add_data(t, y, dydt_in);

          if (t >= _radius)
            break;

          if (gsl_odeiv_step_apply(gsl_st, t, _ode_step, &y, &yerr, &dydt_in, &dydt_out, &gsl_sys))
            throw Error("Foucault curve integration error");

          dydt_in = dydt_out;
          t += _ode_step;
        }

      _updated = true;
    }

    int Foucault::gsl_func(double y, const double x_[],
                           double d[], void *params)
    {
      Foucault *c = static_cast<Foucault *>(params);
      double f = c->_reading.interpolate(y);
      double x = x_[0];
      double s = c->_moving_source ? f + c->_offset : c->_offset;

#if 0
      // moving source only
      d[0] = y / (f - x);
#endif
      double ds = sqrt(math::square(s - x) + math::square(y));
      double df = sqrt(math::square(f - x) + math::square(y));

      d[0] = ((df + ds) * y) / (df * (s - x) + ds * (f - x));

      return 0;
    }

  }

}

