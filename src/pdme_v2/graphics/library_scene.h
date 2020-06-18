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

#ifndef LIBRARYSCENE_H
#define LIBRARYSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsGridLayout>

#include <data/coupled.h>
#include <graphics/gpx_block.h>
class QGraphicsSceneMouseEvent;
class QDrag;

class LibraryScene:public QGraphicsScene {
 public:
	LibraryScene(QObject * parent = 0, Coupled *c = 0);
  GpxBlock *block();
  QList<GpxBlock*> blocks;
  unsigned columns;
  QGraphicsGridLayout *layout;
  QGraphicsWidget *layout_widget;
 protected:
	void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
 private:
	 Coupled *_coupledData;
};

#endif
