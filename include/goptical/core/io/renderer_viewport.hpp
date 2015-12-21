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


#ifndef GOPTICAL_RENDERER_VIEWPORT_HH_
#define GOPTICAL_RENDERER_VIEWPORT_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/math/transform.hpp"
#include "goptical/core/io/renderer.hpp"

namespace _goptical {

  namespace io {

    /**
       @short Base class for viewport based rendering drivers
       @header <goptical/core/io/RendererViewport
       @module {Core}

       This class defines an interface for rendering drivers which rely
       on viewport windows, scaling and 2d projection operations. It's designed to
       be used as a base class for such renderers; this mainly includes
       pixel based output renderers.

       Renderers which write output in vector format in world
       coordinates directly do not need this class as base.
     */

    class RendererViewport : public Renderer
    {
    public:

      RendererViewport();

      /** Set format used to print numeric values */
      inline void set_numeric_format(std::ios_base::fmtflags format, unsigned int precision);

      /** @alias set_margin_ratio1
          Set amount of 2d margin space for @ref set_window operation 
          as ratio of window. */
      void set_margin_ratio(double left, double bottom, double right, double top);

      /** @see __set_margin_ratio1__ */
      inline void set_margin_ratio(double width, double height);

      /** @alias set_margin1
          Set amount of 2d margin space for @ref set_window operation in
          world coordinates units. */
      void set_margin(double left, double bottom, double right, double top);

      /** @see __set_margin1__ */
      inline void set_margin(double width, double height);

      /** @alias set_margin_output1
          Set amount of 2d margin space for @ref set_window operation in
          output size units (usually pixels). */
      void set_margin_output(double left, double bottom, double right, double top);

      /** @see __set_margin_output1__ */
      inline void set_margin_output(double width, double height);

      /** Update 2d output size, not all drivers support this */
      virtual void set_2d_size(double width, double height);

      /** @multiple Set 2d viewport window. This function set 3d
          projection to orthographic. */
      virtual void set_window(const math::Vector2 &center, const math::Vector2 &size, bool keep_aspect = true);
      void set_window(const math::VectorPair2 &window, bool keep_aspect = true);
      void set_window(const math::Vector2 &center, double radius, bool keep_aspect = true);

      /** Get current viewport window */
      inline const math::VectorPair2 & get_window() const;

      /** Set 3d perspective projection mode. This function reset the
          viewport window to (-1,1). @see set_window @see set_fov */
      virtual void set_perspective() = 0;

      GOPTICAL_ACCESSORS(double, fov, "field of view in degrees");

      /** Get reference to 3d camera transform */
      virtual math::Transform<3> get_camera_transform() const = 0;
      /** Get modifiable reference to 3d camera transform */
      virtual void set_camera_transform(const math::Transform<3> & t) = 0;

      /** Set 3d camera rotation from direction vector */
      void set_camera_direction(const math::Vector3 &dir);

      /** Set 3d camera rotation from direction vector */
      void set_camera_position(const math::Vector3 &pos);

      /** This function can be used to setup a pages grid. Current
          output page must be selected with @ref set_page. */
      virtual void set_page_layout(unsigned int cols, unsigned int rows);

      /** Select current page when multiple pages layout is in
          use. @see set_page_layout */
      virtual void set_page(unsigned int page);

      ////////////// Internal stuff

      /** @internal clear output, may not be supported by all drivers */
      virtual void clear();
      /** @internal write output, may not be supported by all drivers */
      virtual void flush();

      /** @internal Draw plot */
      virtual void draw_plot(const data::Plot &plot);

      /** Draw separations between pages, output window must have been setup before. */
      virtual void draw_pages_grid(const Rgb &rgb = rgb_gray);

      /** @internal Draw frame */
      virtual void draw_frame_2d();

      /** @internal Draw scale, axis, ruler tics. Current 2d window is used as range */
      virtual void draw_axes_2d(const RendererAxes &a);
      /** @internal Draw scale, axis, ruler tics ... */
      virtual void draw_axes_3d(const RendererAxes &a);

    protected:

      /** Update 2d window, called from @mref set_window and @ref set_perspective */
      virtual void update_2d_window();
      /** Set 3d projection to orthographic, called from @mref set_window. */
      virtual void set_orthographic() = 0;

      /** @internal set window for 2d plot */
      void set_2d_plot_window(const data::Plot &plot);
      /** @internal Draw plot data */
      virtual void draw_plot_data_2d(const data::Set1d &data, const data::Plotdata &plot);
      /** @internal Draw plot data */
      virtual void draw_plot_data_3d(const data::Grid &data, const data::Plotdata &plot);

      /** transform 2d position for ploting to output window */
      inline double x_trans_pos(double x) const;
      /** transform 2d position for ploting to output window */
      inline double y_trans_pos(double y) const;

      /** scale 2d len for ploting to output window */
      inline double x_scale(double x) const;
      /** scale 2d len for ploting to output window */
      inline double y_scale(double y) const;

      /** Current 2d viewport window */
      math::VectorPair2 _window2d_fit;

      /** Current 2d viewport window (with margins) */
      math::VectorPair2 _window2d;

      /** 2d device resolution */
      math::Vector2 _2d_output_res;

      enum margin_type_e
        {
          /** _margin contains a size ratio */
          MarginRatio,
          /** _margin contains the width in window size units */
          MarginLocal,
          /** _margin contains the width in output size units */
          MarginOutput,
        };

      margin_type_e _margin_type;

      /** Margin size or ratio */
      math::VectorPair2 _margin;

      /** Current layout rows and columns counts */
      unsigned int _rows, _cols;

      /** Current page id */
      unsigned int _pageid;

      /** Current 2d page window */
      math::VectorPair2 _page;

      double _fov;

    private:
      template <int N>
      void draw_axes(const RendererAxes &a);

      template <int N>
      void draw_axes_tic(const RendererAxes &a, unsigned int i,
                         int pow10, bool oor, double x);

      unsigned int _precision;
      std::ios_base::fmtflags _format;
    };

  }
}

#endif

