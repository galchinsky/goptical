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

#include <goptical/core/io/ImportOslo>
#include <goptical/core/Error>

#include <goptical/core/material/Dielectric>
#include <goptical/core/material/Catalog>
#include <goptical/core/material/Sellmeier>
#include <goptical/core/material/Conrady>
#include <goptical/core/material/Schott>
#include <goptical/core/material/Air>

#include <fstream>
#include <sstream>
#include <cstdlib>

namespace _goptical {

  namespace io {

    material::Catalog & ImportOslo::import_catalog(const std::string &filename)
    {
      std::ifstream file (filename.c_str(), std::ios::in);
      std::string line;

      if (!file)
        throw Error("Unable to open file");

      ref<material::Catalog> cat = GOPTICAL_REFNEW(material::Catalog, filename);

      // skip header line
      std::getline(file, line);

      while (std::getline(file, line))
        {
          const char *buf = line.c_str();
          char name[16];
          double density, dndt, tce, transmit;
          unsigned int formula, coeffcnt, len, i;
          double coef[6];

          ////////////////////////////////////////////////////
          // Get general glass information and coefficients

          if (sscanf(buf, "%15s %*s %*s %lf %lf %lf %lf %*s %*s %*s %*s %*s %u %u%n",
                     name, &density, &dndt, &tce, &transmit, &formula, &coeffcnt, &len) < 7)
            continue;
          buf += len;

          if (coeffcnt > 6)
            continue;

          for (i = 0; i < coeffcnt; i++)
            coef[i] = std::strtod(buf, (char**)&buf);

          ref<material::Dielectric> mat;

          switch (formula)
            {
            case 1:
              mat = GOPTICAL_REFNEW(material::Schott, coef[0], coef[1], coef[2], coef[3], coef[4], coef[5]);
              break;

            case 2:
              mat = GOPTICAL_REFNEW(material::Sellmeier, coef[0], coef[3], coef[1], coef[4], coef[2], coef[5]);
              break;

            case 3:
              mat = GOPTICAL_REFNEW(material::Conrady, coef[0], coef[1], coef[2]);
              break;

            default:
              continue;
            }

          mat->set_measurement_medium(material::air);

          cat->add_material(name, *mat);

          ////////////////////////////////////////////////////
          // Get thermal data

          if (sscanf(buf, "%u %u%n", &formula, &coeffcnt, &len) < 2)
            continue;
          buf += len;

          if (coeffcnt > 6)
            continue;

          for (i = 0; i < coeffcnt; i++)
            coef[i] = strtod(buf, (char**)&buf);

          switch (formula)
            {
            case 1:
              mat->set_temperature_schott(coef[0], coef[1], coef[2], coef[3], coef[4], coef[5]);
              break;

            case 2:
              mat->set_temperature_dndt(coef[0] * 1e-6f / 1000.0 /* FIXME check this */ );
              break;

            default:
              break;
            }

          ////////////////////////////////////////////////////
          // Get internal transmittance data

          if (sscanf(buf, "%u %u%n", &formula, &coeffcnt, &len) < 2)
            continue;
          buf += len;

          if (formula == 1 && coeffcnt < 256)
            {
              for (i = 0; i < coeffcnt; i++)
                {
                  double wl = strtod(buf, (char**)&buf);
                  double it = strtod(buf, (char**)&buf);

                  mat->set_internal_transmittance(wl * 1000.f, 5.0, it);
                }
            }

          ////////////////////////////////////////////////////
          // Get wavelen range data

          if (sscanf(buf, "%u %u%n", &formula, &coeffcnt, &len) < 2)
            continue;
          buf += len;

          if (formula == 1 && coeffcnt == 2)
            {
              double low = strtod(buf, (char**)&buf);
              double high = strtod(buf, (char**)&buf);

              mat->set_wavelen_range(low * 1000.0, high * 1000.0);
            }

        }

      return *cat;
    }

  }

}

