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

#include <QtGui>

#include "data/graphic.h"
#include "gpx_temp_connection.h"

GpxTempConnection::GpxTempConnection(QPointF initPoint, QPointF endPoint,
			     GpxConnectionPoint * p, bool c_dir, QGraphicsItem * parent,
			     QGraphicsScene * scene)
 : QGraphicsPathItem(parent), change_dir(c_dir)
{
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	myColor = Qt::black;
	setPen(QPen(myColor, 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
	myInitPoint = initPoint;
	myEndPoint = endPoint;
	myInitPort = p;
	updatePosition();
}

QRectF GpxTempConnection::boundingRect() const 
{
	qreal extra = (pen().width() + 80) / 2.0;

	return QRectF(myInitPoint, QSizeF(myEndPoint.x() - myInitPoint.x(),
					  myEndPoint.y() - myInitPoint.y()))
	    .normalized()
	    .adjusted(-extra, -extra, extra, extra);
}

QPainterPath GpxTempConnection::shape() const
{
	QPainterPath path = QGraphicsPathItem::shape();
	return path;
}

void GpxTempConnection::updatePosition()
{
	QPointF middlePoint;
	if (!change_dir) {
		switch (myInitPort->direction()) {
		case Graphic::RIGHT:
		case Graphic::LEFT:
			middlePoint.setX(myEndPoint.x());
			middlePoint.setY(myInitPoint.y());
			break;
		case Graphic::UP:
		case Graphic::DOWN:
			middlePoint.setX(myInitPoint.x());
			middlePoint.setY(myEndPoint.y());
			break;
		default:
			break;
		}
	} else {
		switch (myInitPort->direction()) {
		case Graphic::UP:
		case Graphic::DOWN:
			middlePoint.setX(myEndPoint.x());
			middlePoint.setY(myInitPoint.y());
			break;
		case Graphic::RIGHT:
		case Graphic::LEFT:
			middlePoint.setX(myInitPoint.x());
			middlePoint.setY(myEndPoint.y());
			break;
		default:
			break;
		}
	}
	//     qDebug("Direccion %d\n", int(myInitGpxTempConnectionPoint->direction()));

	myPolygon.clear();
	myPolygon << myInitPoint << middlePoint << myEndPoint;
	myPath.addPolygon(myPolygon);
	setPath(myPath);
	update();
}

QPointF GpxTempConnection::getInitPoint() const
{
	return myInitPoint;
}

QPointF GpxTempConnection::getEndPoint() const
{
	return myEndPoint;
}

QPointF GpxTempConnection::getMiddlePoint() const
{
	QPointF middlePoint;
	switch (myInitPort->direction()) {
	case Graphic::RIGHT:
	case Graphic::LEFT:
		middlePoint.setX(myEndPoint.x());
		middlePoint.setY(myInitPoint.y());
		break;
	case Graphic::UP:
	case Graphic::DOWN:
		middlePoint.setX(myInitPoint.x());
		middlePoint.setY(myEndPoint.y());
		break;
	default:
		break;
	}
	return middlePoint;
}

void GpxTempConnection::setEndPoint(QPointF p) 
{
    myEndPoint = p;
    updatePosition();
}
