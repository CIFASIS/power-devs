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
#include <QGraphicsScene>
#include <math.h>

#include "data/graphic.h"
#include "gpx_edge.h"
#include "gpx_connection_point.h"
#include "gpx_connection_cross.h"
#include "gpx_connection_node.h"
#include "gpx_edition_scene.h"

GpxEdge::GpxEdge(GpxConnectionPoint * sourcePort, GpxConnectionPoint * destPort, Line *l): _lineData(l)
{
	_source = sourcePort;
	_dest = destPort;
	_sourcePoint = mapFromItem(_source, _source->getConnectionPoint());
	_destPoint = mapFromItem(_dest, _dest->getConnectionPoint());
	_movingEdge = false;
	for(unsigned int i=0;i<l->pointCount();i++)
		_myPolygon << QPointF(l->pointX(i),l->pointY(i));
  if (_myPolygon.size() == 0) {
    _myPolygon << _sourcePoint;
    _myPolygon << _sourcePoint;
    _myPolygon << _destPoint;
  }
  if (destPort->isCross())
  {
    GpxConnectionCross *c = qgraphicsitem_cast<GpxConnectionCross*>(destPort);
    c->setDirection(findDirection(_myPolygon.at(_myPolygon.size()-2), _myPolygon.at(_myPolygon.size()-1)));
  }

	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
 // 	setFlag(QGraphicsItem::ItemSendsScenePositionChanges,true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
  _source->addEdge(this);
  _dest->addEdge(this);
  // Just a temporal fix, to see if the size of the lines is the bug with old models.
  while(_myPolygon.size() < 3)
  	_myPolygon.append(_myPolygon.last());
  simplifyEdge();
  adjustPolygon();
  _edit = new EditEdge(this);
  setAcceptHoverEvents(true);
}

GpxEdge::~GpxEdge()
{
  qDebug() << "Delteing one edge";
  if (_source!=NULL) {
    qDebug() << " with source is a " << (_source->isCross() ? "Cross" : (_source->isNode() ? "Node " : " Port "));
    _source->removeEdge(this);
    if (_source->isCross())
    {
      _source->scene()->removeItem(_source);
      delete _source;
    } 
  }
  if (_dest!=NULL) {
    qDebug() << " and with dest is a " << (_dest->isCross() ? "Cross" : (_dest->isNode() ? "Node " : " Port "));
    _dest->removeEdge(this);
     if (_dest->isCross())
     {
       		_dest->scene()->removeItem(_dest);
	        delete _dest;
    }
  }
}

GpxConnectionPoint *GpxEdge::sourcePort() const
{
	return _source;
}

void GpxEdge::setSourcePort(GpxConnectionPoint * port)
{
	_source = port;
  if (_source!=NULL) {
    qDebug() << "My new source is a " << _source->typeName();
	  updatePosition();
  }
}

GpxConnectionPoint *GpxEdge::destPort() const
{
	return _dest;
}

void GpxEdge::setDestPort(GpxConnectionPoint * port)
{
	_dest = port;
  if (_dest!=NULL) {
    qDebug() << "My new dest is a " << _dest->typeName();
	  updatePosition();
  }
}

void GpxEdge::addPoint(QPointF p, bool updatePos)
{
	qDebug() << _myPolygon;
  _myPolygon.pop_back();
	qDebug() << _myPolygon;
  _myPolygon << p;
	qDebug() << _myPolygon;
	_destPoint = _dest->getSceneConnectionPoint();
  _myPolygon << _destPoint;
	qDebug() << _myPolygon;
  if (_dest->isCross())
  {
    GpxConnectionCross *c = qgraphicsitem_cast<GpxConnectionCross*>(_dest);
    c->setDirection(findDirection(_myPolygon.last(), _myPolygon.at(_myPolygon.size()-2)));
  }
  if (updatePos)
	  updatePosition();
}

