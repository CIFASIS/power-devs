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

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>

#include <graphics/gpx_connection_point.h>
#include <graphics/gpx_edition_scene.h>
#include <graphics/gpx_edge.h>

GpxConnectionPoint::GpxConnectionPoint(QGraphicsItem *p, Graphic::Direction direction): QGraphicsItem(p), _direction(direction)
{
	setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
}

GpxConnectionPoint::~GpxConnectionPoint()
{
	foreach(GpxEdge *e, _edges) {
		if (e->destPort()==this)
			e->setDestPort(NULL);
		else
			e->setSourcePort(NULL);
	}
}


void GpxConnectionPoint::addEdge(GpxEdge *edge) 
{ 
	_edges << edge; 
	_edges = _edges.toSet().toList();
}

void GpxConnectionPoint::removeEdge(GpxEdge *edge) 
{ 
	qDebug() << "Removing edge: " << edge->polygon(); 
	if (isNode() && _edges.size()<3) {
		qDebug() << "ERRRROR" << _edges.size();
	}
	_edges.removeAll(edge); 
	_edges = _edges.toSet().toList();
}

QVariant GpxConnectionPoint::itemChange(GraphicsItemChange change, const QVariant & value)
{
	// If the item position change, we need to adjust the new position
	// to the grid.
	if(change == QGraphicsItem::ItemScenePositionHasChanged)
		setPos((int)x()-(int)x() % GRID, (int)y()-(int)y() % GRID);
	return value;
}

QString GpxConnectionPoint::typeName() 
{
	if (isInport()) 
		return "Inport";
	if (isOutport())
		return "Outport";
	if (isNode())
		return "Node";
	return "Cross";
}
