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


#ifndef GOPTICAL_CURVE_POLYNOMIAL_HH_
#define GOPTICAL_CURVE_POLYNOMIAL_HH_

#include <vector>

#include "goptical/core/common.hpp"

#include "goptical/core/curve/rotational.hpp"

namespace _goptical {

  namespace curve {

    /**
       @short Even and odd polynomial curve implementation.
       @header <goptical/core/curve/Polynomial
       @module {Core}
       @main

       This class allow definition of rotationally symmetric high
       order polynomial curves. Unlimited number of terms can be used.
     */
    class Polynomial : public Rotational
    {
    public:
      /** Create a new zero polynomial */
      Polynomial();
      /** Create a new polynomial with given order and
          coefficients. Same syntax as set() function. */
      Polynomial(unsigned int first_term, unsigned int last_term, ...);

      /** Set polynomial order and coefficients.
       @param first_term order of first (lowest) term.
       @param last_term order of last (highest) term.
       @param ... list of coefficient starting at lowest term
      */
      void set(unsigned int first_term, unsigned int last_term, ...);

      /** Set even polynomial order and coefficients.
       @param first_term order of first (lowest) term, must be even.
       @param last_term order of last (highest) term, must be even.
       @param ... list of even coefficient starting at lowest term
      */
      void set_even(unsigned int first_term, unsigned int last_term, ...);

      /** Set odd polynomial order and coefficients.
       @param first_term order of first (lowest) term, must be odd.
       @param last_term order of last (highest) term, must be odd.
       @param ... list of odd coefficient starting at lowest term
      */
      void set_odd(unsigned int first_term, unsigned int last_term, ...);

      /** Adjust coefficient of a single term. Adujst polynomial
          length as needed, setting additional coefficients to 0.  */
      void set_term_factor(unsigned int n, double c);

      /** Set order of first (lowest) term. Polynomial may be
          truncated or extended with 0 coefficients. */

      void set_first_term(unsigned int n);
      /** Set order of last (highest) term. Polynomial may be
          truncated or extended with 0 coefficients.*/
      void set_last_term(unsigned int n);

      double sagitta(double r) const;
      double derivative(double r) const;

    private:
      unsigned int _first_term, _last_term;
      std::vector<double> _coeff;
    };

  }
}

#endif

