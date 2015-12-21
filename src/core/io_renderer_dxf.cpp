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

#include <dime/entities/Line.h>
#include <dime/entities/Point.h>
#include <dime/entities/Circle.h>
#include <dime/entities/Polyline.h>
#include <dime/entities/Vertex.h>
#include <dime/Output.h>

#include <goptical/core/io/RendererDxf>
#include <goptical/core/sys/System>

namespace _goptical {

  namespace io {

    void RendererDxf::init()
    {
      // define layers
      _table_section = new dimeTablesSection();
      _model.insertSection(_table_section);

      _layers_table = new dimeTable(0);
      _table_section->insertTable(_layers_table);

      _blocks_section = new dimeBlocksSection();
      _model.insertSection(_blocks_section);

      _entities_section = new dimeEntitiesSection();
      _model.insertSection(_entities_section);

      //      add_layer("0");
    }

    RendererDxf::RendererDxf(const char *filename)
      : _filename(filename),
        _group_level(0),
        _model(),
        _current_block(0),
        _layer_id(0)
    {
      init();
    }

    RendererDxf::RendererDxf()
      : _filename(0),
        _group_level(0),
        _model(),
        _current_block(0),
        _layer_id(0)
    {
      init();
    }

    RendererDxf::~RendererDxf()
    {
      if (_filename)
        write(_filename);

#if 0
      // FIXME figure out which dime objects must be deleted
      delete _entities_section;
      delete _blocks_section;
      delete _layers_table;
      delete _table_section;
#endif
    }

    unsigned int RendererDxf::add_layer(std::string name)
    {
      dimeLayerTable *layer = new dimeLayerTable;

      layer->setLayerName(name.c_str(), 0);
      layer->setColorNumber(0);

      // need to set some extra records so that AutoCAD will stop
      // complaining (see dime library sphere example)
      dimeParam param;
      param.string_data = "CONTINUOUS";
      layer->setRecord(6, param);
      param.int16_data = 64;
      layer->setRecord(70, param);

      layer->registerLayer(&_model);

      return _model.getNumLayers() - 1;
    }

    void RendererDxf::add_entity(dimeEntity *entity)
    {
      entity->setLayer(_model.getLayer(_layer_id));

      if (_current_block)
        _current_block->insertEntity(entity);
      else
        _model.addEntity(entity);
    }

    void RendererDxf::write(const std::string & filename)
    {
      dimeOutput out;

      out.setFilename(filename.c_str());
      _model.write(&out);

      _filename = 0;
    }

    void RendererDxf::group_begin(const std::string &name)
    {
      if (_group_level++)
        return;

      _current_block = new dimeBlock(0);
      _current_block->setName(name.c_str());
      _blocks_section->insertBlock(_current_block);      
    }

    void RendererDxf::group_end()
    {
      if (--_group_level)
        return;

      dimeInsert *insert = new dimeInsert();
      insert->setBlock(_current_block);
      _model.addEntity(insert);

      _current_block = 0;
    }

    void RendererDxf::draw_point(const math::Vector2 &p, const Rgb &rgb, enum PointStyle s)
    {
      dimePoint *point = new dimePoint;

      point->setCoords(vec3fconv(p));
      add_entity(point);
    }

    void RendererDxf::draw_segment(const math::VectorPair2 &l, const Rgb &rgb)
    {
      dimeLine *line = new dimeLine;

      line->setCoords(0, vec3fconv(l[0]));
      line->setCoords(1, vec3fconv(l[1]));
      add_entity(line);
    }

#if 0
    void RendererDxf::draw_polygon(const math::Vector2 *array, unsigned int count,
                                   const Rgb &rgb, bool filled, bool closed)
    {
      if (count < 3)
        return;

      dimePolyline *pline = new dimePolyline;
      dimeVertex *vtxct[count];

      for (unsigned int i = 0; i < count; i++)
        {
          dimeVertex *v = new dimeVertex;
          v->setCoords(vec3fconv(array[i]));
          v->setFlags(dimeVertex::POLYLINE_3D_VERTEX);
          vtxct[i] = v;
        }

      pline->setSurfaceType(dimePolyline::POLYLINE);
      pline->setFlags(dimePolyline::IS_POLYLINE_3D);
      pline->setCoordVertices(vtxct, count);

      for (unsigned int i = 0; i < count - 1; i++)
        {
          dimeVertex *v = new dimeVertex;
          v->setIndex(0, i);
          v->setIndex(1, i+1);
          v->setFlags(dimeVertex::POLYLINE_3D_VERTEX);
          vtxct[i] = v;
        }

      pline->setIndexVertices(vtxct, count - 1);

      add_entity(pline);
    }
#endif

    void RendererDxf::draw_circle(const math::Vector2 &c, double r,
                                  const Rgb &rgb, bool filled)
    {
      dimeCircle *circle = new dimeCircle;

      circle->setCenter(vec3fconv(c));
      circle->setRadius(r);

      add_entity(circle);
    }

    void RendererDxf::draw_text(const math::Vector2 &pos, const math::Vector2 &dir,
                                const std::string &str, TextAlignMask a, int size, const Rgb &rgb)
    {
      // FIXME
    }

    void RendererDxf::draw_text(const math::Vector3 &pos, const math::Vector3 &dir,
                                const std::string &str, TextAlignMask a, int size, const Rgb &rgb)
    {
      // FIXME
    }

    void RendererDxf::draw_point(const math::Vector3 &p, const Rgb &rgb, enum PointStyle s)
    {
      dimePoint *point = new dimePoint;

      point->setCoords(vec3fconv(p));
      add_entity(point);
    }

    void RendererDxf::draw_segment(const math::VectorPair3 &l, const Rgb &rgb)
    {
      dimeLine *line = new dimeLine;

      line->setCoords(0, vec3fconv(l[0]));
      line->setCoords(1, vec3fconv(l[1]));

      add_entity(line);
    }

  }
}

