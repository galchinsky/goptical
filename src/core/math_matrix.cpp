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

#include <goptical/core/math/Matrix>
#include <goptical/core/math/Vector>
#include <goptical/core/math/Vector>

namespace _goptical {

  namespace math {

    template <int N>
    double Matrix<N>::determinant() const
    {
      assert(N <= 3 && N > 1);

      switch (N)
        {
        case 1:
          return _val[0][0];

        case 2:
          return _val[0][0] * _val[1][1] - _val[0][1] * _val[1][0];

        case 3:
          return _val[0][0] * (_val[1][1] * _val[2][2] - _val[2][1] * _val[1][2])
               - _val[0][1] * (_val[1][0] * _val[2][2] - _val[2][0] * _val[1][2])
               + _val[0][2] * (_val[1][0] * _val[2][1] - _val[2][0] * _val[1][1])
            ;
        }
    }

    template <int N>
    void Matrix<N>::transpose(Matrix &r) const
    {
      for (int i = 0; i < N - 1; i++)
        for (int j = i + 1; j < N; j++)
          {
            r._val[i][j] = _val[j][i];
            r._val[j][i] = _val[i][j];
          }

      for (int i = 0; i < N; i++)
        r._val[i][i] = _val[i][i];
    }

    template <int N>
    void Matrix<N>::inverse(Matrix &r) const
    {
      assert(N <= 3 && N > 1);

      switch (N)
        {
        case 2: {
          // inverse = adjugate / determinant
          double det = _val[0][0] * _val[1][1] - _val[0][1] * _val[1][0];

          assert(det != 0.0);

          r._val[0][0] = _val[1][1] / det;
          r._val[0][1] = -_val[0][1] / det;
          r._val[1][0] = -_val[1][0] / det;
          r._val[1][1] = _val[0][0] / det;
          break;
        }

        case 3: {
          // inverse = adjugate / determinant
          double s1 = _val[1][1] * _val[2][2] - _val[2][1] * _val[1][2];
          double s2 = _val[1][0] * _val[2][2] - _val[2][0] * _val[1][2];
          double s3 = _val[1][0] * _val[2][1] - _val[2][0] * _val[1][1];

          double det = _val[0][0] * s1 - _val[0][1] * s2 + _val[0][2] * s3;

          assert(det != 0.0);

          r._val[0][0] = + s1 / det;
          r._val[0][1] = - (_val[0][1] * _val[2][2] - _val[0][2] * _val[2][1]) / det;
          r._val[0][2] = + (_val[0][1] * _val[1][2] - _val[0][2] * _val[1][1]) / det;
          
          r._val[1][0] = - s2 / det;
          r._val[1][1] = + (_val[0][0] * _val[2][2] - _val[0][2] * _val[2][0]) / det;
          r._val[1][2] = - (_val[0][0] * _val[1][2] - _val[0][2] * _val[1][0]) / det;

          r._val[2][0] = + s3 / det;
          r._val[2][1] = - (_val[0][0] * _val[2][1] - _val[0][1] * _val[2][0]) / det;
          r._val[2][2] = + (_val[0][0] * _val[1][1] - _val[0][1] * _val[1][0]) / det;
          break;
        }

        }
    }

    template <int N>
    void Matrix<N>::adjugate(Matrix &r) const
    {
      assert(N <= 3 && N > 1);

      switch (N)
        {
        case 2:
          r._val[0][0] = _val[1][1];
          r._val[0][1] = -_val[0][1];
          r._val[1][0] = -_val[1][0];
          r._val[1][1] = _val[0][0];
          break;

        case 3:
          r._val[0][0] = + _val[1][1] * _val[2][2] - _val[1][2] * _val[2][1];
          r._val[0][1] = - _val[0][1] * _val[2][2] + _val[0][2] * _val[2][1];
          r._val[0][2] = + _val[0][1] * _val[1][2] - _val[0][2] * _val[1][1];

          r._val[1][0] = - _val[1][0] * _val[2][2] + _val[1][2] * _val[2][0];
          r._val[1][1] = + _val[0][0] * _val[2][2] - _val[0][2] * _val[2][0];
          r._val[1][2] = - _val[0][0] * _val[1][2] + _val[0][2] * _val[1][0];

          r._val[2][0] = + _val[1][0] * _val[2][1] - _val[1][1] * _val[2][0];
          r._val[2][1] = - _val[0][0] * _val[2][1] + _val[0][1] * _val[2][0];
          r._val[2][2] = + _val[0][0] * _val[1][1] - _val[0][1] * _val[1][0];
          break;
        }
    }

    template <int N> std::ostream & operator<<(std::ostream &o, const Matrix<N> &m)
    {
      o << "[";

      for (unsigned int i = 0; i < N; i++)
        {
          for (unsigned int j = 0; j < N; j++)
            o << m.value(i, j) << ", ";
          if (i + 1 < N)
            o << std::endl << " ";
          else
            o << "]";
        }

      return o;
    }


    template struct Matrix<2>;
    template struct Matrix<3>;

    template std::ostream & operator<<(std::ostream &o, const Matrix<3> &m);
    template std::ostream & operator<<(std::ostream &o, const Matrix<2> &m);

  }

}

