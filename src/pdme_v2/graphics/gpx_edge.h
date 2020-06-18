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

#ifndef GPXEDGE_H
#define GPXEDGE_H

#include <QDebug>
#include <QGraphicsItem>

#include <graphics/gpx_connection_point.h>
#include <graphics/gpx_temp_connection.h>
#include <graphics/gpx_edit.h>
#include <data/coupled.h>

class GpxEdge:public QGraphicsItem {
 public:
	GpxEdge(GpxConnectionPoint * sourcePort, GpxConnectionPoint * destPort, Line *p=NULL);
	~GpxEdge();
	GpxConnectionPoint *sourcePort() const;
	void setSourcePort(GpxConnectionPoint * port);
	GpxConnectionPoint *destPort() const;
	void setDestPort(GpxConnectionPoint * port);
	void updatePosition(bool adjust=true);
	void addPoint(QPointF point, bool updatePos=true);
	QPointF breakAt(QPointF point);
	void preAddPoint(QPointF point);
	enum { Type = UserType + 2 };	/*!< Enum value. */
  	void simplifyEdge();
	QVariant itemChange(GraphicsItemChange change, const QVariant & value);
	int type() const { return Type; }
  	void mergeWith(GpxEdge *e, QPointF m);
	QPolygonF polygon() const { return _myPolygon; }
	void setPolygon(QPolygonF polygon);
  	Line *dataLine() { return _lineData; }
	bool overVerticalSegemt(QPointF);
	void moveTo(QPointF p);
	void startMoving(QPointF p);
	void stopMoving();
	QRectF boundingRect()const;
  	void addPort(GpxConnectionPoint *);
  	void addPorts(QList<GpxConnectionPoint *>);
  	bool isConnectedToPort(GpxConnectionPoint *p) { qDebug() << _connected; return _connected.indexOf(p)!=-1; }
  	QList<GpxConnectionPoint*> ports() { return _connected; }
	void adjustPolygon();
	int segmentDirection(QPointF, QPointF);
	void updateSegmentsPositions(bool);
  	GpxEdit *edit() { return _edit; };
  	Graphic::Direction findDirection(const QPointF middlePoint, const QPointF endPoint);
 protected:
  	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
	QPainterPath shape() const;
	void mousePressEvent(QGraphicsSceneMouseEvent *);
  	void hoverMoveEvent ( QGraphicsSceneHoverEvent *);
 private:
	GpxConnectionPoint *_source, *_dest;
	QPolygonF _myPolygon;	
	QPointF _sourcePoint;
	QPointF _destPoint;
	Line *_lineData;
	int _movingSegment;
  QList<GpxConnectionPoint*> _connected; 
  EditEdge *_edit;
	bool _movingEdge;
	bool _movingDest;
	bool _movingSource;
};
#endif
