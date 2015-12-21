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


#include <goptical/core/math/VectorPair>
#include <goptical/core/analysis/Focus>
#include <goptical/core/trace/Tracer>
#include <goptical/core/sys/Image>
#include <goptical/core/trace/Ray>
#include <goptical/core/trace/Result>
#include <goptical/core/trace/Distribution>

namespace _goptical
{

  namespace analysis
  {

    Focus::Focus(sys::system &system)
      : PointImage(system),
        _processed_focus(false)
    {
    }

    void Focus::process_focus()
    {
      if (_processed_focus)
        return;

      trace();

      // find beam average vector

      double count = (double)_intercepts->size();
      math::VectorPair3 beam_avg(math::Vector3(0, 0, 0),
                                 math::Vector3(0, 0, 0));

      GOPTICAL_FOREACH(i, *_intercepts)
        {
          trace::Ray    &ray = **i;

          beam_avg.origin() += ray.get_position();
          beam_avg.direction() += ray.get_direction();
        }

      beam_avg.origin() /= count;
      beam_avg.direction().normalize();

      _best_focus.direction() = beam_avg.direction();

      // find the closest point on the average beam line for each ray

      std::vector<double>       list;

      list.reserve(_intercepts->size());

      GOPTICAL_FOREACH(i, *_intercepts)
        {
          trace::Ray    &ray = **i;

          const math::Vector3   &u = beam_avg.direction();
          const math::Vector3   &v = ray.get_direction();
          const math::Vector3   w = beam_avg.origin() - ray.get_position();

          double        b = u * v;
          double        c = v * v;
          double        k = (u * u) * c - math::square(b);

          if (k <= 1e-10)       // skip if beam line and ray almost parallel
            continue;

          double        s = (b * (v * w) - c * (u * w)) / k;

          if (s < 0)            // skip non convergent rays
            continue;

          list.push_back(s);
        }

      if (list.empty())
        throw Error("no convergent ray found for focus analysis");

      double    average = 0;

      GOPTICAL_FOREACH(i, list)
        average += /*square*/(*i);

      average /= (double)list.size();

      //      average = sqrt(average);

      _best_focus.origin() = beam_avg.origin() + beam_avg.direction() * average;

      _processed_focus = true;
    }
  }
}

