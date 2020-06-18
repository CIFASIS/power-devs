/***************************************************************************g
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
#include <QDebug>
#include <QGraphicsScene>
#include <QList>
#include <QPainter>

#include <graphics/gpx_connection_node.h>
#include <graphics/gpx_edge.h>
#include <graphics/gpx_edition_scene.h>
#include <data/coupled.h>

GpxConnectionNode::GpxConnectionNode(QGraphicsScene *s, Point *p): GpxConnectionPoint(NULL,Graphic::LEFT), _dataPoint(p)
{
  	_myPolygon 
    << QPoint(-3, 3) 
    << QPoint(3, 3) 
    << QPoint(3, -3)
    << QPoint(-3, -3) 
    << QPoint(-3, 3);
  	setFlag(QGraphicsItem::ItemIsSelectable, true);
  	setFlag(QGraphicsItem::ItemIsMovable, true);
}

QRectF GpxConnectionNode::boundingRect() const
{
  return _myPolygon.boundingRect();
}

QPointF GpxConnectionNode::getSceneConnectionPoint()
{
  return mapToScene(getConnectionPoint());
}

QPointF GpxConnectionNode::getConnectionPoint()
{
  return QPointF(0,0);
}

void GpxConnectionNode::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	  painter->setPen(QPen (Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	  painter->setBrush(Qt::black);
	  painter->drawEllipse(_myPolygon.boundingRect());
}

QPainterPath GpxConnectionNode::shape() const
{
  QPainterPath path;
  path.addPolygon(_myPolygon);
  return path;
}

void GpxConnectionNode::updatePosition()
{
	prepareGeometryChange();
	foreach(GpxEdge * edge, _edges)
		edge->updatePosition();
	_dataPoint->graphic().setX(x());
  	_dataPoint->graphic().setY(y());
}

void GpxConnectionNode::setPos(int x, int y)
{
	_dataPoint->graphic().setX(x);
  	_dataPoint->graphic().setY(y);
	QGraphicsItem::setPos(QPointF(x,y));
}

QVariant GpxConnectionNode::itemChange(GraphicsItemChange change, const QVariant & value)
{
  GpxConnectionPoint::itemChange(change, value);
  if (change == QGraphicsItem::ItemPositionHasChanged) 
	  updatePosition();
  return value;
}
