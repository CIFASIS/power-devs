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


#ifndef GPXCONNECTIONPOINT_H
#define GPXCONNECTIONPOINT_H

#include <QGraphicsPixmapItem>
#include <QCursor>
#include <QList>
#include <QGraphicsWidget>
#include <data/graphic.h>
#include <graphics/gpx_edit.h>

class GpxEdge;
class QGraphicsItem;

#define GPXCONNECTIONPOINT_TYPE 20

class GpxConnectionPoint:public QGraphicsItem {
public:
  	GpxConnectionPoint(QGraphicsItem *p, Graphic::Direction direction);
  	~GpxConnectionPoint();
	/*!< Enum value. */
	enum { Type = UserType + GPXCONNECTIONPOINT_TYPE };
	/*!  This enumeration defines the type of port. */
  	int type() const { return Type; }
  	virtual bool isInport() { return false; }
  	virtual bool isOutport() { return false; }
  	virtual bool isNode() { return false; }
  	virtual bool isCross() { return false; }
  	Graphic::Direction direction() const { return _direction; }
  	void setDirection(Graphic::Direction direction) { _direction = direction; }
  	virtual QPointF getConnectionPoint() = 0;
  	virtual QPointF getSceneConnectionPoint() = 0;
  	void addEdge(GpxEdge *edge);
  	virtual void removeEdge(GpxEdge *edge);
  	GpxEdge *edgeAt(int i) { return _edges.at(i) ; }
  	unsigned edgeCount() { return _edges.size(); }
    bool isConnected() { return edgeCount()>0; }
  	void clearEdges() { _edges.clear(); }
	QString typeName();
	QVariant itemChange( GraphicsItemChange change, const QVariant & value );
protected:
  	Graphic::Direction _direction;
  	QList < GpxEdge* > _edges;
  	QPolygonF _myPolygon;
};
#endif
