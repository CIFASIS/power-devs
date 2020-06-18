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

#include <math.h>

#include <QDebug>
#include <QTextDocument>
#include <QApplication>
#include <QDesktopWidget>
#include <QTextBlockFormat>
#include <QTextCursor>

#include <data/graphic.h>
#include <graphics/gpx_block.h>
#include <graphics/gpx_connection_inport.h>
#include <graphics/gpx_edge.h>
#include <graphics/gpx_edition_scene.h>

#define PORTS_SIZE	10
#define MIN_WIDTH	10
#define MIN_HEIGHT	10

GpxBlock::GpxBlock(): _name_item(this)
{
  setFlag(QGraphicsItem::ItemIsMovable,true);
  setFlag(QGraphicsItem::ItemIsSelectable,true);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
  setCursor(QCursor(Qt::ArrowCursor));
  _name_item.setFlag(QGraphicsItem::ItemIgnoresTransformations);
  _offset = QApplication::desktop()->width() * 0.005;
  setAcceptHoverEvents(true);
}

QRectF GpxBlock::boundingRect() const 
{
	QRectF br = _myPolygon.boundingRect();
	return br.adjusted(-10 + (_myDirection == 3 ? -15 : 0),
			   -10 + (_myDirection == 2 ? -15 : 0),
			   10 + (_myDirection == 1 ? 15 : 0),
			   10 + (_myDirection == 0 ? 15 : 0));
}

void GpxBlock::setName(QString name)
{
	_name_item.setPlainText(name);
	_name_item.adjustSize();
  GpxEditionScene *ges = dynamic_cast<GpxEditionScene*>(scene());
  if (ges)
    ges->on_modified();
	updateTextPosition();
}

void GpxBlock::updateTextPosition()
{
	QPointF pos;
  	//_name_item.setTextWidth(_name_item.boundingRect().width());
    QTextCursor cursor = _name_item.textCursor();
  	QTextBlockFormat bfmt = cursor.blockFormat();
 	bfmt.setAlignment(Qt::AlignCenter);
  	cursor.setBlockFormat(bfmt);
  	_name_item.setTextCursor(cursor); 
  	_name_item.setTextWidth(_width+20);

  switch (direction())
  {
    case Graphic::RIGHT:
      pos.setX(-(_name_item.boundingRect().width()/2));
      pos.setY(_height / 2 + 3);
      break;
    case Graphic::LEFT:
      pos.setX((_name_item.boundingRect().width() / 2));
      pos.setY(-_height / 2 - 3);
      break;
    case Graphic::DOWN:
      pos.setY((_name_item.boundingRect().width() / 2));
      pos.setX(_width/ 2 + 3);
      break;
    case Graphic::UP:
      pos.setY(-(_name_item.boundingRect().width() / 2));
      pos.setX(-_width / 2 - 3);
      break;
  }
	_name_item.setPos(pos);
}


QSizeF GpxBlock::sizeHint ( Qt::SizeHint which, const QSizeF & constraint  ) const
{
  qreal h,w;
  h = _height + _name_item.boundingRect().height() +5;
  w = qMax(_width  + 10.0,  _name_item.boundingRect().width() +5);

  return QSizeF(w,h);
}

QPainterPath GpxBlock::shape() const 
{
  QPainterPath path;
  path.addPolygon(_myPolygon);
  return path;
}

void GpxBlock::removeInport(int portNum)
{
  GpxConnectionInport *p = _inPorts.takeAt(portNum);
	QList<GpxEdge*> list;
	for (int i=0;i<p->edgeCount();i++) {
		list << p->edgeAt(i); // Iterate over all connections
	}
	if (list.size()) 
	{
		GpxEditionScene *s = dynamic_cast<GpxEditionScene*>(scene());
		s->removeLines(list);
		updatePortsPositions();
	}
  delete p;
}

void GpxBlock::removeInport()
{
  GpxConnectionInport *p = _inPorts.takeLast();
	QList<GpxEdge*> list;
	for (int i=0;i<p->edgeCount();i++) {
		list << p->edgeAt(i); // Iterate over all connections
	}
	if (list.size()) 
	{
		GpxEditionScene *s = dynamic_cast<GpxEditionScene*>(scene());
		s->removeLines(list);
		updatePortsPositions();
	}
  delete p;
}

