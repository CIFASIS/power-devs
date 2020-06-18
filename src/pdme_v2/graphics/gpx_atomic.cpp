/****************************************************************************
**
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**										 Universidad Nacional de Rosario - Argentina.
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

#include <QPointF>
#include <QDebug>
#include <QGraphicsSvgItem>
#include <QTextDocument>

#include <globals.h>
#include <data/graphic.h>
#include <graphics/gpx_atomic.h>
#include <graphics/gpx_edition_scene.h>

GpxAtomic::GpxAtomic(QGraphicsScene * scene, Atomic *a): GpxBlock(), _atomicData(a)
{
  svg = NULL;
  Graphic &gpx = a->graphic();
  _width=gpx.width();
  _height=gpx.height();
  _myBackgroundColor = intToColor(gpx.color());
  _myDirection = gpx.direction();
  setDirection(gpx.direction());
  if (!QString(gpx.icon().c_str()).endsWith(".svg"))
    valid_image = _myImage.load(getImage(gpx.icon().c_str()));
  else {
    svg = new QGraphicsSvgItem(getImage(gpx.icon().c_str()),this);
	adjustSVG();
    svg->setFlag(QGraphicsItem::ItemStacksBehindParent,true );
    svg->setFlag(QGraphicsItem::ItemIgnoresTransformations,true );
  }
  for (int p=0;p<a->inPorts();p++)
    GpxBlock::addInport();
  for (int p=0;p<a->outPorts();p++)
    GpxBlock::addOutport();
  setName(a->name().c_str());
	setAcceptHoverEvents(true);
	setToolTip(QString(a->description().c_str()).replace("\\n","\n"));
  _name_item.setTextWidth(_width+10);
  _name_item.document()->setTextWidth(_width+10);
  QTextOption o;
  o.setWrapMode(QTextOption::WordWrap);
  o.setAlignment(Qt::AlignCenter);
  _name_item.document()->setDefaultTextOption(o);
  _edit = new EditAtomic(this);
}

GpxAtomic::~GpxAtomic()
{
  if (svg!=NULL)
    delete svg;
}

void GpxAtomic::setIcon(QString i)
{
  if (!i.endsWith(".svg")){
    valid_image = _myImage.load(getImage(i));
	_atomicData->graphic().setIcon(qPrintable(i));
	if(!valid_image)
		_atomicData->graphic().setIcon("None");
	if(svg != NULL){
		delete svg;
		svg = NULL;
	}
  } else if (svg == NULL){
    svg = new QGraphicsSvgItem(getImage(i),this);
    svg->setFlag(QGraphicsItem::ItemStacksBehindParent,true );
    svg->setFlag(QGraphicsItem::ItemIgnoresTransformations,true );
	adjustSVG();
	_atomicData->graphic().setIcon(qPrintable(i));
  } else {
  	delete svg;
    svg = new QGraphicsSvgItem(getImage(i),this);
    svg->setFlag(QGraphicsItem::ItemStacksBehindParent,true );
    svg->setFlag(QGraphicsItem::ItemIgnoresTransformations,true );
	adjustSVG();
	_atomicData->graphic().setIcon(qPrintable(i));
  }
}

void GpxAtomic::setDirection(Graphic::Direction d)
{
  GpxBlock::setDirection(d);
  _atomicData->graphic().setDirection(d);
  _myDirection = d;
}

void GpxAtomic::setPath(QString path)
{
	_atomicData->setPath(qPrintable(path));
}

void GpxAtomic::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	_myPolygon.clear();
	_myPolygon << QPointF(-(_width / 2), (_height / 2))
            << QPointF((_width / 2), (_height / 2)) 
            << QPointF((_width / 2), -(_height / 2)) 
            << QPointF(-(_width / 2), -(_height / 2)) 
            << QPointF(-(_width / 2), (_height / 2));
	painter->setPen(QPen (Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->setBrush(_myBackgroundColor);
  if (valid_image || svg!=NULL)
	  painter->drawPolyline(_myPolygon);
  else
	  painter->drawPolygon(_myPolygon);
  //painter->drawRect(boundingRect());
  paintAsSelected(painter);
  if (valid_image && svg==NULL) {
  	qreal angle=rotationAngle(Graphic::RIGHT,direction());
		painter->rotate(-angle);
    QRectF rect = _myPolygon.boundingRect();
    if (direction()==Graphic::DOWN || direction()==Graphic::UP) 
    {
      qDebug() << rect;
      QPointF p=rect.topLeft(); 
      QPointF p2=rect.bottomRight(); 
      rect.setTopLeft(QPointF(p.y(),p.x()));
      rect.setBottomRight(QPointF(p2.y(),p2.x()));
      qDebug() << rect;
    }
  	painter->drawImage(rect.adjusted(1, 1, -1, -1), _myImage, _myImage.rect());
	} else if(svg!=NULL) { 
    //svg->setScale(std::min((_width-3)/svg->boundingRect().width(),(_height-3)/svg->boundingRect().height()));
    //svg->setPos(-svg->scale()*svg->boundingRect().width()/2,-svg->scale()*svg->boundingRect().height()/2);
  }
}

void GpxAtomic::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{ 
  if (flags() & QGraphicsItem::ItemIsMovable) 
  {
    emit parametersDialog(this);  
  }
}

void GpxAtomic::rotate()
{
    GpxBlock::rotate(); 
    _atomicData->graphic().setDirection(_myDirection); 
    if (svg!=NULL) {
		  adjustSVG();
      qDebug() << svg->pos();
    }
}

void GpxAtomic::setInportsTo(unsigned int p)
{
    int diff = p - _atomicData->inPorts();  
    if (diff<0)
    {
      for (int i=0;i<-diff;i++) 
        removeInport(p); 
    } else {
      for (int i=0;i<diff;i++) 
        addInport(); 
    }
    _atomicData->setInports(p);
	updatePortsPositions();
}

void GpxAtomic::setOutportsTo(unsigned int p)
{
    int diff = p - _atomicData->outPorts();  
    if (diff<0)
    {
      for (int i=0;i<-diff;i++) 
        removeOutport(p); 
    } else {
      for (int i=0;i<diff;i++) 
        addOutport(); 
    }
    _atomicData->setOutports(p);
	updatePortsPositions();
}

void GpxAtomic::resizeTo(QPointF p,Qt::Corner c)
{
  GpxBlock::resizeTo(p,c);
  _atomicData->graphic().setWidth(_width);
  _atomicData->graphic().setHeight(_height);
  if (svg!=NULL) 
  	adjustSVG();
}

void GpxAtomic::resizeTo(QPoint p)
{
  GpxBlock::resizeTo(p);
  _atomicData->graphic().setWidth(_width);
  _atomicData->graphic().setHeight(_height);
  if (svg!=NULL) 
  	adjustSVG();
}

QVariant GpxAtomic::itemChange(GraphicsItemChange change, const QVariant & value)
{
  QVariant v=GpxBlock::itemChange(change,value);
	if (change == QGraphicsItem::ItemPositionHasChanged) {
//	if(change == QGraphicsItem::ItemScenePositionHasChanged) {
		setPos((int)x()-(int)x() % GRID,
		(int)y()-(int)y() % GRID);
    _atomicData->graphic().setX(x()-getWidth()/2);
    _atomicData->graphic().setY(y()-getHeight()/2);
  }
  return v;
}

void GpxAtomic::setColor(unsigned int c)
{
  _myBackgroundColor = intToColor(c);
  _atomicData->graphic().setColor(c);
  update();
}

void GpxAtomic::setWidth(int w)
{
  Graphic &gpx = _atomicData->graphic();
  gpx.setWidth(w);
  GpxBlock::setWidth(w);
  if (!QString(gpx.icon().c_str()).endsWith(".svg"))
    valid_image = _myImage.load(getImage(gpx.icon().c_str()));
  else if (svg != NULL) 
  	adjustSVG();
}
	
void GpxAtomic::setHeight(int h)
{
  Graphic &gpx = _atomicData->graphic();
  gpx.setHeight(h);
  GpxBlock::setHeight(h);
  if (!QString(gpx.icon().c_str()).endsWith(".svg"))
    valid_image = _myImage.load(getImage(gpx.icon().c_str()));
  else if (svg != NULL) 
  	adjustSVG();
}

int GpxAtomic::color()
{
	return colorToInt(_myBackgroundColor);
}

QString GpxAtomic::icon()
{
  return _atomicData->graphic().icon().c_str();
}	

void GpxAtomic::adjustSVG()
{
      if (direction()==Graphic::DOWN) {
        svg->setScale(std::min((_height-3)/svg->boundingRect().width(),(_width-3)/svg->boundingRect().height()));
        svg->setPos(-svg->scale()*svg->boundingRect().height()/2,svg->scale()*svg->boundingRect().width()/2);
      } else if (direction()==Graphic::RIGHT) {
        svg->setScale(std::min((_width-3)/svg->boundingRect().width(),(_height-3)/svg->boundingRect().height()));
        svg->setPos(-svg->scale()*svg->boundingRect().width()/2,-svg->scale()*svg->boundingRect().height()/2);
      } else if (direction()==Graphic::LEFT) {
        svg->setScale(std::min((_width-3)/svg->boundingRect().width(),(_height-3)/svg->boundingRect().height()));
        svg->setPos(svg->scale()*svg->boundingRect().width()/2,svg->scale()*svg->boundingRect().height()/2);
      } else {
        svg->setScale(std::min((_height-3)/svg->boundingRect().width(),(_width-3)/svg->boundingRect().height()));
        svg->setPos(svg->scale()*svg->boundingRect().height()/2,-svg->scale()*svg->boundingRect().width()/2);
      }
}

/*
void GpxAtomic::addInport()
{
  GpxBlock::addInport();
  _atomicData->setInports(_atomicData->inPorts()+1);
}

void GpxAtomic::addOutport()
{
  GpxBlock::addOutport();
  _atomicData->setOutports(_atomicData->outPorts()+1);
}
*/
