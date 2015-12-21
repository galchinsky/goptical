/*

      This file is part of the <goptical/core library.
  
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

#include <goptical/core/io/RendererViewport>
#include <goptical/core/math/Vector>

class App
{
public:
  virtual ~App()
  {
  }

protected:
  /** implemented by graphic library */
  virtual void main_loop() = 0;

  /** implemented by <goptical/core app */
  virtual void redraw() = 0;
  virtual void resize(int width, int height) = 0;

  goptical::math::Vector3 translation;
  goptical::math::Vector3 rotation;
  goptical::io::RendererViewport *renderer;
};

