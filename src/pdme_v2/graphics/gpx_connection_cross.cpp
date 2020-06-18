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


#include <QCursor>
#include <QList>
#include <QPainter>

#include <graphics/gpx_connection_cross.h>
#include <graphics/gpx_edge.h>

GpxConnectionCross::GpxConnectionCross(Graphic::Direction direction): GpxConnectionPoint(NULL,direction)
{
	_myPolygon 
    << QPoint(-6, 6) 
    << QPoint(6, 6)
    << QPoint(6, -6) 
    << QPoint(-6, -6) 
    << QPoint(-6, 6);
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setCursor(QCursor(Qt::CrossCursor));
}

QRectF GpxConnectionCross::boundingRect() const
{
  return _myPolygon.boundingRect();
}

QPointF GpxConnectionCross::getSceneConnectionPoint()
{
  return mapToScene(getConnectionPoint());
}

QPointF GpxConnectionCross::getConnectionPoint()
{
  return QPointF(0,0);
}

void GpxConnectionCross::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *)
{
		painter-> setPen(QPen (Qt::black, 2.6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		painter->drawLine(QPointF(0, 6), QPointF(0, -6));
		painter->drawLine(QPointF(6, 0), QPointF(-6, 0));
    if (isSelected()) {
  		painter-> setPen(QPen (Qt::red, 1.6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	  	painter->drawLine(QPointF(0, 3), QPointF(0, -3));
		  painter->drawLine(QPointF(3, 0), QPointF(-3, 0));
    }
}

QPainterPath GpxConnectionCross::shape() const
{
  QPainterPath path;
  path.addPolygon(_myPolygon);
  return path;
}

void GpxConnectionCross::updatePosition()
{
	prepareGeometryChange();
	foreach(GpxEdge *edge, _edges)
		edge->updatePosition();
}

QVariant GpxConnectionCross::itemChange(GraphicsItemChange change, const QVariant & value)
{
  GpxConnectionPoint::itemChange(change, value);
  if (change == QGraphicsItem::ItemPositionHasChanged) 
  	updatePosition();
  return value;
}
