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


#ifndef GOPTICAL_SHAPE_COMPOSER_HH_
#define GOPTICAL_SHAPE_COMPOSER_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/math/vector_pair.hpp"
#include "goptical/core/math/transform.hpp"
#include "base.hpp"

namespace _goptical {

  namespace shape {

    /**
       @short Enable definition of shape as composition ot other shapes
       @header <goptical/core/shape/Composer
       @module {Core}
       @experimental
       @main

       This class allows definition of a shape by composition of other
       shape models. Coordinate transforms and boolean operations can
       be performed on shapes.

       This class is still experimental, 2d contour and 3d
       tessellation code doesn't give propser results.
     */

    class Composer : public Base
    {
    public:
      class Attributes;

      /** @override */
      bool inside(const math::Vector2 &point) const;
      /** @override */
      double max_radius() const;
      /** @override */
      double min_radius() const;
      /** @override */
      double get_outter_radius(const math::Vector2 &dir) const;
      /** @override */
      double get_hole_radius(const math::Vector2 &dir) const;
      /** @override */
      void get_pattern(const math::Vector2::put_delegate_t  &f, const trace::Distribution &d, bool unobstructed) const;
      /** @override */
      math::VectorPair2 get_bounding_box() const;
      /** @override */
      unsigned int get_contour_count() const;
      /** @override */
      void get_contour(unsigned int contour, const math::Vector2::put_delegate_t  &f, double resolution) const;
      /** @override */
      void get_triangles(const math::Triangle<2>::put_delegate_t  &f, double resolution) const;

      /** Add a new shape to shape composer.
          
          This function returns a reference to an @ref Attributes
          object which may be modified to set shape transform and
          boolean operations.

          The composed shape is the union between all shapes added
          with this function.
       */
      Attributes & add_shape(const const_ref<Base> &shape);

      /** Set ray distribution behavior. Default is to perform
          individual ray distribution on each composer shape.

          Global mode distributes rays over a circle with maximum
          shape radius, it may be used to ensure ray density is the
          same on each shape. Unobstructed ray tracing require global
          distribution too. */
      inline void use_global_distribution(bool use_global);

      /**
         @short Enable definition of shape as composition ot other shapes
         @header <goptical/core/shape/Composer

         This class contains child shape transform and boolean
         operations for the @ref Composer shape class.
       */
      class Attributes
      {
        friend class Composer;

        Attributes(const const_ref<Base> &shape);

      public:

        /** Apply scaling affine transform using scale factors (xscale, yscale) */
        inline Attributes & scale(const math::Vector2 &factor);
        /** Apply rotation affine transform. Angle is in degree. */
        inline Attributes & rotate(double dangle);
        /** Apply translation transform */
        inline Attributes & translate(const math::Vector2 &offset);

        /** Peform boolean 'and' with the given shape */
        Attributes & include(const const_ref<Base> &shape);
        /** Peform boolean 'and not' with the given shape */
        Attributes & exclude(const const_ref<Base> &shape);

      private:
        bool inside(const math::Vector2 &point) const;

        const_ref<Base>         _shape;
        bool                    _exclude;
        std::list <Attributes>  _list;
        math::Transform<2>      _transform;
        math::Transform<2>      _inv_transform;
      };

      Composer();

    private:

      void update();
      void update() const;

      std::list <Attributes>    _list;
      bool                      _update;
      bool                      _global_dist;
      double                    _max_radius;
      double                    _min_radius;
      math::VectorPair2 _bbox;
      unsigned int              _contour_cnt;
    };

  }
}

#endif

