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

#ifndef GOPTICAL_RENDERER_DXF_HH_
#define GOPTICAL_RENDERER_DXF_HH_

#include <dime/Model.h>
#include <dime/sections/EntitiesSection.h>
#include <dime/sections/TablesSection.h>
#include <dime/sections/BlocksSection.h>
#include <dime/tables/LayerTable.h>
#include <dime/tables/Table.h>
#include <dime/entities/Block.h>
#include <dime/entities/Insert.h>
#include <dime/util/Linear.h>

#include "goptical/core/common.hpp"

#include "goptical/core/io/renderer.hpp"

namespace _goptical {

  namespace io {

    /**
       @short DXF rendering driver base
       @header <goptical/core/io/RendererDxf
       @module {Core}
       @main
       @experimental

       This class implements a 2d and 3d Dxf CAD format output
       driver. It requires the @url {http://www.coin3d.org/lib/dime}
       {Dime} library to compile.
     */
    class RendererDxf : public Renderer
    {
    public:
      /** Create a DXF renderer */
      RendererDxf();

      /** Create a DXF renderer, output will be written to given
          filename when the renderer is destroyed. */
      RendererDxf(const char *filename);

      ~RendererDxf();

      /** Add a new layer in dxf file and return its index */
      unsigned int add_layer(std::string name);
      /** Set current layer */
      inline void use_layer(unsigned int id);

      /** Save output to a DXF file */
      void write(const std::string & filename);

    private:
      void init();

      /** @override */
      void draw_point(const math::Vector2 &p, const Rgb &rgb, enum PointStyle s);
      /** @override */
      void draw_segment(const math::VectorPair2 &l, const Rgb &rgb);
      /** @override */
      void draw_circle(const math::Vector2 &c, double r, const Rgb &rgb, bool filled);
      /** @override */
      void draw_text(const math::Vector2 &pos, const math::Vector2 &dir,
                     const std::string &str, TextAlignMask a, int size, const Rgb &rgb);
      /** @override */
      void draw_text(const math::Vector3 &pos, const math::Vector3 &dir,
                     const std::string &str, TextAlignMask a, int size, const Rgb &rgb);
      /** @override */
      void draw_point(const math::Vector3 &p, const Rgb &rgb, enum PointStyle s);
      /** @override */
      void draw_segment(const math::VectorPair3 &l, const Rgb &rgb);

      /** @override */
      void group_begin(const std::string &name);
      /** @override */
      void group_end();

      void add_entity(dimeEntity *entity);

      const char *_filename;
      dimeLayerTable *current_layer();

      inline dimeVec3f vec3fconv(const math::Vector3 &v);
      inline dimeVec3f vec3fconv(const math::Vector2 &v);

      unsigned int _group_level;

      dimeModel _model;
      dimeTablesSection *_table_section;
      dimeTable *_layers_table;
      dimeEntitiesSection *_entities_section;
      dimeBlocksSection *_blocks_section;

      dimeBlock *_current_block;
      unsigned int _layer_id;
    };
  }
}

#endif

