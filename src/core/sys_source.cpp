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


#include <goptical/core/sys/Source>

namespace _goptical {

  namespace sys {

    Source::Source(const math::VectorPair3 &position)
      : Element(position),
        _spectrum(),
        _mat()
    {
      _max_intensity = _min_intensity = 1.0;
      _spectrum.push_back(light::SpectralLine(550.0, 1.0));
    }

    void Source::refresh_intensity_limits()
    {
      if (_spectrum.empty())
        return;

      _min_intensity = _max_intensity = _spectrum[0].get_intensity();

      for (unsigned int i = 1; i < _spectrum.size(); i++)
        {
          _min_intensity = std::min(_min_intensity, _spectrum[i].get_intensity());
          _max_intensity = std::max(_max_intensity, _spectrum[i].get_intensity());
        }
    }    

    void Source::generate_rays_simple(trace::Result &result,
                                      const targets_t &entry) const
    {
      throw Error("this light source is not designed generate light rays in simple ray trace mode");
    }

    void Source::generate_rays_intensity(trace::Result &result,
                                         const targets_t &entry) const
    {
      throw Error("this light source is not designed generate light rays in intensity ray trace mode");
    }

    void Source::generate_rays_polarized(trace::Result &result,
                                         const targets_t &entry) const
    {
      throw Error("this light source is not designed generate light rays in polarized ray trace mode");
    }

  }
}

