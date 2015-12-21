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

#include <math.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>

#include <goptical/core/data/DiscreteSet>

// #define TEST_WRITE

using namespace goptical;

const char *srcdir;
static const int N = 15.0;
static const int R = 10.0;

data::DiscreteSet d;

#define fail(x)                                 \
{                                               \
  std::cerr << x << std::endl;                  \
  exit(1);                                      \
}

static inline bool test_eq(double a, double b)
{
  return (fabs(a-b) > 1e-6);
}

static void test(const char *name, data::Interpolation i)
{
  d.set_interpolation(i);
  std::string str(srcdir ? srcdir : ".");
  str += "/test_discrete_set-";

#ifdef TEST_WRITE
  std::ofstream out((str + name + ".txt").c_str());
  out.precision(15);

  for (double x = -N/2.0 - 2.0; x < N/2.0 + 2.0; x += 1.0/R)
    {
      out << x << " "
          << d.interpolate(x) << " "
          << d.interpolate(x, 1) << " "
          << d.interpolate(x, 2) << std::endl;
    }
#else

  std::ifstream in((str + name + ".txt").c_str());

  for (double x = -N/2.0 - 2.0; x < N/2.0 + 2.0; x += 1.0/R)
    {
      double xx, y, yy, yyy;
      in >> xx >> y >> yy >> yyy;

      if (test_eq(x, xx))
        fail(name << ":unexpected x value in test data " << x << ":" << xx);

      if (test_eq(y, d.interpolate(x)))
        fail(name << ":bad y value " << x << " "
             << y << " " << d.interpolate(x));

      if (test_eq(yy, d.interpolate(x, 1)))
        fail(name << ":bad yy value " << x << " "
             << yy << " " << d.interpolate(x, 1));

      if (test_eq(yyy, d.interpolate(x, 2)))
        fail(name << ":bad yyy value " << x << " "
             << yyy << " " << d.interpolate(x, 2));
    }
#endif
}

#define DO_TEST(i) test(#i, data::i)

int main()
{
  srcdir = getenv("srcdir");

  std::string str(srcdir ? srcdir : ".");

#ifdef TEST_WRITE
  double x = -N/2.0;

  // write random test data
  std::ofstream in((str + "/test_discrete_set-in.txt").c_str());
  in.precision(15);

  for (int i = 0; i < N; i++)
    {
      double y = drand48() - 0.5;
      double yy = drand48() - 0.5;
      d.add_data(x, y, yy);
      in << x << " " << y << " " << yy << std::endl;
      x += drand48() + 0.5;
    }
#else

  // read test input
  std::ifstream in((str + "/test_discrete_set-in.txt").c_str());

  for (int i = 0; i < N; i++)
    {
      double x, y, yy;
      in >> x >> y >> yy;
      d.add_data(x, y, yy);
    }
#endif

  DO_TEST(Nearest);
  DO_TEST(Linear);
  DO_TEST(Quadratic);
  DO_TEST(CubicSimple);
  DO_TEST(Cubic);
  DO_TEST(Cubic2);
  DO_TEST(CubicDerivInit);
  DO_TEST(Cubic2DerivInit);
  DO_TEST(CubicDeriv);
  DO_TEST(Cubic2Deriv);

#ifdef TEST_WRITE
  std::cerr << "test data written" << std::endl;
  return 2;
#else
  return 0;
#endif
}

