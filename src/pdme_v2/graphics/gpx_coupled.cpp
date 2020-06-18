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
#include <QDebug>
#include <QGraphicsSvgItem>

#include <powergui.h>
#include <globals.h>
#include <data/coupled.h>
#include <graphics/gpx_coupled.h>

GpxCoupled::GpxCoupled(QGraphicsScene * scene, Coupled *coupledData):_coupledData(coupledData)
{
  svg = NULL;
  Graphic &gpx = coupledData->graphic();
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
  for (int p=0;p<coupledData->inPorts();p++) {
    addInport();
    _inPorts.at(p)->setToolTip(coupledData->inport(p)->name().c_str());
  }
  for (int p=0;p<coupledData->outPorts();p++) {
    addOutport();
    _outPorts.at(p)->setToolTip(coupledData->outport(p)->name().c_str());
  }
  setName(coupledData->name().c_str());
	setToolTip(QString(coupledData->description().c_str()).replace("\\n","\n"));
  _edit = new EditCoupled(this);
}


GpxCoupled::~GpxCoupled()
{
	if(svg != NULL)
		delete svg;
}

void GpxCoupled::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *)
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
	
	}
}

void GpxCoupled::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{ 
  if (flags() & QGraphicsItem::ItemIsMovable)
    emit parametersDialog(this);  
}

void GpxCoupled::rotate()
{
    GpxBlock::rotate(); 
    _coupledData->graphic().setDirection(_myDirection); 
    if (svg!=NULL) {
		adjustSVG();
      qDebug() << svg->pos();
    }
    qDebug() << "Rotating";
}

void GpxCoupled::resizeTo(QPointF p,Qt::Corner c)
{
  GpxBlock::resizeTo(p,c);
  _coupledData->graphic().setWidth(_width);
  _coupledData->graphic().setHeight(_height);
  if (svg!=NULL) 
  	adjustSVG();
}

void GpxCoupled::resizeTo(QPoint p)
{
  GpxBlock::resizeTo(p);
  _coupledData->graphic().setWidth(_width);
  _coupledData->graphic().setHeight(_height);
  if (svg!=NULL) 
  	adjustSVG();
}

QVariant GpxCoupled::itemChange(GraphicsItemChange change, const QVariant & value)
{
  QVariant v=GpxBlock::itemChange(change,value);
	if (change == QGraphicsItem::ItemPositionHasChanged) {
	//if (change == QGraphicsItem::ItemScenePositionHasChanged) {
		setPos((int)x()-(int)x() % GRID,
		(int)y()-(int)y() % GRID);
    _coupledData->graphic().setX(x()-getWidth()/2);
    _coupledData->graphic().setY(y()-getHeight()/2);
  }
  return v;
}

void GpxCoupled::addPort(Port *p)
{
	if(p->type() == Port::Inport){
		_coupledData->addInport(p);
		addInport();
	} else {
		_coupledData->addOutport(p);
        addOutport(); 
	}
}

void GpxCoupled::removePort(Port *p)
{
	if(p->type() == Port::Inport){
		_coupledData->removeInport(p);
    removeInport(p->portNumber()); 
		_coupledData->setInports(_coupledData->inportCount());
	} else {
		_coupledData->removeOutport(p);
    removeOutport(p->portNumber()); 
		_coupledData->setOutports(_coupledData->inportCount());
	}
	delete p;
}

void GpxCoupled::setIcon(QString i)
{
  if (!i.endsWith(".svg")){
    valid_image = _myImage.load(getImage(i));
	_coupledData->graphic().setIcon(qPrintable(i));
	if(!valid_image)
		_coupledData->graphic().setIcon("None");
	if(svg != NULL){
		delete svg;
		svg = NULL;
	}
  } else if (svg == NULL){
    svg = new QGraphicsSvgItem(getImage(i),this);
    svg->setFlag(QGraphicsItem::ItemStacksBehindParent,true );
    svg->setFlag(QGraphicsItem::ItemIgnoresTransformations,true );
	adjustSVG();
	_coupledData->graphic().setIcon(qPrintable(i));
  } else {
  	delete svg;
    svg = new QGraphicsSvgItem(getImage(i),this);
    svg->setFlag(QGraphicsItem::ItemStacksBehindParent,true );
    svg->setFlag(QGraphicsItem::ItemIgnoresTransformations,true );
	adjustSVG();
	_coupledData->graphic().setIcon(qPrintable(i));
  }
}

void GpxCoupled::setColor(unsigned int c)
{
  _coupledData->graphic().setColor(c);
  _myBackgroundColor = intToColor(c);
  update();
}

void GpxCoupled::setWidth(int w)
{
  Graphic &gpx = _coupledData->graphic();
  gpx.setWidth(w);
  GpxBlock::setWidth(w);
  if (!QString(gpx.icon().c_str()).endsWith(".svg"))
    valid_image = _myImage.load(getImage(gpx.icon().c_str()));
  else if (svg != NULL) 
  	adjustSVG();
}

void GpxCoupled::setHeight(int h)
{
  Graphic &gpx = _coupledData->graphic();
  gpx.setHeight(h);
  GpxBlock::setHeight(h);
  if (!QString(gpx.icon().c_str()).endsWith(".svg"))
    valid_image = _myImage.load(getImage(gpx.icon().c_str()));
  else if (svg != NULL) 
  	adjustSVG();
}

void GpxCoupled::setDirection(Graphic::Direction d)
{
  GpxBlock::setDirection(d);
  _coupledData->graphic().setDirection(d);
  _myDirection = d;
}

int GpxCoupled::color()
{
	return colorToInt(_myBackgroundColor);
}

QString GpxCoupled::icon()
{
  return _coupledData->graphic().icon().c_str();
}

void GpxCoupled::adjustSVG()
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