void GpxBlock::removeOutport(int portNum)
{
  GpxConnectionOutport *p = _outPorts.takeAt(portNum);
	QList<GpxEdge*> list;
	for (int i=0;i<p->edgeCount();i++) {
		list << p->edgeAt(i); // Iterate over all connections
	}
	if (list.size()) 
	{
		GpxEditionScene *s = dynamic_cast<GpxEditionScene*>(scene());
		s->removeLines(list);
		updatePortsPositions();
	}
  delete p;
	updatePortsPositions();
}

void GpxBlock::removeOutport()
{
  GpxConnectionOutport *p = _outPorts.takeLast();
	QList<GpxEdge*> list;
	for (int i=0;i<p->edgeCount();i++) {
		list << p->edgeAt(i); // Iterate over all connections
	}
	if (list.size()) 
	{
		GpxEditionScene *s = dynamic_cast<GpxEditionScene*>(scene());
		s->removeLines(list);
		updatePortsPositions();
	}
  delete p;
	updatePortsPositions();
}

void GpxBlock::addInport()
{
  	GpxConnectionInport *p = new GpxConnectionInport(this,_myDirection);
  	p->setDirection(direction());
	_inPorts.append(p);
	updatePortsPositions();
	updateTextPosition();
}

void GpxBlock::addOutport()
{
  GpxConnectionOutport *p = new GpxConnectionOutport(this,_myDirection);
  p->setDirection(direction());
	_outPorts.append(p);
	updatePortsPositions();
	updateTextPosition();
}

void GpxBlock::unsetCursor()
{
  for (int i = 0; i < _inPorts.size(); i++) 
    _inPorts.at(i)->unsetCursor();
  for (int i = 0; i < _outPorts.size(); i++) 
    _outPorts.at(i)->unsetCursor();
  QGraphicsItem::unsetCursor();
  setAcceptHoverEvents(false);
}

void GpxBlock::updatePortsPositions()
{
	int i;
  	prepareGeometryChange();
	GpxConnectionPoint *p;
	if (_height < _inPorts.size() * PORTS_SIZE) {
		_height = _inPorts.size() * PORTS_SIZE;
	}
	if (_height < _outPorts.size() * PORTS_SIZE) {
		_height = _outPorts.size() * PORTS_SIZE;
	}
  if (_inPorts.size()) {
  	qreal block = _height / _inPorts.size();
  	for (i = 0; i < _inPorts.size(); i++) {
      if (direction()==Graphic::RIGHT || direction()==Graphic::UP)
  		  p = _inPorts.at(i);
      else
  		  p = _inPorts.at(_inPorts.size()-1-i);
  		int x = -(_width / 2 + 2); 
			x-=x % GRID + GRID;
  		int y = (block / 2) + i * block - _height / 2;
			y-=y % GRID;
  		p->setPos(x,y);
			for (unsigned j=0;j<p->edgeCount();j++)
				p->edgeAt(j)->updatePosition();
  	}
  }
  if (_outPorts.size()) {
    qreal block = _height / _outPorts.size();
  	for (i = 0; i < _outPorts.size(); i++) {
      if (direction()==Graphic::RIGHT || direction()==Graphic::UP)
	  	  p = _outPorts.at(i);
      else
	  	  p = _outPorts.at(_outPorts.size()-1-i);
			int x =(_width / 2 + 6);
			x-=x % GRID - GRID;
			int y = (block / 2) + i * block - _height / 2;
			y-=y % GRID;
	  	p->setPos(x,y);
			for (unsigned j=0;j<p->edgeCount();j++)
				p->edgeAt(j)->updatePosition();
	  }
  }
  update();
}

qreal GpxBlock::rotationAngle(Graphic::Direction dir, Graphic::Direction prev)
{
  qreal angle;
  angle = clockwiseDiff(dir, prev);
  return 90*angle;
}

/*qreal GpxBlock::rotationAngle(Graphic::Direction dir)
{
	qreal angle;
	switch (dir)
  	{
  	case Graphic::RIGHT:
    angle =0;
    break;
	case Graphic::UP:
    angle=-90;
		break;
	case Graphic::DOWN:
    angle=90;
		break;
	case Graphic::LEFT:
    angle=180;
  }
  return angle;
}*/

// Checks if the local direction is initialized.

