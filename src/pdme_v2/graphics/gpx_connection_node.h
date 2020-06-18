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


#ifndef GPXCONNECTIONNODE_H
#define GPXCONNECTIONNODE_H

#include <QCursor>
#include <QList>

#include <data/graphic.h>
#include <graphics/gpx_connection_point.h>
#include <data/coupled.h>


class GpxConnectionNode: public GpxConnectionPoint
{
public:
	GpxConnectionNode(QGraphicsScene *scene=NULL, Point *p=NULL);
  virtual bool isNode() { return true; }
	void updatePosition();
  QRectF boundingRect() const;
  QPainterPath shape() const;
  QPointF getSceneConnectionPoint();
  QPointF getConnectionPoint();
	void setPos(int x, int y);
  void paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *);
	Point *dataPoint() { return _dataPoint; };
  QVariant itemChange(GraphicsItemChange change, const QVariant & value);
private:
	Point *_dataPoint;
};

#endif