int GpxEdge::segmentDirection(QPointF p1, QPointF p2)
{
	if(p1.x() == p2.x())
		return 0;
	else
		return 1;
	// This should never happen.
	Q_ASSERT(false);
}

void GpxEdge::updatePosition(bool adjust)
{
	prepareGeometryChange();
  if (_myPolygon.size()<3)
	{
		_myPolygon.prepend(_myPolygon.first());
		_myPolygon.append(_myPolygon.last());
	}

	_sourcePoint = _source->getSceneConnectionPoint();
	_destPoint = _dest->getSceneConnectionPoint();
  QPointF diff;

	if (_sourcePoint != _myPolygon.at(0)) {
    diff = _myPolygon.at(0) - _sourcePoint;
		Graphic::Direction direction = findDirection(_myPolygon.at(1), _myPolygon.at(0));
		_myPolygon.replace(0, _sourcePoint);
		QPointF middlePoint;
		switch (direction) {
		case Graphic::RIGHT:
		case Graphic::LEFT:
			middlePoint.setX(_myPolygon.at(2).x());
			middlePoint.setY(_sourcePoint.y());
			break;
		case Graphic::UP:
		case Graphic::DOWN:
			middlePoint.setX(_sourcePoint.x());
			middlePoint.setY(_myPolygon.at(2).y());
		}
		_myPolygon.replace(1, middlePoint);
	}
	if (_destPoint != _myPolygon.last()) {
		Graphic::Direction direction = findDirection(_myPolygon.at(_myPolygon.size() - 3), _myPolygon.at(_myPolygon.size() - 2));
		QPointF middlePoint;
		_myPolygon.replace(_myPolygon.size() - 1, _destPoint);
		switch (direction) {
		case Graphic::RIGHT:
		case Graphic::LEFT:
			middlePoint.setX(_destPoint.x());
			middlePoint.setY(_myPolygon.at(_myPolygon.size() - 3).  y());
			break;
		case Graphic::UP:
		case Graphic::DOWN:
			middlePoint.setX(_myPolygon.at(_myPolygon.size() - 3).  x());
			middlePoint.setY(_destPoint.y());
		default:
			break;
		}
		_myPolygon.replace(_myPolygon.size() - 2, middlePoint);
	}
	simplifyEdge();
  adjustPolygon();
  _lineData->clearPoints();
  for(int i=0;i<_myPolygon.size();i++) 
	{
    	QPointF p=_myPolygon.at(i);
      p = mapToScene(p);
    	_lineData->addPoint(p.x(),p.y());
	}
  if (adjust && isSelected() && _myPolygon.size()>3) {
    if (diff.x()!=0 || diff.y()!=0.0)
    {
      // TODO: For do it correctly it should only adjust points in the middle of the polygon and 
      // the correct coordinate of the second and the previous to the last one
      for(int i=1;i<_myPolygon.size()-2;i++) 
        _myPolygon[i] -= diff;
      qDebug() << "Fix position " << diff; 
    }
  }
}

void GpxEdge::updateSegmentsPositions(bool which)
{
	int _xDiff, _yDiff, _xSign, _ySign;
	QPointF compare, _first;
	if(which){
		_xDiff = abs((int)_myPolygon.at(0).x() - (int)_sourcePoint.x());
		_yDiff = abs((int)_myPolygon.at(0).y() - (int)_sourcePoint.y());
		compare = _sourcePoint;
		_first = _myPolygon.at(0);
	} else {
		_xDiff = abs((int)_myPolygon.last().x() - (int)_destPoint.x());
		_yDiff = abs((int)_myPolygon.last().y() - (int)_destPoint.y());
		compare = _destPoint;
		_first = _myPolygon.last();
	}
	if(_first.x() <= 0 && compare.x() >= 0)
		_xSign = 1;
	else if (_first.x() >= 0 && compare.x() <= 0)
		_xSign = -1;
	else if(_first.x() < compare.x())
		_xSign = 1;
	else
		_xSign = -1;
	if(_first.y() <= 0 && compare.y() >= 0)
		_ySign = 1;
	else if (_first.y() >= 0 && compare.y() <= 0)
		_ySign = -1;
	else if(_first.y() < compare.y())
		_ySign = 1;
	else
		_ySign = -1;
	// Update the position of all the middle segments remaining.
	for(int i = 2; i < _myPolygon.size()-2; i++){
		QPointF rep;
		rep.setX(_myPolygon.at(i).x()+ (_xSign * _xDiff));
		rep.setY(_myPolygon.at(i).y()+ (_ySign * _yDiff));
		_myPolygon.replace(i,rep);
	}
	_movingEdge = false;
}

