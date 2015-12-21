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


#ifndef GOPTICAL_MATH_MATRIX_HXX_
#define GOPTICAL_MATH_MATRIX_HXX_

#include "goptical/core/math/vector.hpp"

namespace _goptical {

  namespace math {

    template <int N>
    double Matrix<N>::value(int x, int y) const
    {
      return _val[x][y];
    }

    template <int N>
    double & Matrix<N>::value(int x, int y)
    {
      return _val[x][y];
    }

    template <int N>
    Matrix<N> & Matrix<N>::set(double value)
    {
      for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
          _val[i][j] = value;

      return *this;
    }

    template <int N>
    Matrix<N> & Matrix<N>::set_col(int col, double value)
    {
      for (int i = 0; i < N; i++)
        _val[i][col] = value;

      return *this;
    }

    template <int N>
    Matrix<N> & Matrix<N>::set_row(int row, double value)
    {
      for (int i = 0; i < N; i++)
        _val[row][i] = value;

      return *this;
    }

    template <int N>
    Matrix<N> & Matrix<N>::set_diag(double value)
    {
      for (int i = 0; i < N; i++)
        _val[i][i] = value;

      return *this;
    }

    template <int N>
    Matrix<N> & Matrix<N>::set_col(int col, const Vector<N> &v)
    {
      for (int i = 0; i < N; i++)
        _val[i][col] = v[i];

      return *this;
    }

    template <int N>
    Matrix<N> & Matrix<N>::set_row(int row, const Vector<N> &v)
    {
      for (int i = 0; i < N; i++)
        _val[row][i] = v[i];

      return *this;
    }

    template <int N>
    Matrix<N> & Matrix<N>::set_diag(const Vector<N> &v)
    {
      for (int i = 0; i < N; i++)
        _val[i][i] = v[i];

      return *this;
    }

    template <int N>
    Matrix<N> & Matrix<N>::set_id()
    {
      for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
          _val[i][j] = i == j ? 1.0 : 0.0;

      return *this;
    }

    template <int N>
    Matrix<N> Matrix<N>::operator+(const Matrix &m) const
    {
      Matrix<N> r;

      for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
          r._val[i][j] = _val[i][j] + m._val[i][j];

      return r;
    }

    template <int N>
    Matrix<N> Matrix<N>::operator-(const Matrix &m) const
    {
      Matrix<N> r;

      for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
          r._val[i][j] = _val[i][j] - m._val[i][j];

      return r;
    }

    template <int N>
    Matrix<N> Matrix<N>::operator*(double scale) const
    {
      Matrix<N> r;

      for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
          r._val[i][j] = _val[i][j] * scale;

      return r;
    }

    template <int N>
    Matrix<N> Matrix<N>::operator/(double scale) const
    {
      Matrix<N> r;

      for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
          r._val[i][j] = _val[i][j] / scale;

      return r;
    }

    template <int N>
    Matrix<N> Matrix<N>::operator*(const Matrix<N> &m) const
    {
      Matrix<N> r;

      for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
          {
            double s = 0;

            for (int k = 0; k < N; k++)
              s += _val[i][k] * m._val[k][j];

            r._val[i][j] = s;
          }

      return r;
    }

    template <int N>
    Vector<N> Matrix<N>::operator*(const Vector<N> &v) const
    {
      Vector<N> r;

      for (int i = 0; i < N; i++)
        {
          double s = 0;

          for (int k = 0; k < N; k++)
            s += _val[i][k] * v[k];

          r[i] = s;
        }

      return r;
    }

    template <int N>
    Matrix<N> Matrix<N>::transpose() const
    {
      Matrix<N> r;

      transpose(r);

      return r;
    }

    template <int N>
    Matrix<N> Matrix<N>::inverse() const
    {
      Matrix<N> r;

      inverse(r);

      return r;
    }

    template <int N>
    Matrix<N> Matrix<N>::adjugate() const
    {
      Matrix<N> r;

      adjugate(r);

      return r;
    }

  }

}

#endif

