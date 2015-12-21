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

#include <goptical/core/sys/System>
#include <goptical/core/sys/OpticalSurface>

#include <goptical/core/shape/Disk>
#include <goptical/core/curve/Sphere>
#include <goptical/core/curve/Flat>

#include <goptical/core/trace/Ray>
#include <goptical/core/trace/Distribution>
#include <goptical/core/trace/Result>

#include <goptical/core/io/Rgb>
#include <goptical/core/io/Renderer>

namespace _goptical {

  namespace sys {

    OpticalSurface::OpticalSurface(const math::VectorPair3 &p,
                                   const const_ref<curve::Base> &curve,
                                   const const_ref<shape::Base> &shape,
                                   const const_ref<material::Base> &left,
                                   const const_ref<material::Base> &right)
      : Surface(p, curve, shape)
    {
      _mat[0] = left;
      _mat[1] = right;
    }

    OpticalSurface::OpticalSurface(const math::VectorPair3 &p,
                                   double roc, double ap_radius,
                                   const const_ref<material::Base> &left,
                                   const const_ref<material::Base> &right)
      : Surface(p, roc == 0. ? const_ref<curve::Base>(curve::flat)
                             : const_ref<curve::Base>(ref<curve::Sphere>::create(roc)),
                ref<shape::Disk>::create(ap_radius))
    {
      _mat[0] = left;
      _mat[1] = right;
    }

    OpticalSurface::OpticalSurface(const math::VectorPair3 &p,
                                   const const_ref<curve::Base> &curve, double ap_radius,
                                   const const_ref<material::Base> &left,
                                   const const_ref<material::Base> &right)
      : Surface(p, curve, ref<shape::Disk>::create(ap_radius))
    {
      _mat[0] = left;
      _mat[1] = right;
    }

    OpticalSurface::~OpticalSurface()
    {
    }

    io::Rgb OpticalSurface::get_color(const io::Renderer &r) const
    {
      // FIXME color depends on materials
      return r.get_style_color(io::StyleSurface);
    }

    bool OpticalSurface::refract(const math::VectorPair3 &ray, math::Vector3 &dir,
                                 const math::Vector3 &normal, double refract_index) const
    {
      // Algorithm from Bram de Greve article "Reflections & Refractions in Raytracing"
      // http://www.bramz.org/

      assert(fabs(normal.len() - 1.0) < 1e-10);
      assert(fabs((ray.direction().len()) - 1.0) < 1e-10);

      double    cosi = (normal * ray.direction());
      double    sint2 = math::square(refract_index) * (1.0 - math::square(cosi));

      if (sint2 > 1.0)
        return false;   // total internal reflection

      dir = ray.direction() * refract_index - normal * (refract_index * cosi + sqrt(1.0 - sint2));

      return true;
    }

    void OpticalSurface::reflect(const math::VectorPair3 &ray, math::Vector3 &dir, const math::Vector3 &normal) const
    {
      // Algorithm from Bram de Greve article "Reflections & Refractions in Raytracing"
      // http://www.bramz.org/

      assert(fabs(normal.len() - 1.0) < 1e-10);
      assert(fabs((ray.direction().len()) - 1.0) < 1e-10);

      double    cosi = (normal * ray.direction());

      dir = ray.direction() - normal * (2.0 * cosi);
    }

