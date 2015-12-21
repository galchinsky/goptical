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


#include <goptical/core/io/Rgb>

namespace _goptical {

  namespace io {

    const Rgb rgb_black(0.0, 0.0, 0.0);
    const Rgb rgb_red(1.0, 0.0, 0.0);
    const Rgb rgb_green(0.0, 1.0, 0.0);
    const Rgb rgb_blue(0.0, 0.0, 1.0);
    const Rgb rgb_yellow(1.0, 1.0, 0.0);
    const Rgb rgb_cyan(0.0, 1.0, 1.0);
    const Rgb rgb_magenta(1.0, 0.0, 1.0);
    const Rgb rgb_gray(0.5, 0.5, 0.5);
    const Rgb rgb_white(1.0, 1.0, 1.0);

    const Rgb rgb_table[] = 
      {
        Rgb (0.0, 0.0, 0.0),
        Rgb (1.0, 0.0, 0.0),
        Rgb (0.0, 1.0, 0.0),
        Rgb (0.0, 0.0, 1.0),
        Rgb (1.0, 1.0, 0.0),
        Rgb (0.0, 1.0, 1.0),
        Rgb (1.0, 0.0, 1.0),
        Rgb (0.5, 0.5, 0.5),
        Rgb (1.0, 1.0, 1.0),
      };
  }
}

