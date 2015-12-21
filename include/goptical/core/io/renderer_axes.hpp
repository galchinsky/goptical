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


#ifndef GOPTICAL_RENDERER_AXES_HH_
#define GOPTICAL_RENDERER_AXES_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/math/vector.hpp"

namespace _goptical {

  namespace io {

    /**
       @short RendererAxes rendering descriptor
       @header <goptical/core/io/RendererAxes
       @module {Core}

       This class describe how axes must be rendered on graphical
       output.
     */
    class RendererAxes
    {
      friend class RendererViewport;
    public:

      /** Specify axes */
      enum AxisMask
        {
          X = 1,
          Y = 2,
          Z = 4,
          XY = 3,
          YZ = 6,
          XZ = 5,
          XYZ = 7
        };

      RendererAxes();

      /** @This sets distance between axis tics to specified value.
          @see {set_tics_count, set_tics_base} */
      void set_tics_step(double step, AxisMask a = XYZ);

      /** @This sets tics count. @see {set_tics_step, set_tics_base} */
      void set_tics_count(unsigned int count, AxisMask a = XYZ);

      /** @This sets distance between axis tics to best fit power of
          specified base divided by sufficient factor of 2 and 5 to
          have at least @tt min_count tics. @see {set_tics_step,
          set_tics_count} */
      void set_tics_base(unsigned int min_count = 5, double base = 10.0, AxisMask a = XYZ);

      /** @This sets axis tics values origin. */
      void set_origin(const math::Vector3 &origin);

      /** @This returns axes tics values origin. */
      inline const math::Vector3 & get_origin() const;

      /** @This returns axis position */
      void set_position(const math::Vector3 &position);

      /** @This returns axis position */
      inline const math::Vector3 & get_position() const;

      /** @This sets grid visibility. Grid points use tic
          step. */
      void set_show_grid(bool show = true);

      /** @see set_show_grid */
      inline bool get_show_grid() const;

      /** @This sets frame visibility. */
      void set_show_frame(bool show = true);

      /** @see set_show_frame */
      inline bool get_show_frame() const;

      /** @This sets axes visibility.*/
      void set_show_axes(bool show = true, AxisMask a = XYZ);

      /** @see set_show_axes */
      inline bool get_show_axes(unsigned int axis) const;

      /** @This sets tics visibility. Tics are located on axes and
          frame. @see {set_show_axes, set_show_frame} */
      void set_show_tics(bool show = true, AxisMask a = XYZ);

      /** @see set_show_tics */
      inline bool get_show_tics(unsigned int axis) const;

      /** @This sets tics value visibility. When frame is visible,
          tics value is located on frame tics instead of axes tics.
          @see {set_show_axes, set_show_frame} */
      void set_show_values(bool show = true, AxisMask a = XYZ);

      /** @see set_show_values */
      inline bool get_show_values(unsigned int axis) const;

      /** @This set axis label */
      void set_label(const std::string &label,
                     AxisMask a = XYZ);

      /** @This sets axis unit.

          When @tt pow10_scale is set, value will be scaled to shorten
          their length and appropriate power of 10 factor will be
          displayed in axis label.

          If @tt si_prefix is set, SI letter decimal prefix is used
          and the @tt pow10 parameter can be used to scale base unit
          by power of ten (useful when input data use scaled SI base unit). */
      void set_unit(const std::string &unit, bool pow10_scale = true,
                    bool si_prefix = true, int si_pow10 = 0, AxisMask a = XYZ);

      /** Get axis label */
      inline const std::string & get_label(unsigned int axis) const;

      /** Set value range for given axis. Default range is [0,0] which
          means automatic range. */
      void set_range(const math::range_t &r,
                     AxisMask a = XYZ);

      /** get distance between axis tics */
      double get_tics_step(int index, const math::range_t &r) const;

    private:

      enum step_mode_e
        {
          step_interval,
          step_count,
          step_base
        };

      struct Axis
      {
        Axis();

        bool _axis;
        bool _tics;
        bool _values;

        step_mode_e _step_mode;
        unsigned int _count;
        double _step_base;
        bool _si_prefix;
        bool _pow10_scale;
        int _pow10;

        std::string     _unit;
        std::string     _label;
        math::range_t   _range;
      };

      struct Axis       _axes[3];
      bool              _grid;
      bool              _frame;
      math::Vector3     _pos;
      math::Vector3     _origin;
    };

  }
};

#endif

