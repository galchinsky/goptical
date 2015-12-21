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

#define DPP_DELEGATE_ARGC 5

#include <limits>

#include <goptical/core/math/Vector>

#include <goptical/core/sys/System>
#include <goptical/core/sys/SourcePoint>
#include <goptical/core/sys/Surface>

#include <goptical/core/trace/Ray>
#include <goptical/core/trace/Result>
#include <goptical/core/trace/Params>

namespace _goptical {

  namespace sys {

    SourcePoint::SourcePoint(SourceInfinityMode m, const math::Vector3 &pos_dir)
      : Source(m == SourceAtInfinity
         // position of infinity source is only used for trace::Sequence sort
               ? math::VectorPair3(pos_dir * -1e9, pos_dir)
               : math::VectorPair3(pos_dir, math::vector3_001)),
        _mode(m)
    {
    }

    template <SourceInfinityMode mode>
    void SourcePoint::get_lightrays_(trace::Result &result,
                                     const Element &target) const
    {
      const Surface *starget = dynamic_cast<const Surface*>(&target);

      if (!starget)
        return;

      double rlen = result.get_params().get_lost_ray_length();
      const trace::Distribution &d = result.get_params().get_distribution(*starget);

      auto de = [&]( const math::Vector3 &i ) {
          math::Vector3 r = starget->get_transform_to(*this).transform(i);  // pattern point on target surface
          math::Vector3 direction;
          math::Vector3 position;

          switch (mode)
          {
          case (SourceAtFiniteDistance):
              position = math::vector3_0;
              direction = r.normalized();
              break;
              
          case (SourceAtInfinity):
              direction = math::vector3_001;
              position = math::VectorPair3(starget->get_position(*this) -
                                           math::vector3_001 * rlen, math::vector3_001).
                  pl_ln_intersect(math::VectorPair3(r, direction));
              break;
          }
          
          for (auto&l : this->_spectrum) {
              trace::Ray &r = result.new_ray();
              
              // generated rays use source coordinates
              r.direction() = direction;
              r.origin() = position;
              
              r.set_creator(this);
              r.set_intensity(l.get_intensity()); // FIXME depends on distance from source and pattern density
              r.set_wavelen(l.get_wavelen());
              r.set_material(_mat.valid() ? _mat.ptr() : &get_system()->get_environment_proxy());
          }
      };
      
      starget->get_pattern(de, d, result.get_params().get_unobstructed());

    }
        
    void SourcePoint::generate_rays_simple(trace::Result &result,
                                           const targets_t &entry) const
    {
      for (auto&l : _spectrum) {
        result.add_ray_wavelen(l.get_wavelen());
      }

      switch (_mode)
        {
        case SourceAtFiniteDistance:
          for (auto& target : entry) {
              get_lightrays_<SourceAtFiniteDistance>(result, *target);
          }
          return;

        case SourceAtInfinity:
          for (auto& target : entry) {
              get_lightrays_<SourceAtInfinity>(result, *target);
          }
          return;
        }
    }

    void SourcePoint::generate_rays_intensity(trace::Result &result,
                                              const targets_t &entry) const
    {
      generate_rays_simple(result, entry);
    }

  }

}

