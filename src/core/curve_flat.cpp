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


#include <goptical/core/curve/Flat>
#include <goptical/core/math/Vector>
#include <goptical/core/math/VectorPair>
#include <goptical/core/math/VectorPair>

namespace _goptical {

  namespace curve {

    Flat::Flat()
    {
    }

    double Flat::sagitta(double r) const
    {
      return 0;
    }

    double Flat::derivative(double r) const
    {
      return 1.0;
    }

    /*

    intersection d'un plan defini par :

    P(Px, Py, Pz) appartenant au plan
    N(Px, Py, Pz) normal au plan

    avec une droite AB definie par l'ensemble des points tel que:

    A + * t B

    on a :

    t=(Nz*Pz+Ny*Py+Nx*Px-Az*Nz-Ay*Ny-Ax*Nx)/(Bz*Nz+By*Ny+Bx*Nx)

    */

    bool Flat::intersect(math::Vector3 &point, const math::VectorPair3 &ray) const
    {
      double    s = ray.direction().z();

      if (s == 0)
        return false;

      double    a = -ray.origin().z() / s;

      if (a < 0)
        return false;

      point = ray.origin() + ray.direction() * a;

      return true;
    }

    void Flat::normal(math::Vector3 &normal, const math::Vector3 &point) const
    {
      normal = math::Vector3(0, 0, -1);
    }

    Flat flat;

  }

}

