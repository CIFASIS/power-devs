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

#ifndef GPXBLOCK_H
#define GPXBLOCK_H

#include <QGraphicsWidget>
#include <QPainter>
#include <QDebug>

#include <data/model.h>
#include <graphics/gpx_connection_inport.h>
#include <graphics/gpx_connection_outport.h>
#include <graphics/gpx_textbox.h>
#include <graphics/gpx_edit.h>

#define GPXBLOCK_TYPE 15

class GpxBlock : public QGraphicsWidget
{
 Q_OBJECT
 public:
  GpxBlock();
  virtual ~GpxBlock() {};
	enum { Type = UserType + GPXBLOCK_TYPE };
	enum BlockType { 
    SimpleAtomic=Type,  /**< Graphic item to represent an Atomic model.*/
		InPort,				      /**< Graphic item to represent an Inport for a Coupled model.  */
		OutPort,			      /**< Graphic item to represent an OutPort for a Coupled model. */
		CoupledModel        /**< Graphic item to represent a Coupled model. */
	};
  int type() const
  {
    return Type;
  }
  virtual QPixmap pixmap() ;
  virtual bool isAtomic() const { return false; };
  virtual bool isCoupled() const { return false; };
  virtual bool isInport() const { return false; };
  virtual bool isOutport() const { return false; };
  Graphic::Direction direction() { return _myDirection; }
  virtual void setDirection(Graphic::Direction d);
  bool isAtCorner(QPointF m, Qt::Corner &c);
  virtual void rotate();
  virtual void resizeTo(QPointF,Qt::Corner);
  virtual void resizeTo(QPoint);
  void removeInport(int);
  void removeInport();
  void removeOutport(int);
  void removeOutport();
  virtual Model *model()=0;
  virtual void setInportsTo(unsigned int) {};
  virtual void setOutportsTo(unsigned int) {};
  virtual void setColor(unsigned int) {};
  virtual void setIcon(QString) {};
  QVariant itemChange(GraphicsItemChange change, const QVariant & value);
  void setWidth(unsigned int);
  void setHeight(unsigned int);
  int getHeight() { return _height; };
  int getWidth() { return _width ; };
  int getBoundingWidth() { return _width+10; };
  int getBoundingHeight() { return _height+_name_item.boundingRect().height(); };
  void setName(QString);
  QString name() { return _name_item.toPlainText(); }
  GpxConnectionInport  *inport(unsigned int i) { if (i>=_inPorts.size()) 
      qDebug() << "block " << name() << " has only " << _inPorts.size() << " inports"; 
      assert(i<_inPorts.size()); return _inPorts[i]; }
  GpxConnectionOutport *outport(unsigned int i) { assert(i<_outPorts.size()); return _outPorts[i]; }
  unsigned int inportIndex(GpxConnectionPoint *p) { return _inPorts.indexOf((GpxConnectionInport*)p); }
  unsigned int outportIndex(GpxConnectionPoint *p) { return _outPorts.indexOf((GpxConnectionOutport*)p); }
  QList<GpxEdge*> connectedLines();
  void updateTextPosition();
  virtual GpxEdit *edit() = 0;
  void unsetCursor();
signals:
  void modified();

protected:
  QSizeF sizeHint ( Qt::SizeHint which, const QSizeF & constraint = QSizeF() ) const;
  void updatePortsPositions();
  virtual void addInport();
  virtual void addOutport();
  virtual QRectF boundingRect() const;
  QPainterPath shape() const;
  qreal rotationAngle(Graphic::Direction d, Graphic::Direction prev) ;
  void paintAsSelected(QPainter *painter);
  int clockwiseDiff(Graphic::Direction d1, Graphic::Direction d2);
  bool checkDirection(int);
  void hoverMoveEvent( QGraphicsSceneHoverEvent * event );

  QPolygonF _myPolygon;
  QList<GpxConnectionInport*> _inPorts;
  QList<GpxConnectionOutport*> _outPorts;
  Graphic::Direction _myDirection;
  int _width,_height;
  GpxTextBox _name_item;
  int _offset;
};

#endif
