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

#include <goptical/core/math/Vector>
#include <goptical/core/math/VectorPair>

#include <goptical/core/shape/Infinite>
#include <goptical/core/curve/Flat>

#include <goptical/core/sys/system>
#include <goptical/core/sys/Group>
#include <goptical/core/sys/Surface>
#include <goptical/core/sys/Element>

#include <stdlib.h>

using namespace goptical;

#define fail(x)                                 \
{                                               \
  std::cerr << x << std::endl;                  \
  exit(1);                                      \
}

#define ECOUNT 5

#define COMPARE_VAL(a, b) (fabs((a)-(b)) < 1e-10)
#define COMPARE_VECTOR3(a, b) \
  (COMPARE_VAL(a.x(), b.x()) && COMPARE_VAL(a.y(), b.y()) && COMPARE_VAL(a.z(), b.z()))
#define COMPARE_PLANE(a, b) \
  (COMPARE_VECTOR3(a[0], b[0]) && COMPARE_VECTOR3(a[1], b[1]))

math::Vector3 rand_vector()
{
  return math::Vector3(drand48(), drand48(), drand48());
}

math::VectorPair3 rand_plane()
{
  math::Vector3 p = rand_vector();
  math::Vector3 d = rand_vector();
  d.normalize();
  return math::VectorPair3(p, d);
}

int main()
{
  sys::system   sys;

  sys::Group    g1  (math::vector3_0);
  sys::Group    g21 (math::vector3_0);
  sys::Group    g22 (math::vector3_0);
  sys::Surface  s211(math::vector3_0, curve::flat, shape::infinite);
  sys::Surface  s221(math::vector3_0, curve::flat, shape::infinite);

  g22.add(s221);
  g1. add(g21);
  sys.add(g1);
  g1. add(g22);
  g21.add(s211);

  // test position

  if (!COMPARE_VECTOR3(s211.get_local_position(), math::Vector3(0, 0, 0)))
    fail(__LINE__);

  if (!COMPARE_VECTOR3(s211.get_position(), math::Vector3(0, 0, 0)))
    fail(__LINE__);

  g21.set_local_position(math::Vector3(1, 2, 3));

  if (!COMPARE_VECTOR3(s211.get_local_position(), math::Vector3(0, 0, 0)))
    fail(__LINE__);

  if (!COMPARE_VECTOR3(s211.get_position(), math::Vector3(1, 2, 3)))
    fail(__LINE__);  

  g1.set_local_position(math::Vector3(3, 2, 1));

  if (!COMPARE_VECTOR3(s211.get_position(), math::Vector3(4, 4, 4)))
    fail(__LINE__);  

  s211.set_local_position(math::Vector3(7, 7, 7));

  if (!COMPARE_VECTOR3(s211.get_position(), math::Vector3(11, 11, 11)))
    fail(__LINE__);  

  s211.set_position(math::Vector3(9, 9, 9));

  if (!COMPARE_VECTOR3(s211.get_position(), math::Vector3(9, 9, 9)))
    fail(__LINE__);  

  if (!COMPARE_VECTOR3(s211.get_local_position(), math::Vector3(5, 5, 5)))
    fail(__LINE__);  

  g21.rotate(0, 0, -90);

  if (!COMPARE_VECTOR3(s211.get_position(), math::Vector3(-1, 9, 9)))
    fail(__LINE__);  

  s211.set_position(math::Vector3(3, 4, 5));

  if (!COMPARE_VECTOR3(s211.get_local_position(), math::Vector3(0, 1, 1)))
    fail(__LINE__);  

  g1.rotate(0, 0, 90);

  if (!COMPARE_VECTOR3(s211.get_position(), math::Vector3(5, 2, 5)))
    fail(__LINE__);

  // test set/get

  sys::Element *e[ECOUNT] = {
    &g1, &g21, &g22, &s211, &s221
  };

  for (int j = 0; j < 10; j++)
    {
      for (int i = 0; i < ECOUNT; i++)
        {
          math::VectorPair3 p;
          math::VectorPair3 r;

          p = rand_plane();
          e[i]->set_plane(p);
          r = e[i]->get_plane();

          if (!COMPARE_PLANE(p, r))
            fail(__LINE__ << ":" << p << " " << r);

          p = rand_plane();
          e[i]->set_local_plane(p);
          r = e[i]->get_local_plane();

          if (!COMPARE_PLANE(p, r))
            fail(__LINE__ << ":" << p << " " << r);
        }
    }
}