    void OpticalSurface::trace_ray_simple(trace::Result &result,
                                          trace::Ray &incident,
                                          const math::VectorPair3 &local,
                                          const math::VectorPair3 &intersect) const
    {
      math::Vector3     direction;      // refracted ray direction

      bool right_to_left = intersect.normal().z() > 0;
      const material::Base *prev_mat = _mat[right_to_left].ptr();
      const material::Base *next_mat = _mat[!right_to_left].ptr();

      // check ray didn't "escaped" from its material
      std::cout << prev_mat->name << " " << next_mat->name <<
                " " << incident.get_material()->name << std::endl;

      if (prev_mat != incident.get_material()) {
        return;
      }

      double wl = incident.get_wavelen();
      double index = prev_mat->get_refractive_index(wl) / next_mat->get_refractive_index(wl);

      if (!refract(local, direction, intersect.normal(), index))
        {
          trace::Ray &r = result.new_ray();
          // total internal reflection

          r.set_wavelen(wl);
          r.set_intensity(incident.get_intensity());
          r.set_material(prev_mat);
          r.origin() = intersect.origin();
          reflect(local, r.direction(), intersect.normal());
          r.set_creator(this);
          incident.add_generated(&r);

          return;
        }

      // transmit
      if (!next_mat->is_opaque())
        {
          trace::Ray &r = result.new_ray();

          r.set_wavelen(wl);
          r.set_intensity(incident.get_intensity());
          r.set_material(next_mat);
          r.origin() = intersect.origin();
          r.direction() = direction;
          r.set_creator(this);
          incident.add_generated(&r);
        }

      // reflect
      if (next_mat->is_reflecting())
        {
          trace::Ray &r = result.new_ray();

          r.set_wavelen(wl);
          r.set_intensity(incident.get_intensity());
          r.set_material(prev_mat);
          r.origin() = intersect.origin();
          reflect(local, r.direction(), intersect.normal());
          r.set_creator(this);
          incident.add_generated(&r);
        }
    }

    void OpticalSurface::trace_ray_intensity(trace::Result &result,
                                             trace::Ray &incident,
                                             const math::VectorPair3 &local,
                                             const math::VectorPair3 &intersect) const
    {
      math::Vector3     direction;

      bool right_to_left = intersect.normal().z() > 0;

      const material::Base *prev_mat = _mat[right_to_left].ptr();
      const material::Base *next_mat = _mat[!right_to_left].ptr();

      // check ray didn't "escaped" from its material
      if (prev_mat != incident.get_material())
        return;

      double wl = incident.get_wavelen();
      double index = prev_mat->get_refractive_index(wl) / next_mat->get_refractive_index(wl);
      double intensity = incident.get_intercept_intensity();

      if (!refract(local, direction, intersect.normal(), index))
        {
          // total internal reflection
          trace::Ray &r = result.new_ray();

          r.set_wavelen(wl);
          r.set_intensity(intensity);
          r.set_material(prev_mat);
          r.origin() = intersect.origin();
          reflect(local, r.direction(), intersect.normal());
          r.set_creator(this);
          incident.add_generated(&r);

          return;
        }

      // transmit
      if (!next_mat->is_opaque())
        {
          double tintensity = intensity * next_mat->get_normal_transmittance(prev_mat, wl);

          if (tintensity >= get_discard_intensity())
            {
              trace::Ray &r = result.new_ray();

              r.set_wavelen(wl);
              r.set_intensity(tintensity);
              r.set_material(next_mat);
              r.origin() = intersect.origin();
              r.direction() = direction;
              r.set_creator(this);
              incident.add_generated(&r);
            }
        }

      // reflect
      {
        double rintensity = intensity * next_mat->get_normal_reflectance(prev_mat, wl);

        if (rintensity >= get_discard_intensity())
          {
            trace::Ray &r = result.new_ray();

            r.set_wavelen(wl);
            r.set_intensity(rintensity);
            r.set_material(prev_mat);
            r.origin() = intersect.origin();
            reflect(local, r.direction(), intersect.normal());
            r.set_creator(this);
            incident.add_generated(&r);
          }
      }

    }

    void OpticalSurface::set_material(unsigned index, const const_ref<material::Base> &m)
    {
      assert(index < 2);

      if (!m.valid() && get_system())
        _mat[index] = get_system()->get_environment_proxy();
      else
        _mat[index] = m;
    }

    void OpticalSurface::system_register(system &s)
    {
      Surface::system_register(s);

      for (unsigned int i = 0; i < 2; i++)
        if (!_mat[i].valid())
          _mat[i] = s.get_environment_proxy();
    }

    void OpticalSurface::system_unregister()
    {
      for (unsigned int i = 0; i < 2; i++)
        if (_mat[i].ptr() == &get_system()->get_environment_proxy())
          _mat[i] = material::none;

      Surface::system_unregister();
    }

    void OpticalSurface::print(std::ostream &o) const
    {
      o << " [" << id() << "]" << typeid(*this).name() << " at " << get_position() << " "
        << typeid(get_material(0)).name() << " " << typeid(get_material(1)).name();
    }

  }

}

