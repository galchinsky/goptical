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

#include <goptical/core/io/ImportZemax>
#include <goptical/core/Error>

#include <goptical/core/shape/Infinite>
#include <goptical/core/shape/Disk>
#include <goptical/core/shape/Ring>
#include <goptical/core/shape/Rectangle>
#include <goptical/core/shape/Ellipse>
#include <goptical/core/shape/Composer>

#include <goptical/core/curve/Flat>
#include <goptical/core/curve/Sphere>
#include <goptical/core/curve/Conic>
#include <goptical/core/curve/Parabola>

#include <goptical/core/sys/System>
#include <goptical/core/sys/Stop>
#include <goptical/core/sys/Image>
#include <goptical/core/sys/Surface>
#include <goptical/core/sys/OpticalSurface>

#include <goptical/core/material/Dielectric>
#include <goptical/core/material/Catalog>
#include <goptical/core/material/Sellmeier>
#include <goptical/core/material/SellmeierMod>
#include <goptical/core/material/Conrady>
#include <goptical/core/material/Herzberger>
#include <goptical/core/material/Schott>
#include <goptical/core/material/DispersionTable>
#include <goptical/core/material/Air>
#include <goptical/core/material/Mirror>

#include <goptical/core/math/Transform>

#include <string>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string.h>

#include <iostream>
#define ZMX_WARN(str) std::cerr << str << std::endl;

namespace _goptical {

  namespace io {

    std::string ImportZemax::basename(const std::string &path)
    {
      std::string str(path);

      size_t n = str.rfind(PATH_SEPARATOR);

      if (n != std::string::npos)
        str.erase(0, n + 1);

      n = str.rfind('.');

      if (n != std::string::npos)
        str.erase(n);

      return str;
    }


    ////////////////////////////////////////////////////////////////////////
    // Optical design import
    ////////////////////////////////////////////////////////////////////////

    enum zemax_surface_e
      {
        zs_none,
        zs_standard,
        zs_coordbrk
      };

    enum zemax_aperture_e
      {
        za_none,
        za_circular,
        za_rectangular,
        za_elliptical,
      };

    enum zemax_glass_e
      {
        zg_fixed = 0,
        zg_model = 1,
        zg_pickup = 2,
        zg_subst = 3,
        zg_offset = 4,
        zg_mirror,
        zg_air,
      };

    struct zemax_surface_s
    {
      enum zemax_surface_e type;

      bool stop;

      enum zemax_aperture_e ap_type;
      double ap_params[4];
      bool ap_obscuration;
      bool ap_decenter;

      enum zemax_glass_e gl_type;
      char gl_name[32];
      unsigned int gl_pickup;
      double gl_params[3];

      double roc;
      double coni;
      double thick;

      double params[13];

      inline zemax_surface_s()
        : type(zs_none),
          stop(false),
          ap_type(za_none),
          ap_obscuration(false),
          ap_decenter(false),
          gl_type(zg_air),
          roc(0.0),
          coni(0.0),
          thick(0.0)
      {
        ap_params[0] = ap_params[1] = 0.0;
      }
    };

    const_ref<shape::Base> ImportZemax::get_ap_shape(const struct zemax_surface_s &surf, double unit_factor) const
    {
      const_ref<shape::Base> r;

      switch (surf.ap_type)
        {
        default:
          ZMX_WARN("unknown aperture shape");

        case za_none:
          r = shape::infinite;
          break;

        case za_circular:
          if (surf.ap_params[0] > 0.0)
            r = GOPTICAL_REFNEW(shape::Ring,
                          surf.ap_params[1] * unit_factor,
                          surf.ap_params[0] * unit_factor);
          else
            r = GOPTICAL_REFNEW(shape::Disk, surf.ap_params[1] * unit_factor);
          break;

        case za_elliptical:
          r = GOPTICAL_REFNEW(shape::Ellipse,
                        surf.ap_params[0] * unit_factor,
                        surf.ap_params[1] * unit_factor);
          break;

        case za_rectangular:
          r = GOPTICAL_REFNEW(shape::Rectangle,
                              surf.ap_params[0] * 2. * unit_factor,
                              surf.ap_params[1] * 2. * unit_factor);
          break;
        }

      if (surf.ap_decenter)
        {
          ref<shape::Composer> c = GOPTICAL_REFNEW(shape::Composer);

          c->add_shape(*r).translate(math::Vector2(surf.ap_params[2] * unit_factor,
                                                     surf.ap_params[3] * unit_factor));

          return c;
        }
      else
        {
          return r;
        }
    }

