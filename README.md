Goptical fork by Dmitry Galchinsky <galchinsky@gmail.com>. Very WIP.

* different directory structure
* cmake instead of autotools

-----

Compiling
=====

Tested for Linux Mint 18.3, but should also work for other Ubuntu-type Linux distributions:

     $ sudo apt-get install libgsl-dev libgsl2 gsl-bin libgd3 libgd-tools libgd-dev libplplot-dev libplplot-c++11 freeglut3 freeglut3-dev libopencv-dev libdime-dev libxmu-dev libxmuu-dev libopenblas-dev

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

Usage
=====

Now the `goptical` library is available. To use it in your own programs it is necessary to provide directories for the include files, the library files, and
to link several libraries within the final compilation step. Being in build directory of `goptical`, it is assumed that a file `example.cc` with an optical
system is located directly below the `goptical` directory. Therefore `cd` there and build it via the following code:

     $ cd ../..
     $ g++ -std=c++11 -o example example.cc -Lgoptical/build/src/ -lgoptical_static -lgsl -lblas -lGL -lGLU -Igoptical/include/

The `example` executable is linked with `goptical_static`, because it should be possible to provide it standalone without relying on `goptical`.

TODO: what about cmake based larger projects?

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