bool GpxBlock::checkDirection(int dir)
{
	if(dir == Graphic::RIGHT || dir == Graphic::LEFT || dir == Graphic::UP || dir == Graphic::DOWN)
		return true;
	else
		return false;
}

void GpxBlock::setDirection(Graphic::Direction dir)
{
  qreal angle;
  if(_myDirection != dir && checkDirection(_myDirection))
    angle=rotationAngle(_myDirection, dir);
  else
  	angle = rotationAngle(Graphic::RIGHT,dir);
  _myDirection = dir;
	QGraphicsItem::setRotation(angle);
	foreach(GpxConnectionInport* port, _inPorts) {
	    port->setDirection(direction());
		port->updatePosition();
	}
	foreach(GpxConnectionOutport* port, _outPorts) {
	    port->setDirection(direction());
		port->updatePosition();
	}
	updatePortsPositions();
	updateTextPosition();
}

QPixmap GpxBlock::pixmap()
{
	QPixmap pixmap(_width + 10, _height + 10);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.translate((_width + 10) / 2, (_height + 10) / 2);
  //painter.rotate(-rotationAngle(Graphic::RIGHT,_myDirection));
  painter.rotate(-rotationAngle(Graphic::RIGHT,_myDirection));
	paint(&painter, NULL, NULL);
	return pixmap;
}

