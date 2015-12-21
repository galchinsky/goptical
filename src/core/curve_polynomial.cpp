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

#include <goptical/core/curve/Polynomial>

#include <cstdarg>
#include <cassert>

namespace _goptical {

  namespace curve {

    Polynomial::Polynomial()
      : _first_term(0),
        _last_term(0),
        _coeff()
    {
      _coeff.push_back(0);
    }

    Polynomial::Polynomial(unsigned int first_term, unsigned int last_term, ...)
      : _coeff()
    {
      va_list ap;

      assert(last_term >= last_term);
      _first_term = first_term;
      _last_term = last_term;
      _coeff.resize(_last_term + 1, 0.0);

      va_start(ap, last_term);

      for (unsigned int i = first_term; i <= last_term; i++)
        _coeff[i] = va_arg(ap, double);

      va_end(ap);
    }

    void Polynomial::set(unsigned int first_term, unsigned int last_term, ...)
    {
      va_list ap;

      assert(last_term >= last_term);
      _first_term = first_term;
      _last_term = last_term;
      _coeff.clear();
      _coeff.resize(_last_term + 1, 0.0);

      va_start(ap, last_term);

      for (unsigned int i = first_term; i <= last_term; i++)
        _coeff[i] = va_arg(ap, double);

      va_end(ap);
    }

    void Polynomial::set_even(unsigned int first_term, unsigned int last_term, ...)
    {
      va_list ap;

      assert(first_term % 2 == 0);
      assert(last_term % 2 == 0);
      assert(last_term >= last_term);
      _first_term = first_term;
      _last_term = last_term;
      _coeff.clear();
      _coeff.resize(_last_term + 1, 0.0);

      va_start(ap, last_term);

      for (unsigned int i = first_term; i <= last_term; i += 2)
        _coeff[i] = va_arg(ap, double);

      va_end(ap);
    }

    void Polynomial::set_odd(unsigned int first_term, unsigned int last_term, ...)
    {
      va_list ap;

      assert(first_term % 2 == 1);
      assert(last_term % 2 == 1);
      assert(last_term >= last_term);
      _first_term = first_term;
      _last_term = last_term;
      _coeff.clear();
      _coeff.resize(_last_term + 1, 0.0);

      va_start(ap, last_term);

      for (unsigned int i = first_term; i <= last_term; i += 2)
        _coeff[i] = va_arg(ap, double);

      va_end(ap);
    }

    void Polynomial::set_term_factor(unsigned int n, double c)
    {
      if (_last_term < n)
        {
          _last_term = n;
          _coeff.resize(n + 1, 0.0);
        }
      else if (_first_term > n)
        {
          _first_term = n;
        }

      _coeff[n] = c;
    }

    void Polynomial::set_last_term(unsigned int n)
    {
      _last_term = n;

      if (_first_term > _last_term)
        _first_term = _last_term;

      _coeff.resize(_last_term + 1, 0.0);
    }

    void Polynomial::set_first_term(unsigned int n)
    {
      _first_term = n;

      if (_first_term > _last_term)
        _last_term = _first_term;

      _coeff.resize(_last_term + 1, 0.0);

      for (unsigned int i = 0; i < _first_term; i++)
        _coeff[i] = 0.0;
    }

    double Polynomial::sagitta(double r) const
    {
      double y = 0;
      int i;

      for (i = _last_term; i >= (int)_first_term; i--)
        y = y * r + _coeff[i];

      y *= pow(r, (double)(i + 1));

      return y;
    }

    double Polynomial::derivative(double r) const
    {
      double y = 0;
      int i;

      for (i = _last_term; i >= (int)_first_term; i--)
        y = y * r + (double)i * _coeff[i];

      y *= pow(r, (double)i);

      return y;
    }

  }

}

