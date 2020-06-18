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

#ifndef GPXINPORT_H
#define GPXINPORT_H

#include <QGraphicsScene>

#include <data/coupled.h>
#include <graphics/gpx_block.h>

class GpxInport:public GpxBlock {
  Q_OBJECT
public:
	GpxInport(QGraphicsScene * scene, Port *p);
  virtual	~GpxInport();
  virtual int type() { return 66000; }
	BlockType blockType() { return InPort; };
  virtual bool isInport() const { return true; };
  Port *portData() { return _portData; };
  Model *model() { return NULL; };
  virtual void rotate(); 
  virtual void resizeTo(QPointF,Qt::Corner);
  virtual void resizeTo(QPoint);
  QVariant itemChange(GraphicsItemChange change, const QVariant & value);
  GpxEdit *edit() { return _edit; };
protected:
  void paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *);

private:
  Port *_portData;
	QColor _myBackgroundColor;
  unsigned _portNumber;
  EditInport *_edit;
};
#endif
