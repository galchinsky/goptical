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


#ifndef GOPTICAL_ELEMENT_HH_
#define GOPTICAL_ELEMENT_HH_

#include <iostream>

#include "goptical/core/common.hpp"

#include "goptical/core/math/transform.hpp"
#include "goptical/core/math/vector_pair.hpp"
#include "goptical/core/math/vector_pair.hpp"

namespace _goptical {

  namespace sys {

    /**
       @short Optical element base class
       @header <goptical/core/sys/Element
       @module {Core}

       This is the base class for all optical elements.

       Instances of this class may be registered as members of the
       @ref Container class. It handles element position and transform
       to parent coordinates.

       This class has virtual functions which must be reimplemented to
       handle incoming rays.
     */

    class Element : public ref_base<Element>
    {
      friend class Container;
      friend class system;
      friend class Group;
      friend std::ostream & operator<<(std::ostream &o, const Element &e);

    public:
      typedef std::function<void (const Element &)> put_delegate_t;
      typedef std::function<void (Element &)> mod_delegate_t;

      /** Create a new optical element with given position and
          direction in parent coordinates system. */
      Element(const math::VectorPair3 &plane);

      virtual ~Element();

      /** Set element position in parent local coordinate system */
      inline void set_local_position(const math::Vector3 &v);
      /** Get element position in parent local coordinate system */
      inline const math::Vector3 & get_local_position() const;

      /** Set element direction in parent coordinate system */
      void set_local_direction(const math::Vector3 &v);
      /** Get element direction in parent coordinate system */
      math::Vector3 get_local_direction() const;

      /** Get element position in given element coordinate system */
      math::Vector3 get_position(const Element &e) const;
      /** Get element direction in given element coordinate system */
      math::Vector3 get_direction(const Element &e) const;

      /** Get element position in system global coordinate system */
      math::Vector3 get_position() const;
      /** Set element position in system global coordinate system */
      void set_position(const math::Vector3 &v);

      /** Get element direction in system global coordinates */
      math::Vector3 get_direction() const;
      /** Set element direction in system global coordinates */
      void set_direction(const math::Vector3 &v);

      /** Get reference plane (position and direction) in system global coordinates
          @see get_direction @see get_position */
      math::VectorPair3 get_plane() const;
      /** Get reference plane (position and direction) in given element coordinates
          @see get_direction @see get_position */
      math::VectorPair3 get_plane(const Element &e) const;
      /** Set reference plane (position and direction) in system global coordinates
          @see set_direction @see set_position */
      void set_plane(const math::VectorPair3 &p);

      /** Get reference plane (position and direction) in parent coordinate system 
          @see get_local_direction @see get_local_position */
      math::VectorPair3 get_local_plane() const;
      /** Set reference plane (position and direction) in parent coordinate system
          @see set_local_direction @see set_local_position */
      void set_local_plane(const math::VectorPair3 &p);

      /** Set transform to parant coordinate system */
      inline void set_transform(const math::Transform<3> &t);

      /** Get transform from this element to parent element coordinate system */
      inline const math::Transform<3> & get_transform() const;

      /** Get transform from this element to given element coordinate system */
      const math::Transform<3> & get_transform_to(const Element &e) const;

      /** Get transform from given element to this element coordinate system */
      const math::Transform<3> & get_transform_from(const Element &e) const;

      /** Get transform from this element to given element coordinate
          system. Transform to global coordinates is returned if
          paramter is 0. */
      const math::Transform<3> & get_transform_to(const Element *e) const;

      /** Get transform from given element to this element coordinate
          system. Transform from global coordinates is returned if
          paramter is 0. */
      const math::Transform<3> & get_transform_from(const Element *e) const;

      /** Get transform from this element local to global coordinates */
      const math::Transform<3> & get_global_transform() const;

      /** Get transform from global to this element local coordinates */
      const math::Transform<3> & get_local_transform() const;

      /** Rotate element direction. angles are in degrees */
      inline void rotate(double x, double y, double z);

      /** Get element identifer in current system */
      inline unsigned int id() const;

      /** Get a pointer to system */
      inline system * get_system() const;

      /** Get a pointer to parent element, if any. */
      Group * get_parent() const;

      /** Get bounding box in local coordinates */
      virtual math::VectorPair3 get_bounding_box() const;

      /** Get element version. version is updated each time element
          or its children properties are changed */
      inline unsigned int get_version() const;

      /** Increase current element version */
      void update_version();

      /** Return true if element must be considered */
      inline bool is_enabled() const;

      /** Set enable state */ /* FIXME virtual, recursive for groups */
      inline void set_enable_state(bool enabled);

      /** Batch process a list of light rays interacting with element.
          This function is only used in sequential ray trace mode. */
      template <trace::IntensityMode m>
      inline void process_rays(trace::Result &result,
                               trace::rays_queue_t *input) const;

      /** Draw element 2d layout using the given renderer in given
          element coordinates. */
      virtual void draw_2d_e(io::Renderer &r, const Element *ref) const;

      /** Draw element in 3d using the given renderer in given element
          coordinates */
      virtual void draw_3d_e(io::Renderer &r, const Element *ref) const;

      virtual void print(std::ostream &o) const;

protected:

      /** This function process incoming light rays. It must be
          reimplemented in subclasses if the element can interact with
          light in simple raytrace mode.
          This function is only used in sequential ray trace mode. */
      virtual void process_rays_simple(trace::Result &result,
                                       trace::rays_queue_t *input) const;

      /** This function process incoming light rays. It must be
          reimplemented in subclasses if the element can interact with
          light in intensity raytrace mode.
          This function is only used in sequential ray trace mode. */
      virtual void process_rays_intensity(trace::Result &result,
                                          trace::rays_queue_t *input) const;

      /** This function process incoming light rays. It must be
          reimplemented in subclasses if the element can interact with
          light in polarized raytrace mode.
          This function is only used in sequential ray trace mode. */
      virtual void process_rays_polarized(trace::Result &result,
                                          trace::rays_queue_t *input) const;

      /** This function is called from the @ref system class when the
          element is added to a system */
      virtual void system_register(system &s);

      /** This function is called from the @ref system class when the
          element is removed from a system */
      virtual void system_unregister();

      /** This function is called from the @ref Element base class
          when the local 3d transform has been updated. */
      virtual void system_moved();

private:
      system *_system;
      Container *_container;

      bool      _enabled;
      // must be incremented each time a change is made to element properties
      unsigned int _version;

      // index of element in system
      unsigned int _system_id;
      // position relative to element container
      math::Transform<3> _transform;
    };

    inline std::ostream & operator<<(std::ostream &o, const Element &e);

  }
}

#endif

