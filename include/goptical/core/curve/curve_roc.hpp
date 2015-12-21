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


#ifndef GOPTICAL_CURVE_ROC_HH_
#define GOPTICAL_CURVE_ROC_HH_

#include "goptical/core/common.hpp"

namespace _goptical {

  namespace curve {

    /**
       @short Base class for curves with a radius of curvature
       @header <goptical/core/curve/curveRoc
       @module {Core}
     */
    class curveRoc
    {
    public:
      /** Set the radius of curvature */
      inline void set_roc(double roc);

      /** Get the radius of curvature */
      inline double get_roc() const;

    protected:
      inline curveRoc(double roc);

      double _roc;
    };

  }
}

#endif

