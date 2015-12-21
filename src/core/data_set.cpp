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

#include <limits>
#include <iostream>

#include <goptical/core/Error>
#include <goptical/core/data/Set>

namespace _goptical
{

  namespace data
  {

    Set::Set()
      : _version(0)
    {
    }

    Set::~Set()
    {
    }

    math::range_t Set::get_y_range() const
    {
      math::range_t r(std::numeric_limits<double>::max(),
                      std::numeric_limits<double>::min());

      unsigned int d = get_dimensions();
      unsigned int x[d];
      unsigned int c[d];

      for (unsigned int i = 0; i < d; i++)
        {
          if (get_count(i) == 0)
            throw Error("data set contains no data");

          x[i] = 0;
          c[i] = get_count(i) - 1;
        }

      while (1)
        {
          double y = get_y_value(x);

          if (y < r.first)
            r.first = y;

          if (y > r.second)
            r.second = y;

          for (unsigned int i = 0; ; )
            {
              if (x[i] < c[i])
                {
                  x[i]++;
                  break;
                }
              else
                {
                  x[i++] = 0;

                  if (i == d)
                    return r;
                }
            }
        }
    }


    std::ostream & operator<<(std::ostream &o, const Set &s)
    {
      switch (s.get_dimensions())
        {
        case 2:
          for (unsigned int x1 = 0; x1 < s.get_count(0); x1++)
            {
              for (unsigned int x2 = 0; x2 < s.get_count(1); x2++)
                {
                  unsigned int t[2];

                  t[0] = x1;
                  t[1] = x2;

                  o << s.get_x_value(x1, 0) << " " 
                    << s.get_x_value(x2, 1) << " "
                    << s.get_y_value(t) << " " << std::endl;
                }
              o << std::endl;
            }
          break;

        case 1:
          for (unsigned int x = 0; x < s.get_count(0); x++)
            o << s.get_x_value(x, 0) << " " << s.get_y_value(&x) << std::endl;
          break;
        }

      return o;
    }

  }

}

