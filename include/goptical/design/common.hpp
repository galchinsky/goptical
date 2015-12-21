/*

      This file is part of the <goptical/core Design library.
  
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

#ifndef GOPTICAL_DESIGN_COMMON_HH_
#define GOPTICAL_DESIGN_COMMON_HH_

namespace goptical {

  namespace Design {

    namespace telescope {

      /** Specify Cassegrain telescope type */
      enum CassegrainType
        {
          ClassicalCassegrain,
          RitcheyChretien,
          DallKirkham,
          PressmannCamichel
        };

    }
  }
}

#ifdef __MKDOC__
# define _goptical <goptical/core
#endif

namespace _goptical {

  /** @module {Design}
      @short Design module classes */
  namespace Design {

    /** @short telescope designs */
    namespace telescope {
      using namespace goptical::Design::telescope;

      class telescope;
      class Newton;
      template <CassegrainType type> class Cassegrain;

    }
  }
}

#endif

