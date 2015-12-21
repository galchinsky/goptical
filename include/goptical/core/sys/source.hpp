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


#ifndef GOPTICAL_SOURCE_HH_
#define GOPTICAL_SOURCE_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/light/spectral_line.hpp"
#include "goptical/core/sys/element.hpp"
#include "goptical/core/sys/surface.hpp"

#include "goptical/core/material/base.hpp"

namespace _goptical {

  namespace sys {

    /**
       @short Base class for light sources
       @header <goptical/core/sys/Source
       @module {Core}

       This class is the base class for all light sources
       implementations.
     */
    class Source : public Element
    {
    public:
      typedef std::vector<const sys::Element *> targets_t;

      /** Create a source at given position. */
      Source(const math::VectorPair3 &position);

      /** Set material where light rays are generated. system
          environment material is used by default. */
      inline void set_material(const const_ref<material::Base> &m);

      /** Add a new wavelen for ray generation */
      inline void add_spectral_line(const light::SpectralLine & l);

      /** Set a wavelen in list for ray generation */
      inline void set_spectral_line(const light::SpectralLine & l,
                                    int index = 0);

      /** Clear ray wavelen list and set a single wavelen */
      inline void single_spectral_line(const light::SpectralLine & l);

      /** Clear wavelen list */
      inline void clear_spectrum();

      /** Get maximal spectral line intensity */
      inline double get_max_intensity() const;

      /** Get minimal spectral line intensity */
      inline double get_min_intensity() const;

      /** Generate light rays from source */
      template <trace::IntensityMode m>
      inline void generate_rays(trace::Result &result,
                                const targets_t &entry) const;

    protected:

      /** This function generate light rays from source. Target entry
          surfaces may be used depending on source model. It must be
          reimplemented by subclasses. */
      virtual void generate_rays_simple(trace::Result &result,
                                        const targets_t &entry) const;

      /** This function process incoming light rays. It must be
          reimplemented by subclasses if the element can interact with
          light in intensity raytrace mode. */
      virtual void generate_rays_intensity(trace::Result &result,
                                           const targets_t &entry) const;

      /** This function process incoming light rays. It must be
          reimplemented by subclasses if the element can interact with
          light in polarized raytrace mode. */
      virtual void generate_rays_polarized(trace::Result &result,
                                           const targets_t &entry) const;

      void refresh_intensity_limits();

      std::vector<light::SpectralLine>  _spectrum;
      double                            _min_intensity, _max_intensity;
      const_ref<material::Base>     _mat;
    };
  }
}

#endif