    const_ref<material::Base> ImportZemax::get_glass(sys::system &sys, const struct zemax_surface_s &surf) const
    {
      switch (surf.gl_type)
        {
//      case zg_model:
          // FIXME use Abbe model

        case zg_air:
          return sys.get_environment();

        case zg_mirror:
          return material::mirror;

        case zg_fixed:
          for (auto& c : _cat_list)
            {
              try {
                return c.second->get_material(surf.gl_name);
              } catch (...) {
              }
            }
          throw Error("unable to find glass in loaded catalogs");

        default:
          throw Error("glass type not supported yet");
        }
    }

    ref<sys::system> ImportZemax::import_design(const std::string &filename)
    {
      std::ifstream file(filename.c_str());
      std::string line;

      double unit_factor = 1.0;
      std::vector<zemax_surface_s> surf_array;

      if (!file)
        throw Error("Unable to open file");

      ref<sys::system> sys = GOPTICAL_REFNEW(sys::system);

      while (std::getline(file, line))
        {
        surf_end:

          if (line.size() < 4)
            continue;

          const char *buf = line.c_str();

#define ZMX_TYPE(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

          unsigned int type = ZMX_TYPE(buf[0], buf[1], buf[2], buf[3]);
          buf += 4;

          ////////////////////////////////////////////////////
          // system units

          switch (type)
            {
            case ZMX_TYPE('U', 'N', 'I', 'T'): {

              char unit[6];

              if (sscanf(buf,
                         " %5s" // lens unit MM, CM, IN, METER
                         " %*s" // source prefix
                         " %*s" // source unit
                         " %*s" // analysis prefix
                         " %*s",        // analysis unit
                         unit) < 1)
                break;

              if (!strcasecmp(unit, "mm"))
                unit_factor = 1.0;
              else if (!strcasecmp(unit, "cm"))
                unit_factor = 10.0;
              else if (!strcasecmp(unit, "in"))
                unit_factor = 25.4;
              else if (!strcasecmp(unit, "meter"))
                unit_factor = 1000.0;
              else
                ZMX_WARN("unknown unit token");

              break;
            }

          ////////////////////////////////////////////////////
          // system temperature and pressure

            case ZMX_TYPE('E', 'N', 'V', 'D'): {
              double temp, pressure;

              if (sscanf(buf,
                         " %lf" // system global temperature
                         " %lf", // system relative pressure
                         &temp, &pressure) < 2)
                break;

              ref<material::AirKohlrausch68> env =
                ref<material::AirKohlrausch68>::create();

              env->set_temperature(temp);
              env->set_pressure(pressure * material::AirKohlrausch68::std_pressure);

              sys->set_environment(env);

              break;
            }


          ////////////////////////////////////////////////////
          // Load glass catalog

            case ZMX_TYPE('G', 'C', 'A', 'T'): {
              char catname[32];
              int pos;

              while (sscanf(buf, " %31s%n", catname, &pos) >= 1)
                {
                  import_catalog(catname);
                  buf += pos;
                }

              break;
            }

          ////////////////////////////////////////////////////
          // Surface data

            case ZMX_TYPE('S', 'U', 'R', 'F'): {

              unsigned int id = std::atoi(buf);

              if (id <= surf_array.size())
                surf_array.resize(id + 1);

              zemax_surface_s &surface = surf_array[id];

              surface = zemax_surface_s();

              while (std::getline(file, line))
                {
                  if (line.size() < 1 || line[0] != ' ')
                    goto surf_end;

                  if (line.size() < 6)
                    continue;

                  buf = line.c_str();
                  type = ZMX_TYPE(buf[2], buf[3], buf[4], buf[5]);
                  buf += 6;

                  switch (type)
                    {
                      ////////////////////////////////////////////////////
                      // Surface type
                    case ZMX_TYPE('S', 'T', 'O', 'P'): {
                        surface.stop = true;
                        break;
                    }
                    case ZMX_TYPE('T', 'Y', 'P', 'E'): {

                      if (surface.type != zs_none)
                        {
                          ZMX_WARN("surface type already defined");
                          break;
                        }

                      char typestr[9];

                      if (sscanf(buf, " %8s", typestr) < 1)
                        break;

                      if      (!strcasecmp(typestr, "standard"))
                        surface.type = zs_standard;
                      else if (!strcasecmp(typestr, "coordbrk"))
                        surface.type = zs_coordbrk;
                      else
                        ZMX_WARN("unknown surface type token");

                      break;
                    }

                    ////////////////////////////////////////////////////
                    // Surface curvature

                    case ZMX_TYPE('C', 'U', 'R', 'V'): {

                      switch (surface.type)
                        {
                        case zs_standard: {
                          double curv = std::atof(buf);
                          surface.roc = curv == 0.0 ? 0.0 : 1.0 / curv;
                          break;
                        }

                        default:
                          break;
                        }

                      break;
                    }

                    ////////////////////////////////////////////////////
                    // Surface conic constant

                    case ZMX_TYPE('C', 'O', 'N', 'I'): {

                      surface.coni = std::atof(buf);

                      break;
                    }

                    ////////////////////////////////////////////////////
                    // Surface param

                    case ZMX_TYPE('P', 'A', 'R', 'M'): {

                      unsigned int param_id;
                      double param_val;

                      sscanf(buf, " %u %lf", &param_id, &param_val);

                      if (param_id < 13)
                        surface.params[param_id] = param_val;

                      break;
                    }

                    ////////////////////////////////////////////////////
                    // Surface thickness

                    case ZMX_TYPE('D', 'I', 'S', 'Z'): {

                      if (id > 0 && buf[2] == 'I') // infinity
                        ZMX_WARN("infinit thickness surface");

                      surface.thick = std::atof(buf);

                      break;
                    }

                    ////////////////////////////////////////////////////
                    // Surface shape

                      case ZMX_TYPE('D', 'I', 'A', 'M'): {

                        double d = std::atof(buf);
                        if (surface.ap_type == za_none && d > 0.0)
                          {
                            surface.ap_params[0] = 0.0;
                            surface.ap_params[1] = d;
                            surface.ap_type = za_circular;
                          }

                        break;
                      }

                      ////////////////////////////////////////////////////
                      // Rectangular aperture

                      case ZMX_TYPE('S', 'Q', 'O', 'B'):
                        surface.ap_obscuration = true;

                      case ZMX_TYPE('S', 'Q', 'A', 'P'): {

                        surface.ap_type = za_rectangular;
                        sscanf(buf, " %lf %lf", &surface.ap_params[0], &surface.ap_params[1]);
                        break;
                      }

                      ////////////////////////////////////////////////////
                      // Circular aperture

                      case ZMX_TYPE('O', 'B', 'S', 'C'):
                        surface.ap_obscuration = true;

                      case ZMX_TYPE('F', 'L', 'A', 'P'):
                      case ZMX_TYPE('C', 'L', 'A', 'P'): {

                        surface.ap_type = za_circular;
                        sscanf(buf, " %lf %lf", &surface.ap_params[0], &surface.ap_params[1]);
                        break;
                      }

                      ////////////////////////////////////////////////////
                      // Elliptical aperture

                      case ZMX_TYPE('E', 'L', 'O', 'B'):
                        surface.ap_obscuration = true;

                      case ZMX_TYPE('E', 'L', 'A', 'P'): {

                        surface.ap_type = za_elliptical;
                        sscanf(buf, " %lf %lf", &surface.ap_params[0], &surface.ap_params[1]);
                        break;
                      }

                      ////////////////////////////////////////////////////
                      // Aperture decenter

                      case ZMX_TYPE('O', 'B', 'D', 'C'): {

                        surface.ap_decenter = true;
                        sscanf(buf, " %lf %lf", &surface.ap_params[2], &surface.ap_params[3]);
                        break;
                      }

                      ////////////////////////////////////////////////////
                      // Surface material

                      case ZMX_TYPE('G', 'L', 'A', 'S'): {
                        unsigned int gl_type;
                        int pos;

                        if (sscanf(buf, " %31s %u %u%n", surface.gl_name, &gl_type, &surface.gl_pickup, &pos) < 3)
                          break;

                        surface.gl_type = (enum zemax_glass_e)gl_type;
                        buf += pos;

                        switch (surface.gl_type)
                          {
                          case zg_model:
                            sscanf(buf, " %lf %lf %lf %*i %*i %*f %*f",
                                   &surface.gl_params[0],
                                   &surface.gl_params[1],
                                   &surface.gl_params[2]);
                            break;

                          case zg_offset:
                            sscanf(buf, " %*f %*f %*f %*i %*i %lf %lf",
                                   &surface.gl_params[0],
                                   &surface.gl_params[1]);
                            break;

                          case zg_fixed:
                            if (!strcasecmp(surface.gl_name, "mirror"))
                              surface.gl_type = zg_mirror;

                          default:
                            break;
                          }

                        break;
                      }

                      ////////////////////////////////////////////////////

                    } /* !switch */

                } /* !while */

              break;
            }

            ////////////////////////////////////////////////////

            } /* !switch */

        } /* !while */


#if 0
      // resolv "pickup" references

      for (unsigned int i = 0; i < surf_array.size(); i++)
        {
          zemax_surface_s &surface = surf_array[i];

          if (surface.gl_type == zg_pickup)
            {
              unsigned int id = surface.gl_pickup;

              if (id < surf_array.size())
                {
                  surface.gl_type = surf_array[id].gl_type;
                  strcpy(surface.gl_name, surf_array[id].gl_name);
                  surface.gl_pickup = surf_array[id].gl_pickup; // FIXME ugly surf copy
                  for (int j = 0; i < 3; i++)
                    surface.gl_params[j] = surf_array[id].gl_params[j];

                  i = 0;
                  continue;
                }
            }
        }
#endif

      math::Transform<3> coord;
      coord.reset();

      const_ref<material::Base> last_mat = sys->get_environment();

      for (unsigned int i = 1; i < surf_array.size(); i++)
        {
          zemax_surface_s &surf = surf_array[i];

          const_ref<curve::Base> curve;

          switch (surf.type)
            {
            case zs_coordbrk: {
              bool order = surf.params[6] != 0.0;
              // FIXME
              if (!order)
                coord.apply_translation(math::Vector3(surf.params[1], surf.params[2], 0.0) * unit_factor);
              coord.linear_rotation(-math::Vector3(surf.params[3], surf.params[4], surf.params[5]));
              if (order)
                coord.apply_translation(math::Vector3(surf.params[1], surf.params[2], 0.0) * unit_factor);
              continue;
            }

            case zs_none:
              ZMX_WARN("surface has unknown type");
              continue;

            case zs_standard:
              if (surf.roc == 0.0)
                curve = curve::flat;
              else if (surf.coni == 0.0)
                curve = GOPTICAL_REFNEW(curve::Sphere, unit_factor * surf.roc);
              else if (surf.coni == -1.0)
                curve = GOPTICAL_REFNEW(curve::Parabola, unit_factor * surf.roc);
              else
                curve = GOPTICAL_REFNEW(curve::Conic, unit_factor * surf.roc, surf.coni);
              break;
            }

          const_ref<shape::Base> shape = get_ap_shape(surf, unit_factor);

          ref<sys::Element> element;

          if (i == surf_array.size() - 1) {
              element = GOPTICAL_REFNEW(sys::Image, math::vector3_0, curve, shape);
          }
          else if (surf.stop == true) {
              ref<sys::Stop> s = GOPTICAL_REFNEW(sys::Stop, math::vector3_0, shape);
              
              element = s;
              std::cout << "@@@@@@" << std::endl;
          }
          else if (surf.gl_type == zg_air && surf_array[i-1].gl_type == zg_air) {
              ref<sys::Surface> s = GOPTICAL_REFNEW(sys::OpticalSurface, math::vector3_0, curve, shape,
                                                    material::none, material::none);
              
              s->set_enable_state(false);
              
              element = s;
          }
          else {
              const_ref<material::Base> mat = get_glass(*sys, surf);
              
              element = GOPTICAL_REFNEW(sys::OpticalSurface, math::vector3_0,
                                        curve, shape, last_mat, mat);
              
              if (surf.gl_type != zg_mirror)
                  last_mat = mat;
          }

          element->set_transform(coord);

          sys->add(*element);

          coord.apply_translation(math::Vector3(0.0, 0.0, unit_factor * surf.thick));
        }

      return sys;
    }


