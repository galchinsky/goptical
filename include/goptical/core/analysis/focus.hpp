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


#ifndef GOPTICAL_ANALYSIS_FOCUS_HH_
#define GOPTICAL_ANALYSIS_FOCUS_HH_

#include "goptical/core/common.hpp"

#include "goptical/core/math/vector_pair.hpp"

#include "goptical/core/analysis/pointimage.hpp"

namespace _goptical
{

  namespace analysis
  {

    /**
       @short Best focus point analysis
       @header <goptical/core/analysis/Focus
       @module {Core}
       @main

       This class is designed to find the best point of focus of
       an optical system.
    */
    class Focus : public PointImage
    {
    public:
      Focus(sys::system &system);

      inline void invalidate();

      /** Get best point of focus in system global coordinates. */
      inline const math::VectorPair3 & get_best_focus();

    private:
      void process_focus();

      bool              _processed_focus;
      math::VectorPair3 _best_focus;
    };

  }
}

#endif

