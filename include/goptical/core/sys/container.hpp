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

#ifndef GOPTICAL_CONTAINER_HH_
#define GOPTICAL_CONTAINER_HH_

#include <iostream>
#include <list>

#include "goptical/core/common.hpp"

#include "goptical/core/sys/element.hpp"

namespace _goptical {

  namespace sys {

    /**
       @short Base class for system and Group
       @header <goptical/core/sys/Container
       @module {Core}

       This class base contains optical elements membership management code.
       It's used as a base class for @ref system and @ref Group classes.
     */

    class Container
    {
      friend std::ostream & operator<<(std::ostream &o, const Container &s);
      friend class Element;

    public:
      typedef std::list <ref<Element> > element_list_t;

      Container();
      virtual ~Container();

      /** Add an element */
      void add_front(const ref<Element> &e);
      void add(const ref<Element> &e);

      /** Remove an element */
      void remove(Element &e);

      /** Find first element of type X in container and subcontainers */
      template <class X> inline X* find() const;

      /** Invoke a delegate for each element of type X in
          container and subcontainers */
      template <class X>
      inline void get_elements(const std::function<void (const X &)> &d) const;

      /** Disable all elements of specified type which are not specified element */
      template <class X>
      inline void enable_single(const X &e);

      /** Test if element is contained in container and subcontainers */
      template <class X> inline bool contains(const X *x) const;

      /** Return a reference to container children list */
      inline const element_list_t & get_element_list() const;

      /** Get system or element group bounding box */
      math::VectorPair3 get_bounding_box() const;

      /** Setup the renderer 2d viewport to best fit for this
          system or element group. @pb This function calls
          @ref io::Renderer::set_window and @ref io::Renderer::set_feature_size. */
      void draw_2d_fit(io::RendererViewport &r, bool keep_aspect = true) const;

      /** Draw system 2d layout using specified renderer. @see draw_2d_fit */
      void draw_2d(io::Renderer &r) const;

      /** Move the renderer 3d camera to best fit for this
          system or element group. @pb This function calls
          @ref io::RendererViewport::set_camera_transform
          and @ref io::Renderer::set_feature_size. @pb
      */
      void draw_3d_fit(io::RendererViewport &r, double z_offset = 0) const;

      /** Draw system in 3d using specified renderer. @see draw_3d_fit */
      void draw_3d(io::Renderer &r) const;

    protected:

      /** remove all elements in container */
      void remove_all();

      /** called when elements are inserted or removed */
      virtual void added(Element &e) = 0;
      /** called when elements are inserted or removed */
      virtual void removed(Element &e) = 0;

    private:

      element_list_t            _list;
    };

    std::ostream & operator<<(std::ostream &o, const Container &v);

  }
}

#endif