    ////////////////////////////////////////////////////////////////////////
    // Glass catalog import
    ////////////////////////////////////////////////////////////////////////

    ref<material::Catalog> ImportZemax::import_catalog(const std::string &name)
    {
      std::string filename(_cat_path);
      // FIXME ignore filename case
      filename += PATH_SEPARATOR;
      filename += name;
      filename += ".AGF";
      std::cerr << "Importing catalog " << name << std::endl;
      return import_catalog(filename, name);
    }

    ref<material::Catalog> ImportZemax::import_catalog_file(const std::string &filename)
    {
      std::string name(basename(filename));
      return import_catalog(filename, name);
    }

    ref<material::Catalog> ImportZemax::import_catalog(const std::string &filename,
                                                       const std::string &catname)
    {
      std::ifstream file(filename.c_str());
      std::string line;

      if (!file)
        throw Error("Unable to open file");

      ref<material::Catalog> cat = GOPTICAL_REFNEW(material::Catalog, catname);

      // FIXME check already loaded catalog

      ref<material::Dielectric> mat;

      while (std::getline(file, line))
        {
          int formula;

          if (line.size() < 2)
            continue;

          const char *buf = line.c_str();

#define AGF_TYPE(a, b) ((a) + ((b) << 8))

          int type = AGF_TYPE(buf[0], buf[1]);

          switch(type)
            {

              ////////////////////////////////////////////////////
              // New material line

            case (AGF_TYPE('N', 'M')): {
              char name[32];

              mat.invalidate();

              if (sscanf(buf, "NM %s %u", name, &formula) != 2)
                break;

              switch (formula)
                {
                case (1):       // Schott
                case (10):      // Extended
                case (12):      // Extended 2
                  mat = GOPTICAL_REFNEW(material::Schott);
                  break;

                case (3):       // Herzberger
                  mat = GOPTICAL_REFNEW(material::Herzberger);
                  break;

                case (5):       // Conrady
                  mat = GOPTICAL_REFNEW(material::Conrady);
                  break;

                case (7):       // Hanbook 1
                  mat = GOPTICAL_REFNEW(material::Handbook1);
                  break;

                case (8):       // Hanbook 2
                  mat = GOPTICAL_REFNEW(material::Handbook2);
                  break;

                case (4):       // Sellmeier 2
                  mat = GOPTICAL_REFNEW(material::SellmeierMod2);
                  break;

                case (2):       // Sellmeier 1
                case (6):       // Sellmeier 3
                case (9):       // Sellmeier 4
                case (11):      // Sellmeier 5
                  mat = GOPTICAL_REFNEW(material::Sellmeier);
                  break;
                }

              if (mat.valid())
                {
                  mat->set_measurement_medium(material::air);
                  cat->add_material(name, *mat);
                }

              break;
            }

              ////////////////////////////////////////////////////
              // Coefficient data line

            case (AGF_TYPE('C', 'D')): {
              double c0, c1, c2, c3, c4, c5, c6, c7, c8, c9;

              if (!mat.valid())
                break;

              if (sscanf(buf, "CD %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                         &c0, &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8, &c9) < 3)
                break;

              switch (formula)
                {
                case (1): {     // Schott
                  material::Schott &m = static_cast<material::Schott &>(*mat);

                  m.set_terms_range(-8, 2);
                  m.set_term(0, c0);
                  m.set_term(2, c1);
                  m.set_term(-2, c2);
                  m.set_term(-4, c3);
                  m.set_term(-6, c4);
                  m.set_term(-8, c5);
                  break;
                }

                case (2): {     // Sellmeier 1
                  material::Sellmeier &m = static_cast<material::Sellmeier &>(*mat);

                  m.set_terms_count(3);
                  m.set_term(0, c0, c1);
                  m.set_term(1, c2, c3);
                  m.set_term(2, c4, c5);
                  m.set_contant_term(1.0);
                  break;
                }

                case (3): {     // Herzberger
                  material::Herzberger &m = static_cast<material::Herzberger &>(*mat);

                  m.set_coefficients(c0, c3, c4, c5, c1, c2);
                  break;
                }

                case (4): {     // Sellmeier 2
                  material::SellmeierMod2 &m = static_cast<material::SellmeierMod2 &>(*mat);
                  m.set_coefficients(c0, c1, c2, c3, c4);
                  break;
                }

                case (5): {     // Conrady
                  material::Conrady &m = static_cast<material::Conrady &>(*mat);

                  m.set_coefficients(c0, c1, c2);
                  break;
                }
                case (6): {     // Sellmeier 3
                  material::Sellmeier &m = static_cast<material::Sellmeier &>(*mat);

                  m.set_terms_count(4);
                  m.set_term(0, c0, c1);
                  m.set_term(1, c2, c3);
                  m.set_term(2, c4, c5);
                  m.set_term(3, c6, c7);
                  m.set_contant_term(1.0);
                  break;
                }

                case (7): {     // Hanbook 1
                  material::Handbook1 &m = static_cast<material::Handbook1 &>(*mat);
                  m.set_coefficients(c0, -c3, c1, c2);
                  break;
                }
                case (8): {     // Hanbook 2
                  material::Handbook2 &m = static_cast<material::Handbook2 &>(*mat);
                  m.set_coefficients(c0, -c3, c1, c2);
                  break;
                }

                case (9): {     // Sellmeier 4
                  material::Sellmeier &m = static_cast<material::Sellmeier &>(*mat);

                  m.set_terms_count(2);
                  m.set_term(0, c1, c2);
                  m.set_term(1, c3, c4);
                  m.set_contant_term(c0);
                  break;
                }

                case (10): {    // Extended
                  material::Schott &m = static_cast<material::Schott &>(*mat);

                  m.set_terms_range(-12, 2);
                  m.set_term(0, c0);
                  m.set_term(2, c1);
                  m.set_term(-2, c2);
                  m.set_term(-4, c3);
                  m.set_term(-6, c4);
                  m.set_term(-8, c5);
                  m.set_term(-10, c6);
                  m.set_term(-12, c7);
                  break;
                }

                case (11): {    // Sellmeier 5
                  material::Sellmeier &m = static_cast<material::Sellmeier &>(*mat);

                  m.set_terms_count(5);
                  m.set_term(0, c0, c1);
                  m.set_term(1, c2, c3);
                  m.set_term(2, c4, c5);
                  m.set_term(3, c6, c7);
                  m.set_term(4, c8, c9);
                  m.set_contant_term(1.0);
                  break;
                }

                case (12): {    // Extended 2
                  material::Schott &m = static_cast<material::Schott &>(*mat);

                  m.set_terms_range(-8, 6);
                  m.set_term(0, c0);
                  m.set_term(2, c1);
                  m.set_term(-2, c2);
                  m.set_term(-4, c3);
                  m.set_term(-6, c4);
                  m.set_term(-8, c5);
                  m.set_term(4, c6);
                  m.set_term(6, c7);
                  break;
                }

                }

              break;
            }

              ////////////////////////////////////////////////////
              // Thermal data line

            case (AGF_TYPE('T', 'D')): {
              double d0, d1, d2, e0, e1, ltk, reftemp;

              if (!mat.valid())
                break;

              if (sscanf(buf, "TD %lf %lf %lf %lf %lf %lf %lf",
                         &d0, &d1, &d2, &e0, &e1, &ltk, &reftemp) != 7)
                break;

              mat->set_temperature_schott(d0, d1, d2, e0, e1, ltk * 1000.);

              // Zemax glasses are measured in air medium
              ref<material::AirKohlrausch68> air =
                ref<material::AirKohlrausch68>::create();

              air->set_temperature(reftemp);
              mat->set_measurement_medium(*air);

              break;
            }

              ////////////////////////////////////////////////////
              // Internal Transmition line

            case (AGF_TYPE('I', 'T')): {
              double wl, it, th;

              if (!mat.valid())
                break;

              if (sscanf(buf, "IT %lf %lf %lf", &wl, &it, &th) != 3)
                break;

              mat->set_internal_transmittance(wl * 1000.0, th, it);
              break;
            }

              ////////////////////////////////////////////////////
              // Extra data line

            case (AGF_TYPE('E', 'D')): {
              double tce, density;

              if (!mat.valid())
                break;

              if (sscanf(buf, "ED %lf %*f %lf", &tce, &density) != 2)
                break;

              mat->set_thermal_expansion(tce * 1e-6);
              mat->set_density(density);
              break;
            }

              ////////////////////////////////////////////////////
              // Limit data line

            case (AGF_TYPE('L', 'D')): {
              double low, high;

              if (!mat.valid())
                break;

              if (sscanf(buf, "LD %lf %lf", &low, &high) != 2)
                break;

              mat->set_wavelen_range(low * 1000.0, high * 1000.0);
              break;
            }

            }

        }

      _cat_list.insert(cat_map_t::value_type(catname, cat));

      return cat;
    }

    ref<material::Catalog> ImportZemax::get_catalog(const std::string &catalogname)
    {
      cat_map_t::iterator i = _cat_list.find(catalogname);

      if (i == _cat_list.end())
        throw Error("no such catalog loaded");

      return i->second;
    }


    ////////////////////////////////////////////////////////////////////////
    // Table glass import
    ////////////////////////////////////////////////////////////////////////

    ref<material::Dielectric> ImportZemax::import_table_glass(const std::string &filename)
    {
      std::ifstream file(filename.c_str());
      std::string line;

      if (!file)
        throw Error("Unable to open file");

      ref<material::DispersionTable> mat = GOPTICAL_REFNEW(material::DispersionTable);

      while (std::getline(file, line))
        {
          double wl, index, trans, thick;
          const char *buf = line.c_str();

          switch (sscanf(buf, "%lf %lf %lf %lf", &wl, &index, &trans, &thick))
            {
            case 0: {
              double density;

              if (sscanf(buf, "DENSITY %lf", &density) == 1)
                mat->set_density(density);
              break;
            }

            case 4:
              mat->set_internal_transmittance(wl * 1000.0, thick, trans);
            case 2:
            case 3:
              mat->set_refractive_index(wl * 1000.0, index);
            default:
              break;
            }
        }

      return mat;
    }

  }

}