void GpxBlock::paintAsSelected(QPainter *painter)
{
  if (isSelected()) {
    painter->setPen(QPen (Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    painter->drawLine(QPointF(-(_width / 2 + 2), -(_height / 2 - 2)), QPointF(-(_width / 2 + 2), -(_height / 2 + 2)));
    painter->drawLine(QPointF(-(_width / 2 + 2), -(_height / 2 + 2)), QPointF(-(_width / 2 - 2), -(_height / 2 + 2)));
    painter->drawLine(QPointF(-(_width / 2 + 2), (_height / 2 - 2)), QPointF(-(_width / 2 + 2), (_height / 2 + 2)));
    painter->drawLine(QPointF(-(_width / 2 + 2), (_height / 2 + 2)), QPointF(-(_width / 2 - 2), (_height / 2 + 2)));
    painter->drawLine(QPointF((_width / 2 + 2), -(_height / 2 - 2)), QPointF((_width / 2 + 2), -(_height / 2 + 2)));
    painter->drawLine(QPointF((_width / 2 + 2), -(_height / 2 + 2)), QPointF((_width / 2 - 2), -(_height / 2 + 2)));
    painter->drawLine(QPointF((_width / 2 + 2), (_height / 2 - 2)), QPointF((_width / 2 + 2), (_height / 2 + 2)));
    painter->drawLine(QPointF((_width / 2 + 2), (_height / 2 + 2)), QPointF((_width / 2 - 2), (_height / 2 + 2)));
  }
}

QVariant GpxBlock::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if (change == QGraphicsItem::ItemPositionHasChanged) {
		prepareGeometryChange();
		setPos((int)x()-((int)x() % GRID), (int)y()-((int)y() % GRID));
		foreach(GpxConnectionInport* port, _inPorts) {
			port->updatePosition();
		}
		foreach(GpxConnectionOutport* port, _outPorts) {
			port->updatePosition();
		}
		updatePortsPositions();
   	emit modified();
	}
	return value;
} 
bool GpxBlock::isAtCorner(QPointF m, Qt::Corner &c)
{
	m = mapFromScene(m);
	//double offset = 6;
	if (fabs(-(_width / 2) - m.x()) < _offset && fabs(-(_height / 2) - m.y()) < _offset)
  {
		c = Qt::TopRightCorner;
    return true;
  }
	if (fabs((_width / 2) - m.x()) < _offset && fabs(-(_height / 2) - m.y()) < _offset)
  {
		c = Qt::TopLeftCorner;
    return true;
  }
	if (fabs((_width / 2) - m.x()) < _offset && fabs((_height / 2) - m.y()) < _offset)
  {
		c =Qt::BottomLeftCorner;
    return true;
  }
	if (fabs(-(_width / 2) - m.x()) < _offset && fabs((_height / 2) - m.y()) < _offset)
  {
		c = Qt::BottomRightCorner;
    return true;
  }
	return false;
}

void GpxBlock::rotate()
{
  prepareGeometryChange();
	QGraphicsItem::setRotation(rotation() + 90);
  switch(direction())
  {
    case Graphic::RIGHT:
      _myDirection = Graphic::DOWN;
      break;
    case Graphic::DOWN:
      _myDirection = Graphic::LEFT;
      break;
    case Graphic::LEFT:
      _myDirection = Graphic::UP;
      break;
    case Graphic::UP:
      _myDirection = Graphic::RIGHT;
      break;
  }
	foreach(GpxConnectionInport* port, _inPorts) {
    port->setDirection(direction());
		port->updatePosition();
	}
	foreach(GpxConnectionOutport* port, _outPorts) {
    port->setDirection(direction());
		port->updatePosition();
	}
	updatePortsPositions();
	updateTextPosition();
	update();
}

void GpxBlock::resizeTo(QPointF p,Qt::Corner resizeCorner)
{
	prepareGeometryChange();
	p = mapFromScene(p);
	switch (resizeCorner) {
	case Qt::TopRightCorner:
		_width = -p.x() + _width / 2;
		_height = -p.y() + _width / 2;
		break;
	case Qt::TopLeftCorner:
		_width = p.x() + _width / 2;
		_height = -p.y() + _width / 2;
		break;
	case Qt::BottomLeftCorner:
		_width = p.x() + _width / 2;
		_height = p.y() + _width / 2;
		break;
	case Qt::BottomRightCorner:
		_width = -p.x() + _width / 2;
		_height = p.y() + _width / 2;
		break;

	}
	if (_width < 20) {
		_width = 20;
	}
	if (_height < 20) {
		_height = 20;
	}
	if (_height < _inPorts.size() * 12) {
		_height = _inPorts.size() * 12;
	}
	if (_height < _outPorts.size() * 12) {
		_height = _outPorts.size() * 12;
	}
	foreach(GpxConnectionInport* port, _inPorts) {
		port->updatePosition();
	}
	foreach(GpxConnectionOutport* port, _outPorts) {
		port->updatePosition();
	}
	updatePortsPositions();
	update();
	updateTextPosition();
}

void GpxBlock::resizeTo(QPoint p)
{
	prepareGeometryChange();
	_width = p.x();
	_height = p.y();
	if (_width < MIN_WIDTH) {
		_width = MIN_WIDTH;
	}
	if (_height < MIN_HEIGHT) {
		_height = MIN_HEIGHT;
	}
	if (_height < _inPorts.size() * 12) {
		_height = _inPorts.size() * 12;
	}
	if (_height < _outPorts.size() * 12) {
		_height = _outPorts.size() * 12;
	}
	foreach(GpxConnectionInport* port, _inPorts) {
		port->updatePosition();
	}
	foreach(GpxConnectionOutport* port, _outPorts) {
		port->updatePosition();
	}
	updatePortsPositions();
	update();
	updateTextPosition();
}

void GpxBlock::setHeight(unsigned int h)
{
	prepareGeometryChange();
  	_height = h;
	updatePortsPositions();
	update();
	updateTextPosition();
}

void GpxBlock::setWidth(unsigned int w)
{
	prepareGeometryChange();
   	_width = w;
	updatePortsPositions();
	update();
	updateTextPosition();
}


QList<GpxEdge*> GpxBlock::connectedLines()
{
	QList<GpxEdge *> list;
	foreach(GpxConnectionInport *p, _inPorts)
		for (int i=0;i<p->edgeCount();i++)
		list << p->edgeAt(i); // Iterate over all connections
	qDebug() << "Number of edges in the Inport: " << list.size();
	foreach(GpxConnectionOutport *p, _outPorts)
		for (int i=0;i<p->edgeCount();i++)
		list << p->edgeAt(i); // Iterate over all connections
	qDebug() << "Number of edges in the outport: " << list.size();
	return list;
}

int GpxBlock::clockwiseDiff(Graphic::Direction d1, Graphic::Direction d2)
{
     int ret;
	 switch(d1){
     		case Graphic::DOWN:
				switch(d2){
     				case Graphic::DOWN:
       					ret = 0;
						break;
      				case Graphic::UP:
        				ret = 2;
						break;
      				case Graphic::LEFT:
        				ret =  1;
						break;
      				case Graphic::RIGHT:
        				ret = 3;
						break;
    			}
				break;
      		case Graphic::UP:
				switch(d2){
      				case Graphic::DOWN:
        				ret = 2;
						break;
      				case Graphic::UP:
        				ret = 0;
						break;
      				case Graphic::LEFT:
        				ret = 3;
						break;
      				case Graphic::RIGHT:
        				ret = 1;
						break;
    			}
				break;
      		case Graphic::LEFT:
				switch(d2){
      				case Graphic::DOWN:
        				ret = 3;
						break;
      				case Graphic::UP:
        				ret = 1;
						break;
      				case Graphic::LEFT:
        				ret = 0;
						break;
      				case Graphic::RIGHT:
        				ret = 2;
						break;
    			}
				break;
      		case Graphic::RIGHT:
				switch(d2){
  					case Graphic::DOWN:
       					ret = 1;
						break;
   					case Graphic::UP:
       					ret = 3;
						break;
   					case Graphic::LEFT:
       					ret = 2;
						break;
   					case Graphic::RIGHT:
       					ret = 0;
						break;
   			}
   	}
	return ret;
}

void GpxBlock::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
  Qt::Corner c;
  QPointF pos = event->scenePos();
  if (isAtCorner(pos,c)) {
      if (c==Qt::TopLeftCorner || c==Qt::BottomRightCorner) {
          if (direction()==Graphic::LEFT || direction()==Graphic::RIGHT)
            setCursor(Qt::SizeBDiagCursor);
          else
            setCursor(Qt::SizeFDiagCursor);
      } else {
          if (direction()==Graphic::LEFT || direction()==Graphic::RIGHT)
            setCursor(Qt::SizeFDiagCursor);
          else
            setCursor(Qt::SizeBDiagCursor);
      }
  } else {
    setCursor(Qt::OpenHandCursor); 
  }  
}

