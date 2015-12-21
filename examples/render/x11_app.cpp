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

#include <X11/Xlib.h>

#include <goptical/core/io/RendererX11>
#include <goptical/core/math/Quaternion>
#include <goptical/core/math/Transform>

#include "app.hpp"
#include "newton.hpp"

using namespace goptical;

class X11App : public App
{
public:
  X11App();
  ~X11App();

protected:
  void main_loop();

private:
  Display *disp;
};

X11App::X11App()
{
  io::RendererX11 *xr = new io::RendererX11(800, 600);

  renderer = xr;

  disp = xr->get_disp();
  Window win = xr->get_win();

  XStoreName(disp, win, "goptical/core X11 renderer");

  XSelectInput(disp, win, ButtonPressMask | ButtonReleaseMask |
               PointerMotionMask | ExposureMask | StructureNotifyMask);
}

X11App::~X11App()
{
  delete renderer;
  XCloseDisplay(disp);
}

void X11App::main_loop()
{
  static unsigned int mouse_bt = 0;
  static int mouse_x, mouse_y;

  redraw();

  while (1)
    {
      XEvent e;

      XNextEvent(disp, &e);

      switch (e.type)
        {
        case Expose: {
          if (e.xexpose.count >= 1)
            break;

          redraw();
          continue;
        }

        case ConfigureNotify: {
          XConfigureEvent *ce = (XConfigureEvent*)&e;
          resize(ce->width, ce->height);
          break;
        }

        case ButtonPress: {
          XButtonPressedEvent *pe = (XButtonPressedEvent*)&e;
          mouse_bt |= (1 << pe->button);
          break;
        }

        case ButtonRelease: {
          XButtonReleasedEvent *pe = (XButtonReleasedEvent*)&e;
          mouse_bt &= ~(1 << pe->button);
          break;
        }

        case MotionNotify: {
          XPointerMovedEvent *me = (XPointerMovedEvent*)&e;

          double dx = (double)(mouse_x - me->x_root);
          double dy = (double)(mouse_y - me->y_root);

          if (mouse_bt & 2)
            {
              translation.x() -= dx;
              translation.y() += dy;
            }

          else if (mouse_bt & 4)
            {
              translation.z() += dx;
            }

          else if (mouse_bt & 8)
            {
              rotation.y() += 0.1 * dx;
              rotation.x() += 0.1 * dy;
            }

          if (mouse_bt) {
            redraw();
          }

          mouse_x = me->x_root;
          mouse_y = me->y_root;
          break;
        }
    }
    }
}

int main(int argc, char **argv)
{
  Newton<X11App> app;

  return 0;
}


