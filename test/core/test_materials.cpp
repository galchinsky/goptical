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

#include <iostream>
#include <cstdlib>

#include <goptical/core/material/Air>
#include <goptical/core/material/Vacuum>
#include <goptical/core/material/Sellmeier>
#include <goptical/core/material/Abbe>
#include <goptical/core/material/Mil>
#include <goptical/core/material/Conrady>

using namespace goptical;

#define FAIL(x)                                 \
{                                               \
  std::cerr << x << std::endl;                  \
  std::exit(1);                                 \
}

#define COMPARE(a_, b_, p)                                              \
  {                                                                     \
    double a = a_;                                                      \
    double b = b_;                                                      \
                                                                        \
    if (fabs((a)-(b)) > p)                                           \
      FAIL(__LINE__ << " " << a << " found, expecting " << b << " " << std::endl); \
  }

int main()
{
  std::cerr.precision(15);

  // test Vacuum mode
  {
    material::Vacuum vac;

    COMPARE( vac.get_refractive_index(550), 1. , 1e-8);
  }

  // test Air model
  {
    material::AirKohlrausch68 airk;

    COMPARE( airk.get_refractive_index(550), 1.00027308 , 1e-7);

    airk.set_temperature(30.);
    airk.set_pressure(2 * airk.std_pressure);
    COMPARE( airk.get_refractive_index(550), 1.00052810 , 1e-7);

    airk.set_pressure(0);
    COMPARE( airk.get_refractive_index(550), 1. , 1e-7);
  }

  // test Air model
  {
    material::AirBirch94 airb;

    COMPARE( airb.get_refractive_index(550), 1.00027308 , 1e-7);

    airb.set_temperature(30.);
    airb.set_pressure(2 * airb.std_pressure);
    COMPARE( airb.get_refractive_index(550), 1.00052826 , 1e-7);

    airb.set_pressure(0);
    COMPARE( airb.get_refractive_index(550), 1. , 1e-7);
  }

  // test Sellmeier and temperature/pressure
  {
    // measurment material
    material::AirKohlrausch68 airm;
    // environment material
    material::AirKohlrausch68 airk;

    // BAF3 Sellmeier
    material::Sellmeier sellm(1.32064267E+000, 8.87798715E-003, 1.33572683E-001,
                              4.20290346E-002, 8.85521821E-001, 1.11729167E+002);

    sellm.set_temperature_schott(1.4100E-006, 1.7300E-008, -1.5100E-011,
                                 5.7600E-007, 4.6800E-010, 2.6700E-001);

    sellm.set_measurement_medium(airm);

    COMPARE( sellm.Base::get_refractive_index(400., airk), 1.6056515 , 1e-7);
    COMPARE( sellm.Base::get_refractive_index(800., airk), 1.5738740 , 1e-7);

    airk.set_temperature(100.);
    sellm.set_temperature(100.);

    COMPARE( sellm.Base::get_refractive_index(400., airk), 1.6061251 , 1e-7);
    COMPARE( sellm.Base::get_refractive_index(800., airk), 1.5741071 , 1e-7);

    airk.set_pressure(10 * airk.std_pressure);

    COMPARE( sellm.Base::get_refractive_index(400., airk), 1.6029774 , 1e-7);
    COMPARE( sellm.Base::get_refractive_index(800., airk), 1.5711062 , 1e-7);
  }

  // test Abbe model
  {
    material::AirKohlrausch68 airm;

    // BAF3
    material::Sellmeier sellm(1.32064267E+000, 8.87798715E-003, 1.33572683E-001,
                              4.20290346E-002, 8.85521821E-001, 1.11729167E+002);

    COMPARE( sellm.get_abbe_vd(), 46.47, 0.01);

    material::AbbeVd abbevd(1.582670, 46.47, .0001);
    abbevd.set_measurement_medium(airm);

    COMPARE( abbevd.Base::get_refractive_index(400., airm), 1.605655 , 1e-6);
    COMPARE( abbevd.Base::get_refractive_index(800., airm), 1.573845 , 1e-6);

    COMPARE( sellm.get_abbe_ve(), 46.18, 0.01);

    material::AbbeVe abbeve(1.585648, 46.18, .0001);
    abbevd.set_measurement_medium(airm);

    COMPARE( abbeve.Base::get_refractive_index(400., airm), 1.605655 , 1e-6);
    COMPARE( abbeve.Base::get_refractive_index(800., airm), 1.573844 , 1e-6);
  }

  // test Mil model
  {
    material::AirKohlrausch68 airm;

    material::Mil mil(583465);
    mil.set_measurement_medium(airm);

    COMPARE( mil.Base::get_refractive_index(400., airm), 1.605 , 2e-3);
    COMPARE( mil.Base::get_refractive_index(800., airm), 1.573 , 2e-3);
  }

  // test Conrady
  {
    material::AirKohlrausch68 airm;
    material::Conrady cn(1.45217000E+000, 1.25800000E-002, 6.65900000E-005);
    cn.set_measurement_medium(airm);

    COMPARE( cn.Base::get_refractive_index(400., airm), 1.4852651 , 2e-7);
    COMPARE( cn.Base::get_refractive_index(800., airm), 1.4680404 , 2e-7);
  }

  return 0;
}

