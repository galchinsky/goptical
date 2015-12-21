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


#ifndef GOPTICAL_MATH_MATRIX_HH_
#define GOPTICAL_MATH_MATRIX_HH_

#include <ostream>

#include "goptical/core/common.hpp"

namespace _goptical {

  namespace math {

    /**
       @short NxN square matrix class.
       @header <goptical/core/math/Matrix
       @module {Core}
     */
    template <int N> struct Matrix
    {
      /** Get value at x row and y column */
      inline double value(int x, int y) const;
      /** Get modifiable reference to value at x row and y column */
      inline double & value(int x, int y);

      /** Set whole matrix to specified value. */
      inline Matrix & set(double value = 0);
      /** Set whole matrix column to specified value. */
      inline Matrix & set_col(int col, double value);
      /** Set whole matrix row to specified value. */
      inline Matrix & set_row(int row, double value);
      /** Set matrix diagonal to specified value. */
      inline Matrix & set_diag(double value = 1);
      /** Set matrix column to specified vector. */
      inline Matrix & set_col(int col, const Vector<N> &v);
      /** Set matrix row to specified vector. */
      inline Matrix & set_row(int row, const Vector<N> &v);
      /** Set matrix diagonal to specified value. */
      inline Matrix & set_diag(const Vector<N> &v);
      /** Setup an identity matrix. */
      inline Matrix & set_id();

      /** Add matrix */
      inline Matrix operator+(const Matrix &m) const;
      /** Subtract matrix */
      inline Matrix operator-(const Matrix &m) const;
      /** Scale matrix */
      inline Matrix operator*(double scale) const;
      /** Scale matrix */
      inline Matrix operator/(double scale) const;
      /** Multiply matrix */
      inline Matrix operator*(const Matrix &m) const;
      /** Multiply matrix with vector. See Vector class for
          vector/matrix multiplication. */
      inline Vector<N> operator*(const Vector<N> &v) const;

      /** compute matrix determinant */
      inline double determinant() const;

      /** replace matrix by the transposed matrix */
      void transpose(Matrix &result) const;
      /** replace matrix by the inverse matrix */
      void inverse(Matrix &result) const;
      /** replace matrix by the adjugate matrix */
      void adjugate(Matrix &result) const;

      /** Get the transposed matrix */
      inline Matrix transpose() const;
      /** Get the inverse matrix */
      inline Matrix inverse() const;
      /** Get the adjugate matrix */
      inline Matrix adjugate() const;

    protected:
      double _val[/* row */N][/* col */N];
    };

    template <int N> std::ostream & operator<<(std::ostream &o, const Matrix<N> &m);

  }
}

#endif

