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

#include <QGraphicsSvgItem>
#ifndef GPXCOUPLED_H
#define GPXCOUPLED_H

#include <data/coupled.h>
#include <graphics/gpx_block.h>

class GpxCoupled:public GpxBlock {
  Q_OBJECT
public:
	GpxCoupled(QGraphicsScene * scene,Coupled *c);
	virtual ~GpxCoupled();
  Coupled *coupledData() { return _coupledData; };
  Model *model() { return coupledData(); }
  bool isCoupled() const { return true; };
  vector<Parameter*> parameters() { return _coupledData->parameters() ; }
  void addPort(Port *);
  void removePort(Port *);
  virtual void setIcon(QString);
  QString icon();
  virtual void setColor(unsigned int);
  int color();
  virtual void rotate(); 
  virtual void resizeTo(QPointF,Qt::Corner);
  virtual void resizeTo(QPoint);
  QVariant itemChange(GraphicsItemChange change, const QVariant & value);
  virtual void setWidth(int);
  virtual void setHeight(int);
  virtual void setDirection(Graphic::Direction d);
  GpxEdit *edit() { return _edit; };
signals:
  void parametersDialog(GpxBlock*);
protected:
  void paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *);
  void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );
private:
  void adjustSVG();
  QColor _myBackgroundColor;
  QImage _myImage;
  Coupled *_coupledData;
  bool valid_image;
  QGraphicsSvgItem *svg;
  EditCoupled *_edit;
};

#endif
