/****************************************************************************
**
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**     Universidad Nacional de Rosario - Argentina.
**  Contact: PowerDEVS Information (kofman@fceia.unr.edu.ar, fbergero@fceia.unr.edu.ar)
**
**  This file is part of PowerDEVS.
**
**  PowerDEVS is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  PowerDEVS is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with PowerDEVS.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include <QPainter>
#include <QDebug>

#include <data/graphic.h>
#include <graphics/gpx_connection_inport.h>
#include <graphics/gpx_edge.h>

GpxConnectionInport::GpxConnectionInport(QGraphicsItem *parent, Graphic::Direction direction):
  GpxConnectionPoint(parent,direction)
{
	_myPolygon 
    << QPointF(-4, 4) 
    << QPointF(0, 0) 
    << QPointF(-4, -4) 
    << QPointF(-4, 0) 
	#ifdef INPORTSIZE
    	<< QPointF(-10, 0) 
	#else
    	<< QPointF(-6, 0)
	#endif
    << QPoint(-4, 0);
  setCursor(Qt::CrossCursor);
}  

QRectF GpxConnectionInport::boundingRect() const
{
  return _myPolygon.boundingRect().adjusted(-3,-3,3,3);
}

QPointF GpxConnectionInport::getSceneConnectionPoint()
{
 return  mapToScene(getConnectionPoint());
}  

QPointF GpxConnectionInport::getConnectionPoint()
{
  return _myPolygon.at(4);
}

QVariant GpxConnectionInport::itemChange(GraphicsItemChange change, const QVariant &value)
{
  GpxConnectionPoint::itemChange(change, value);
  if (change == QGraphicsItem::ItemPositionHasChanged) 
  	updatePosition();
  return value;
}

void GpxConnectionInport::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *)
{
  painter-> setPen(QPen (Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->setBrush(Qt::black);
	if (_edges.empty()) {
		painter->drawLine(_myPolygon.at(0), _myPolygon.at(1));
		painter->drawLine(_myPolygon.at(1), _myPolygon.at(2));
  } else {
    painter->drawPolygon(_myPolygon);
  }
}

void GpxConnectionInport::updatePosition()
{
	prepareGeometryChange();
	foreach(GpxEdge * edge, _edges)
		edge->updatePosition();
}
