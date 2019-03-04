Goptical fork by Dmitry Galchinsky <galchinsky@gmail.com>. Very WIP.

* different directory structure
* cmake instead of autotools

-----

Compiling
=====

Tested for Linux Mint 18.3, but should also work for other Ubuntu-type Linux distributions:

     $ sudo apt-get install libgsl-dev libgsl2 gsl-bin libgd3 libgd-tools libgd-dev libplplot-dev libplplot-c++11 freeglut3 freeglut3-dev libopencv-dev libdime-dev libxmu-dev libxmuu-dev

First, clone the github repo by:

     $ git clone https://github.com/galchinsky/goptical.git
     $ cd goptical

Then create a directory `build` and `cd` there:

     $ mkdir build
     $ cd build

For creating the Makefiles via `cmake`, use:

     $ cmake ../

After successfully creating the Makefiles, perform `make`:

     $ make

As a result, you should find lots of examples in the `examples` sub directory which can be executed and the plots can be inspected by using, e.g., `inkscape`.

-----
 Copyright (C) 2010-2011 Free Software Foundation, Inc
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
