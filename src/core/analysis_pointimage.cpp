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

#include <goptical/core/trace/Result>
#include <goptical/core/trace/Distribution>
#include <goptical/core/trace/Ray>

#include <goptical/core/sys/Image>
#include <goptical/core/sys/Surface>

#include <goptical/core/analysis/PointImage>

namespace _goptical
{

  namespace analysis
  {
    PointImage::PointImage(sys::system &system)
      : _system(system), _tracer(system),
        _processed_trace(false),
        _image(0),
        _intercepts(0)
    {
      _tracer.get_params().get_default_distribution().set_uniform_pattern();
    }

    PointImage::~PointImage()
    {
    }

    void PointImage::get_default_image()
    {
      if (!_image)
        _image = _system.find<sys::Image>();

      if (!_image)
        throw Error("no image found for analysis");
    }

    void PointImage::trace()
    {
      if (_processed_trace)
        return;

      trace::Result &result = _tracer.get_trace_result();

      get_default_image();
      result.set_intercepted_save_state(*_image, true);
      _tracer.trace();

      //      if (_sys_system.has_exit_pupil())
      //        result.discard_intercepts_not_from(*_image, _system.get_exit_pupil());

      _intercepts = &result.get_intercepted(*_image);

      _processed_trace = true;
    }

  }

}