/*
void GpxBlock::removePort(GpxConnectionPoint * p,
			  GpxConnectionPoint::ConnectionPointType pt)
{
	int index;
	GpxConnectionPoint *tmp;
	if (pt == GpxConnectionPoint::InPort) {
		for (int i=0;i<p->numberOfEdges();i++) {
			GpxEdge *edge = p->getEdgeAt(i);
			if (edge != NULL && edge->destPort()->connectionPointType() == GpxConnectionPoint::NoConnection) {
				scene()->removeItem(edge->destPort());
			}
		}
		index = inPorts.indexOf(p);
		if (index != -1)
			tmp = inPorts.takeAt(index);
		p->removeEdges();
		delete tmp;
	} else {
		for (int i=0;i<p->numberOfEdges();i++) {
			GpxEdge *edge = p->getEdgeAt(i);
			if (edge != NULL && edge->destPort()->connectionPointType() == GpxConnectionPoint::NoConnection) {
				scene()->removeItem(edge->destPort());
			}
		}
		index = outPorts.indexOf(p);
		tmp = outPorts.takeAt(index);
		p->removeEdges();
		delete tmp;
	}
	updatePortsPositions();
}

void GpxBlock::removePorts()
{
	foreach(GpxConnectionPoint * port, inPorts) {
		for (int i=0;i<port->numberOfEdges();i++) {
			GpxEdge *edge = port->getEdgeAt(i);
			if (edge != NULL && edge->destPort()->connectionPointType() == GpxConnectionPoint::NoConnection)
				scene()->removeItem(edge->destPort());
		}
		port->removeEdges();
		delete port;
	}
	foreach(GpxConnectionPoint * port, outPorts) {
		for (int i=0;i<port->numberOfEdges();i++) {
			GpxEdge *edge = port->getEdgeAt(i);
			if (edge != NULL
		    	&& edge->destPort()->connectionPointType() ==
			    GpxConnectionPoint::NoConnection)
				scene()->removeItem(edge->destPort());
		}
		port->removeEdges();
		delete port;
	}
}

void GpxBlock::setSize(qreal w, qreal h)
{
	width = w;
	height = h;
	myPolygon.clear();
	myPolygon << QPointF(-(width / 2), (height / 2)) << QPointF((width / 2),
								    (height /
								     2))
	    << QPointF((width / 2), -(height / 2)) << QPointF(-(width / 2),
							      -(height / 2))
	    << QPointF(-(width / 2), (height / 2));
	updatePortsPositions();
	prepareGeometryChange();
	update();
	updateTextPosition();
	return;
}

*/
