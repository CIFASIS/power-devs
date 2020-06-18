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

#include <QGraphicsPixmapItem>
#include <QList>
#include <QPainter>

#include <graphics/gpx_connection_outport.h>
#include <graphics/gpx_edge.h>

GpxConnectionOutport:: GpxConnectionOutport(QGraphicsItem *parent, Graphic::Direction direction): GpxConnectionPoint(parent,direction)
{
	_myPolygon 
    << QPointF(-4, 4) 
    << QPointF(0, 0) 
    << QPointF(-4, -4)
	<< QPointF(-4, 0) 
    << QPointF(-6, 0) 
    << QPoint(-4, 0);
  	setCursor(QCursor(Qt::CrossCursor));
} 

QRectF GpxConnectionOutport::boundingRect() const
{
  return _myPolygon.boundingRect().adjusted(-3,-3,3,3);
}

QPointF GpxConnectionOutport::getSceneConnectionPoint()
{
  return mapToScene(getConnectionPoint());
}

QVariant GpxConnectionOutport::itemChange(GraphicsItemChange change, const QVariant &value)
{
  GpxConnectionPoint::itemChange(change, value);
  if (change == QGraphicsItem::ItemPositionHasChanged) 
  	updatePosition();
  return value;
}

QPointF GpxConnectionOutport::getConnectionPoint()
{
  return _myPolygon.at(1);
}

void GpxConnectionOutport::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *)
{
 painter-> setPen(QPen (Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
 painter->setBrush(Qt::black);
 if (_edges.empty()) {
  painter->drawLine(_myPolygon.at(0), _myPolygon.at(1));
  painter->drawLine(_myPolygon.at(1), _myPolygon.at(2));
  } else {
	painter->drawLine(QPointF(_myPolygon.at(0).x(), 0), _myPolygon.at(1));
  }
}

void GpxConnectionOutport::updatePosition()
{
	prepareGeometryChange();
	foreach(GpxEdge * edge, _edges)
		edge->updatePosition();
}