QRectF GpxEdge::boundingRect() const
{
	if (!_source || !_dest)
		return QRectF();

//      qreal penWidth =  1;
	qreal extra = 50;
	QRectF rectangle = _myPolygon.boundingRect();
	return rectangle.adjusted(-extra, -extra, extra, extra);
}

void GpxEdge::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if (!_source || !_dest)
		return;

	//Draw the line itself
	painter-> setPen(QPen (Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawPolyline(_myPolygon);
	painter-> setPen(QPen (Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	// If the edge is selected, it makes its points visible.
	if (isSelected()) {
		for (int i = 0; i < _myPolygon.size() - 1; i++) {
			QPointF point = _myPolygon.at(i);
			painter->drawPoint(point);
		}
	}
}

  /*
void GpxEdge::addPoint(QPointF point)
{
	painter->
	myPolygon << point;
}

void GpxEdge::preAddPoint(QPointF point)
{
	myPolygon.prepend(point);
}
*/

QPainterPath GpxEdge::shape() const
{
	QPainterPath path;
	for (int i = 0; i < _myPolygon.size() - 1; i++) {
		QPointF init = _myPolygon.at(i), end = _myPolygon.at(i + 1);
		if (fabs(init.x() - end.x())<GRID)
			path.  addRect(QRectF (init.x() - 3, init.y(), 6, end.y() - init.y()).normalized());
		else
			path.  addRect(QRectF (init.x(), init.y() - 3, end.x() - init.x(), 6).normalized());
	}
	return path;
}

Graphic::Direction GpxEdge::findDirection(const QPointF middlePoint, const QPointF endPoint)
{
	if (middlePoint.x() > endPoint.x() && fabs(middlePoint.y() - endPoint.y())<GRID)	
		return Graphic::LEFT;
	else if (middlePoint.x() <= endPoint.x() && fabs(middlePoint.y() - endPoint.y())<GRID)	
		return Graphic::RIGHT;
	else if (middlePoint.y() < endPoint.y())
		return Graphic::DOWN;
	else		
		return Graphic::UP;
}

void GpxEdge::mergeWith(GpxEdge *e, QPointF m)
{
	QPolygonF p = e->polygon();
  _myPolygon << m;
  for(int i=p.size()-1;i>=0;i--) {
    _myPolygon << p.at(i);
  }
  _dest->removeEdge(this);
  scene()->removeItem(_dest);
	delete _dest;
  setDestPort(e->sourcePort());
  e->destPort()->removeEdge(e);
  e->sourcePort()->removeEdge(e);
  e->sourcePort()->addEdge(this);
	// Update extrems if  e->sourcePort is a Node
  GpxConnectionPoint *end = e->destPort();
  scene()->removeItem(e->destPort());
	delete e->destPort();
  e->setDestPort(NULL);
  e->setSourcePort(NULL);
  updatePosition();
  delete e;
	simplifyEdge();
	adjustPolygon();
}

QPointF GpxEdge::breakAt(QPointF point)
{
	QPointF newPoint = mapToItem(this,point);
	for (int i = 0; i < _myPolygon.size() - 1; i++) {
		QPointF init = _myPolygon.at(i), end = _myPolygon.at(i + 1);
		QRectF rectangle;
		if (init.x() == end.x()) // Insert in vertical line.
			rectangle = QRect(init.x() - 3, init.y(), 6, end.y() - init.y()) .normalized();
		else // Insert in horizontal line.
			rectangle = QRectF(init.x(), init.y() - 3, end.x() - init.x(), 6) .normalized();
		if (rectangle.contains(newPoint)){
			if (init.x() == end.x()) {
				int y=point.y();
				y-= y % GRID;
				return QPointF(init.x(),y);
			} else {
				int x=point.x();
				x-= x % GRID;
				return QPointF(x,init.y());
			}
		}
	}
	// This should never happen.
	Q_ASSERT(false);
	/*
  QPolygonF old = polygon(), rest;
  destPort()->removeEdge(this);
  bool vertical;
  bool found=false;
  _myPolygon.clear();
  for (int i=0;i<old.size()-1;i++)
  {
    QPointF init = old.at(i), end = old.at(i + 1);
		QRectF rectangle;
		if (init.x() == end.x()) {
			rectangle = QRectF(init.x() - 3, init.y(), 6, end.y() - init.y()) .normalized();
			vertical = true;
		} else {
			rectangle = QRectF(init.x(), init.y() - 3, end.x() - init.x(), 6) .normalized();
			vertical = false;
		}
    if (!found) {
      _myPolygon << init;
    } else
      rest << init;
		  if (rectangle.contains(point)) {
        found = true;
        if (vertical) {
          point.setX(init.x());
        } else {
          point.setY(init.y());
        }
      }
  }
  _myPolygon << point;
  if (_myPolygon.size()<3) {
    // Create an extra point
    QPointF start=_myPolygon.at(0), end=_myPolygon.at(1);
    if (start.x()==end.x())
      _myPolygon.insert(1,QPointF(end.x(),std::min(end.y(),start.y())+fabs(start.y()-end.y())/2));
    else
      _myPolygon.insert(1,QPointF(std::min(end.x(),start.x())+fabs(start.x()-end.x())/2,end.y()));
  }
  rest.prepend(point);
  n->setPos(point);
  GpxEdge *newEdge = new GpxEdge(n,destPort());
  destPort()->addEdge(newEdge);
  setDestPort(n);
  scene()->addItem(newEdge);
  newEdge = new GpxEdge(t->getInitPort(),n);
  n->addEdge(this);
  scene()->addItem(newEdge);
	*/
}

void GpxEdge::simplifyEdge()
{
  bool cont;
  do {
    cont=false;
	  if (_myPolygon.size() > 3) {
		  for (int i = 1; i < _myPolygon.size() - 2; i++) {
			  if (fabs(_myPolygon.at(i).x() - _myPolygon.at(i + 2).x())<GRID/2) 
  			{
	  			  _myPolygon[i+2].setX(_myPolygon[i].x());
		  		  _myPolygon.remove(i + 1);
			  	  cont=true;
            	qDebug() << "Removing one node X";
            break;
	  		}
		  	if (fabs(_myPolygon.at(i).y() - _myPolygon.at(i + 2).y())<GRID/2 )
  			{
	  			  _myPolygon[i+2].setY(_myPolygon[i].y());
		  		  _myPolygon.remove(i + 1);
			  	  cont=true;
           	 	  qDebug() << "Removing one node Y";
            break;
	  		}
		  }
  	}
  } while (cont);
  Q_ASSERT(_myPolygon.size() >= 3);
}

void GpxEdge::adjustPolygon()
{
	// Adjust polygon to grid coordenates.
	for(int i = 2; i < _myPolygon.size()-2; i++){
	//for(int i = 0; i < _myPolygon.size(); i++){
    	_myPolygon[i].setX((int)_myPolygon[i].x()-((int)_myPolygon[i].x() % GRID));
    	_myPolygon[i].setY((int)_myPolygon[i].y()-((int)_myPolygon[i].y() % GRID));
  	}		
	
  // Check for diagonals in the middle segments.
 for(int i = 0; i < _myPolygon.size()-2; i++){
    qreal x_diff = fabs(_myPolygon[i].x() - _myPolygon[i+1].x());
    qreal y_diff = fabs(_myPolygon[i].y() - _myPolygon[i+1].y());
	if((y_diff > x_diff) && x_diff){ // if it's a vertical segment.
		_myPolygon[i+1].setX(_myPolygon[i].x());
  	}else if((x_diff > y_diff) && y_diff){ 
		_myPolygon[i+1].setY(_myPolygon[i].y());
	}
  }	
  // The last segment must be checked separately.
  // Extra checks added to avoid diagonals in the last segment.
  // Check to avoid z-like diagonals.
	bool verticalDir = false;
	int last = _myPolygon.size()-1;
    /*qreal x_diff = fabs(_myPolygon[last-1].x() - _myPolygon[last].x());
    qreal y_diff = fabs(_myPolygon[last-1].y() - _myPolygon[last].y());
	if((y_diff > x_diff) && x_diff){ // if it's a vertical segment.
		if(_myPolygon.at(last-1).x() != _myPolygon.last().x()){
			QPointF newPoint;
			newPoint.setX(_myPolygon.last().x());
			newPoint.setY(_myPolygon.at(last-1).y());
			_myPolygon.insert(last,newPoint);
		}	
  	}else if((x_diff > y_diff) && y_diff){ 
		if(_myPolygon.at(last-1).y() != _myPolygon.last().y()){
			QPointF newPoint;
			newPoint.setX(_myPolygon.at(last-1).x());
			newPoint.setY(_myPolygon.last().y());
			_myPolygon.insert(last,newPoint);
		}	
	}*/
	if(_myPolygon[last-2].y() == _myPolygon[last-1].y())
		verticalDir = true;
	if(verticalDir)
		_myPolygon[last-1].setX(_myPolygon[last].x());
	else
		_myPolygon[last-1].setY(_myPolygon[last].y());
	// Check to avoid straight diagonals.
	if(_myPolygon[last] == _myPolygon[last-1]){
		if(verticalDir)
			_myPolygon[last-1].setX(_myPolygon[last-2].x());
		else
			_myPolygon[last-1].setY(_myPolygon[last-2].y());
	}
}

bool GpxEdge::overVerticalSegemt(QPointF point)
{
	QPointF newPoint = mapFromScene(point);
	for (int i = 0; i < _myPolygon.size() - 1; i++) {
		QPointF init = _myPolygon.at(i), end = _myPolygon.at(i + 1);
		QRectF rectangle;
		if (init.x() == end.x())
			rectangle = QRect(init.x() - 3, init.y(), 6, end.y() - init.y()) .normalized();
		else
			rectangle = QRectF(init.x(), init.y() - 3, end.x() - init.x(), 6) .normalized();
		if (rectangle.contains(newPoint))
			return init.x() == end.x();
	}
	return true;
}


void GpxEdge::moveTo(QPointF pos)
{
	QPointF p = mapFromScene(pos);
	p.setX((int)p.x()-(int)p.x() % GRID);
	p.setY((int)p.y()-(int)p.y() % GRID);
	qDebug() << "Move line to " << p;
	if (_myPolygon.at(_movingSegment).x()== _myPolygon.at(_movingSegment+1).x()) {
		_myPolygon[_movingSegment].setX(p.x());
		_myPolygon[_movingSegment+1].setX(p.x());
	} else {
		_myPolygon[_movingSegment].setY(p.y());
		_myPolygon[_movingSegment+1].setY(p.y());
	}
	update();
}


void GpxEdge::stopMoving()
{
  simplifyEdge();
  adjustPolygon();
  if (_dest->isCross())
  {
    GpxConnectionCross *c = qgraphicsitem_cast<GpxConnectionCross*>(_dest);
    c->setDirection(findDirection(_myPolygon.at(_myPolygon.size()-2), _myPolygon.at(_myPolygon.size()-1)));
		qDebug() << "Direction " << c->direction();
  }
	updatePosition(false);
	/*_lineData->clearPoints();
  	foreach(QPointF p,_myPolygon) {
		p = mapFromScene(p);
    	_lineData->addPoint(p.x(),p.y());
	}*/
}

void GpxEdge::startMoving(QPointF p)
{
	QPointF newPoint = mapFromScene(p);
	for (int i = 0; i < _myPolygon.size() - 1; i++) {
		QPointF init = _myPolygon.at(i), end = _myPolygon.at(i + 1);
		QRectF rectangle;
		if (init.x() == end.x())
			rectangle = QRect(init.x() - 3, init.y(), 6, end.y() - init.y()) .normalized();
		else
			rectangle = QRectF(init.x(), init.y() - 3, end.x() - init.x(), 6) .normalized();
		if (rectangle.contains(newPoint)) {
			_movingSegment=i;	
			qDebug() << "Moving segment " << i;
			if (_movingSegment==0) {
				_myPolygon.insert(1,_myPolygon.at(0));
				_movingSegment++;
			} else if (_movingSegment==_myPolygon.size()-2) {
				_myPolygon.append(_myPolygon.last());
			}
			if (_myPolygon.size()==3) {	
				if (_movingSegment==1) 
					_myPolygon.append(_myPolygon.last());
				else
					_myPolygon.prepend(_myPolygon.first());
			}	
			return;
		}
	}
}

QVariant GpxEdge::itemChange(GraphicsItemChange change, const QVariant & value)
{
	//if (change == QGraphicsItem::ItemScenePositionHasChanged || change == QGraphicsItem::ItemPositionHasChanged) {
	if (change == QGraphicsItem::ItemPositionHasChanged) {
		qDebug() << "Edge change position: ";
		qDebug() << pos();
		qDebug() << _myPolygon;
		setPos(0,0);
		_movingEdge = true;
	}
	return value;
}

void GpxEdge::hoverMoveEvent ( QGraphicsSceneHoverEvent * event )
{
  
  QPointF pos = event->scenePos();
  if (overVerticalSegemt(pos))
  {
    setCursor(Qt::SizeHorCursor);
  } else {
    setCursor(Qt::SizeVerCursor);
  }
}

void GpxEdge::setPolygon(QPolygonF polygon) 
{
	_myPolygon = polygon;
  	simplifyEdge();
	updatePosition();
}

void GpxEdge::addPort(GpxConnectionPoint *p) 
{ 
	_connected << p; 
	_connected = _connected.toSet().toList(); 
}

void GpxEdge::addPorts(QList<GpxConnectionPoint *> p) 
{ 
	_connected << p; 
	_connected = _connected.toSet().toList();
}

void GpxEdge::mousePressEvent(QGraphicsSceneMouseEvent * event) 
{
	setSelected(!isSelected()); 
	ungrabMouse(); 
}

/*
void GpxEdge::endMovingEdge()
{
	movingSegment = -1;
}

void GpxEdge::startMovingEdgeAt(QPointF point)
{
	QPointF newPoint = mapFromScene(point);
	for (int i = 0; i < myPolygon.size() - 1; i++) {
		QPointF init = myPolygon.at(i), end = myPolygon.at(i + 1);
		QRectF rectangle;
		if (init.x() == end.x())
			rectangle =
			    QRectF(init.x() - 3, init.y(), 6,
				   end.y() - init.y())
			    .normalized();
		else
			rectangle =
			    QRectF(init.x(), init.y() - 3, end.x() - init.x(),
				   6)
			    .normalized();
		if (rectangle.contains(newPoint)) {
			movingSegment = i;
			break;
		}
	}
}

void GpxEdge::moveEdgeAt(QPointF point)
{
	QPointF newPoint = mapFromScene(point);

	if (movingSegment < 0)
		return;

	QPointF firstPoint = myPolygon.at(movingSegment),
	    secondPoint = myPolygon.at(movingSegment + 1);
	if (firstPoint.x() == secondPoint.x()) {	// Vertical segment.
		firstPoint.setX(newPoint.x());
		secondPoint.setX(newPoint.x());
	} else {		// Horizontal segment.
		firstPoint.setY(newPoint.y());
		secondPoint.setY(newPoint.y());
	}
	myPolygon.replace(movingSegment, firstPoint);
	myPolygon.replace(movingSegment + 1, secondPoint);
	updatePosition();
}

GpxConnectionPoint::Direction GpxEdge::segmentDirectionAt(QPointF point)
{
	QPointF newPoint = mapFromScene(point);
	for (int i = 0; i < myPolygon.size() - 1; i++) {
		QPointF init = myPolygon.at(i), end = myPolygon.at(i + 1);
		QRectF rectangle;
		if (init.x() == end.x())
			rectangle =
			    QRect(init.x() - 3, init.y(), 6, end.y() - init.y())
			    .normalized();
		else
			rectangle =
			    QRectF(init.x(), init.y() - 3, end.x() - init.x(),
				   6)
			    .normalized();
		if (rectangle.contains(newPoint))
			return (init.x() ==
				end.
				x()? GpxConnectionPoint::
				Down : GpxConnectionPoint::Left);
	}
	return GpxConnectionPoint::Left;
}

void GpxEdge::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	setSelected(!isSelected());
	ungrabMouse();
}

QPointF GpxEdge::breakAt(QPointF point, QPolygonF & initPolygon,
			 QPolygonF & endPolygon)
{
	QPointF newPoint = mapFromScene(point), ret;
	bool found = false;
	bool vertical = false;

	for (int i = 0; i < myPolygon.size() - 1; i++) {
		QPointF init = myPolygon.at(i), end = myPolygon.at(i + 1);
		QRectF rectangle;
		if (init.x() == end.x()) {
			rectangle =
			    QRectF(init.x() - 3, init.y(), 6,
				   end.y() - init.y())
			    .normalized();
			vertical = true;
		} else {
			rectangle =
			    QRectF(init.x(), init.y() - 3, end.x() - init.x(),
				   6)
			    .normalized();
			vertical = false;
		}
		if (rectangle.contains(newPoint)) {
			found = true;
			initPolygon << myPolygon.at(i);
			if (vertical) {
				ret.setX(init.x());
				ret.setY(newPoint.y());
			} else {
				ret.setY(init.y());
				ret.setX(newPoint.x());
			}
		} else if (!found)
			initPolygon << myPolygon.at(i);
		else
			endPolygon << myPolygon.at(i);
	}
	endPolygon << myPolygon.at(myPolygon.size() - 1);

	return mapToScene(ret);
}

void GpxEdge::setMiddlePoint()
{
	QPointF middlePoint;
	switch (source->direction()) {
	case Graphic::RIGHT:
	case Graphic::LEFT:
		middlePoint.setX(destPoint.x());
		middlePoint.setY(sourcePoint.y());
		break;
	case Graphic::UP:
	case Graphic::DOWN:
		middlePoint.setX(sourcePoint.x());
		middlePoint.setY(destPoint.y());
	default:
		break;
	}
	_myPolygon.replace(1, middlePoint);
	_myPolygon << destPoint;
}

void GpxEdge::removeConnections()
{
	source->removeEdge(this);
	if (source->connectionPointType() == GpxConnectionPoint::NoConnection
	    && source->numberOfEdges() == 0) {
		scene()->removeItem(source);
		qDebug("Borrando una crush");
		delete source;
	}
	dest->removeEdge(this);
	if (dest->connectionPointType() == GpxConnectionPoint::NoConnection
	    && dest->numberOfEdges() == 0) {
		scene()->removeItem(dest);
		qDebug("Borrando una crush");
		delete dest;
	}
}
*/
