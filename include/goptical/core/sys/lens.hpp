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


#ifndef GOPTICAL_LENS_HH_
#define GOPTICAL_LENS_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/sys/optical_surface.hpp"
#include "goptical/core/sys/group.hpp"

namespace _goptical {

  namespace sys {

    /**
       @short Lens optical element
       @header <goptical/core/sys/Lens
       @module {Core}
       @main

       This class is a @ref Group {group} of @ref OpticalSurface
       {optical surfaces}. It contains functions to describe a
       lens or lens group component in a convenient way.

       If @ref material::none is passed as material, it will be
       replaced by system environement @ref material::Proxy
       {proxy} material when the optical surface becomes part of a
       @ref system.

       @xsee {tuto_lens}
    */

    class Lens : public Group
    {
    public:
      /** Create an empty lens. Surfaces can be added with the @mref
          add_surface functions. */
      Lens(const math::VectorPair3 &p, double offset = 0.,
           const const_ref<material::Base> &env = material::none);

      /** Create a lens at given position with given thickness,
          shapes, curvatures and glass. @see __add_surface1__ */
      Lens(const math::VectorPair3 &p,
           const const_ref<curve::Base> &curve0,
           const const_ref<shape::Base> &shape0,
           const const_ref<curve::Base> &curve1,
           const const_ref<shape::Base> &shape1,
           double thickness0,
           const const_ref<material::Base> &glass0,
           const const_ref<material::Base> &env = material::none);

      /** Create a circular lens with flat or spherical surfaces at
          given position with given thickness, radius of curvature,
          aperture radius and glass. @see __add_surface2__ */
      Lens(const math::VectorPair3 &p,
           double roc0, double ap_radius0,
           double roc1, double ap_radius1,
           double thickness,
           const const_ref<material::Base> &glass0,
           const const_ref<material::Base> &env = material::none);

      virtual ~Lens();

      /** @alias add_surface1
          Add an optical surface with given curve, shape, thickness and material.
      */
      unsigned int add_surface(const const_ref<curve::Base> &curve,
                               const const_ref<shape::Base> &shape,
                               double thickness = 0.,
                               const const_ref<material::Base> &glass = material::none);

      /** @alias add_surface2
          Add a spherical or flat optical surface with circular aperture.

          @param roc spherical radius of curvature. 0 means flat curve.
          @param ap_radius circular aperture radius.
          @param thickness distance to the next surface or exit plane. @see get_exit_plane.
          @param glass material between added surface and next surface.
      */
      unsigned int add_surface(double roc,
                               double ap_radius,
                               double thickness = 0.,
                               const const_ref<material::Base> &glass = material::none);

      /** @alias add_stop1
          Add an aperture stop with given, shape and thickness.
      */
      void add_stop(const const_ref<shape::Base> &shape,
                    double thickness);

      /** @alias add_stop2
          Add an aperture stop with circular aperture.

          @param radius circular aperture radius.
          @param thickness distance to the next stop or exit plane. @see get_exit_plane.
      */
      void add_stop(double ap_radius, double thickness);

      /** Adjust thickness between two surfaces */
      void set_thickness(double thickness, unsigned int index = 0);
      /** Get thickness between two surfaces */
      double get_thickness(unsigned int index = 0) const;

      /** Set glass material */
      void set_glass_material(const const_ref<material::Base> &m,
                              unsigned int index = 0);

      /** Set left material */
      void set_left_material(const const_ref<material::Base> &m);
      /** Set left material */
      void set_right_material(const const_ref<material::Base> &m);

      /** Set left curve */
      void set_left_curve(const const_ref<curve::Base> &c);
      /** Set right curve */
      void set_right_curve(const const_ref<curve::Base> &c);

      /** Set curve of given surface index */
      void set_curve(const const_ref<curve::Base> &c, unsigned int index);

      /** Set lens shape (all surfaces) */
      void set_shape(const const_ref<shape::Base> &s);

      /** Set lens shape of given surface index */
      void set_shape(const const_ref<shape::Base> &s, unsigned int index);

      /** Get a reference to optical surface at given index */
      inline const OpticalSurface & get_surface(unsigned int index) const;
      /** Get a reference to optical surface at given index */
      inline OpticalSurface & get_surface(unsigned int index);

      /** Get a reference to right optical surface element */
      inline const OpticalSurface & get_right_surface() const;
      /** Get a reference to right optical surface element */
      inline OpticalSurface & get_right_surface();

      /** Get a reference to left optical surface element */
      inline const OpticalSurface & get_left_surface() const;
      /** Get a reference to left optical surface element */
      inline OpticalSurface & get_left_surface();

      /** Get plane of last surface + thickness z offset */
      math::VectorPair3 get_exit_plane() const;

    private:

      /** prevent use of @ref Container::add */
      inline void add(const ref<Element> &e);
      /** prevent use of @ref Container::remove */
      inline void remove(Element &e);

      /** @override */
      void draw_2d_e(io::Renderer &r, const Element *ref) const;
      /** @override */
      void draw_3d_e(io::Renderer &r, const Element *ref) const;

      void draw_2d_edge(io::Renderer &r, const Surface &left, double l_y,
                        const Surface &right, double r_y, LensEdge type,
                        const Element *ref) const;

      bool check_thickness(double thickness);

      double _last_pos;

      // preallocated static storage for 8 surface
      vector_pool<OpticalSurface, 8>::block_type _surfaces_storage;
      vector_pool<OpticalSurface, 8> _surfaces;

      ref<Stop> _stop;
      const_ref<material::Base> _next_mat;
    };

  }
}

#endif

