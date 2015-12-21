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


#ifndef GOPTICAL_DATA_PLOT_HH_
#define GOPTICAL_DATA_PLOT_HH_

#include <string>

#include "goptical/core/common.hpp"

#include "goptical/core/io/renderer_axes.hpp"
#include "goptical/core/io/rgb.hpp"
#include "goptical/core/data/plotdata.hpp"
#include "goptical/core/math/vector.hpp"

namespace _goptical {

  namespace data {

    /**
       @short data plots container
       @header <goptical/core/data/Plot
       @module {Core}
       @main

       This class is used to describe a data plot. It contains a list
       of @ref Plotdata objects and describes some plot properties
       (title, range, ...).

       Plots can be built from data sets or obtained directly from
       various analysis functions. They can be rendered on a @ref
       io::RendererViewport object with the @ref draw function.
     */
    class Plot : public ref_base<Plot>
    {
    public:
      /** Create a new empty plot */
      Plot();

      /** Create and add plot data from specified data set. */
      Plotdata & add_plot_data(const Set &data,
                               const io::Rgb &color = io::rgb_red,
                               const std::string &label = "data",
                               PlotStyleMask style = InterpolatePlot | PointPlot);

      /** Add plot data */
      void add_plot_data(Plotdata &data);

      /** Discard all plot data set */
      void erase_plot_data();

      /** Get plot data set count */
      inline unsigned int get_plot_count() const;

      /** Get plot data set at given index */
      inline Plotdata & get_plot_data(unsigned int index);

      /** Get plot data set at given index */
      inline const Plotdata & get_plot_data(unsigned int index) const;

      /** Set plot main title */
      void set_title(const std::string & title);

      /** Get plot main title */
      inline const std::string & get_title() const;

      /** Set color for all plots */
      void set_color(const io::Rgb & color);

      /** Automatically choose different colors for each plot */
      void set_different_colors();

      /** Set plot style for all plot */
      void set_style(PlotStyleMask style);

      /** Swap x and y axis for 2d plots */
      inline void set_xy_swap(bool doswap);

      /** Get x and y axis swap state for 2d plots */
      inline bool get_xy_swap() const;

      /** Set axis position to dataset range */
      void fit_axes_range();

      /** Get plot axes object */
      inline io::RendererAxes & get_axes();

      /** Get plot axes object */
      inline const io::RendererAxes & get_axes() const;

      /** Get data sets dimensions, return 0 if inconsistent */
      unsigned int get_dimensions() const;

      /** Get range of x data in sets */
      math::range_t get_x_data_range(unsigned int dimension = 0) const;

      /** Get range of y data in sets */
      math::range_t get_y_data_range() const;

      /** draw */
      void draw(io::RendererViewport &r);

    private:

      double get_tic_step(const math::range_t &r) const;

      std::string       _title;
      std::vector<Plotdata>     _plots;

      io::RendererAxes  _axes;

      bool              _xy_swap;
    };
  }
}

#endif

