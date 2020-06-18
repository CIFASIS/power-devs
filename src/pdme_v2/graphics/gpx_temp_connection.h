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

#ifndef GPXCONNECTION_H
#define GPXCONNECTION_H

#include <QGraphicsPolygonItem>

#include "gpx_connection_point.h"
#define GPXCONNECTION_TYPE 4
class QGraphicsPathItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QPolygonF;
class QPainterPath;

class GpxTempConnection:public QGraphicsPathItem {
 public:
	enum { Type = UserType + GPXCONNECTION_TYPE };

	 GpxTempConnection(QPointF initPoint, QPointF endPoint,
		       GpxConnectionPoint * p = 0, bool change_direction=false, QGraphicsItem * parent =
		       0, QGraphicsScene * scene = 0);

	int type() const { return Type; }
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void setColor(const QColor & color) { myColor = color; } 
	QPointF getInitPoint() const;
	QPointF getEndPoint() const;
  	void setEndPoint(QPointF);
	QPointF getMiddlePoint() const;
	GpxConnectionPoint *getInitPort() { return myInitPort; };

public slots:
	void updatePosition();

private:
	QPointF myInitPoint;
	QPointF myEndPoint;
	QColor myColor;
	QPolygonF myPolygon;
	QPainterPath myPath;
	GpxConnectionPoint *myInitPort;
	bool change_dir;
};
#endif
