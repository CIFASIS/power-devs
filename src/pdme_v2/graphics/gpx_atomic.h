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

#ifndef GPXATOMIC_H
#define GPXATOMIC_H

#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QDebug>

#include <data/atomic.h>
#include <graphics/gpx_block.h>

class GpxAtomic:public GpxBlock {
  Q_OBJECT
public:
	GpxAtomic(QGraphicsScene * scene, Atomic *a);
	virtual ~GpxAtomic();
  virtual int type()
  {
    return GpxBlock::SimpleAtomic;
  }
  virtual bool isAtomic() const { return true; }
  Atomic *atomicData() { return _atomicData; }
  vector<Parameter*> parameters() { return _atomicData->parameters(); }
  Model *model() { return _atomicData; };
  virtual void rotate(); 
  virtual void resizeTo(QPointF,Qt::Corner);
  virtual void resizeTo(QPoint);
  virtual void setInportsTo(unsigned int p);
  virtual void setOutportsTo(unsigned int p);
  virtual void setColor(unsigned int);
  int color();	
  QVariant itemChange(GraphicsItemChange change, const QVariant & value);
  virtual void setIcon(QString);
  QString icon();
  virtual void setWidth(int);
  virtual void setHeight(int);
  virtual void setDirection(Graphic::Direction d);
  void setPath(QString);
  QString path() { return _atomicData->path().c_str(); };
  GpxEdit *edit() { return _edit; };
signals:
  void parametersDialog(GpxBlock*);
protected:
  void paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *);
  void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );
private:
  void adjustSVG();
  Atomic *_atomicData;
	QColor _myBackgroundColor;
  QImage _myImage;
  bool valid_image;
  QGraphicsSvgItem *svg;
  EditAtomic *_edit;
};
#endif
