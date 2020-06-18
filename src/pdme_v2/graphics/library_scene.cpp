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

#include <vector>
#include <iostream>
#include <sstream>

#include <QtGui>
#include <QGraphicsGridLayout>
#include <QMimeData>

#include "globals.h"
#include "library_scene.h"
#include "graphics/gpx_atomic.h"
#include "graphics/gpx_coupled.h"
#include "graphics/gpx_inport.h"
#include "graphics/gpx_outport.h"

using namespace std;

LibraryScene::LibraryScene(QObject * parent, Coupled * c):QGraphicsScene (parent)
{
  _coupledData = c; 
  for (int child=0;child<_coupledData->childCount();child++)
  {
    Model *m = _coupledData->child(child);
    if (m->isAtomic())
    {
        Atomic *atomicData = (dynamic_cast<Atomic*> (m)); 
        GpxAtomic *a = new GpxAtomic(this,atomicData);
        a->setFlag(QGraphicsItem::ItemIsMovable, false);
        addItem(a);
        a->updateTextPosition();
        blocks << a;
        a->unsetCursor();
    } 
    else
    {
        Coupled *coupledData = (dynamic_cast<Coupled*> (m)); 
        GpxCoupled *cp = new GpxCoupled(this,coupledData);
        cp->unsetCursor();
        addItem(cp);
        blocks << cp;
    }
  }
  for (int inport=0;inport<_coupledData->inportCount();inport++)
  {
    Port *p = _coupledData->inport(inport);
    GpxInport *gp = new GpxInport(this,p);
    gp->unsetCursor();
    addItem(gp);
    blocks << gp;
  }
  for (int outport=0;outport<_coupledData->outportCount();outport++)
  {
    Port *p = _coupledData->outport(outport);
    GpxOutport *gp = new GpxOutport(this,p);
    gp->unsetCursor();
    blocks << gp;
    addItem(gp);
  }
  double max_width=1,max_height=1;
  for (int i=0;i<blocks.size();i++)
  {
    if (max_width<blocks.at(i)->getBoundingWidth())
      max_width=blocks.at(i)->getBoundingWidth();
    if (max_height<blocks.at(i)->getBoundingHeight())
      max_height=blocks.at(i)->getBoundingHeight();
  }
  max_width+=20;

  int columns = getSetting("LibraryWidth").toInt()/max_width;
  columns = (columns < 1 ? 1 : columns);
  int col=0,row=0;
  for (int i=0;i<blocks.size();i++) {
    blocks.at(i)->setPos(col*max_width,row*max_height);
    col++;
    if (col==columns) {
      row++;
      col=0;
    }
  }
  setSceneRect(-max_width/2-10,-max_height/2-10,max_width*columns,row*max_height);
}

void LibraryScene::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->button() != Qt::LeftButton) {
		event->ignore();
		return;
	}
	QList < QGraphicsItem * >clickedItems = items(event->scenePos());
	if (!clickedItems.isEmpty() &&
       clickedItems.first() != NULL &&
  	   clickedItems.first()->type() == GpxBlock::Type) {

		GpxBlock *item = qgraphicsitem_cast < GpxBlock * >(clickedItems.first());
    QMimeData *mimeData = new QMimeData;
		QDrag *drag = new QDrag(event->widget());
    drag->setPixmap(item->pixmap());
    if (item->isAtomic())
    {
      GpxAtomic *a = dynamic_cast<GpxAtomic*>(item);
      ostringstream stream;
      depth=0;
      stream << *a->atomicData();
      depth=0;
      mimeData->setText(stream.str().c_str());
    }
    if (item->isInport())
    {
      GpxInport *p = dynamic_cast<GpxInport*>(item);
      ostringstream stream;
      depth=0;
      stream << *p->portData();
      depth=0;
      mimeData->setText(stream.str().c_str());
    }
    if (item->isOutport())
    {
      GpxOutport *p = dynamic_cast<GpxOutport*>(item);
      ostringstream stream;
      depth=0;
      stream << *p->portData();
      depth=0;
      mimeData->setText(stream.str().c_str());
    }
    if (item->isCoupled())
    {
      GpxCoupled *c = dynamic_cast<GpxCoupled*>(item);
      ostringstream stream;
      depth=0;
      stream << *c->coupledData();
      depth=0;
      mimeData->setText(stream.str().c_str());
    }
 
		drag->setMimeData(mimeData);
		drag->setHotSpot(QPoint(15, 30));
		drag->start();
  }
}

GpxBlock *LibraryScene::block()
{
  QList<QGraphicsItem *> it = items();
  foreach(QGraphicsItem *i,it)
  {
    if (i->type()==GpxBlock::Type)
    {
      GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(i);
      return b;
    }
  }
  return NULL;
}
