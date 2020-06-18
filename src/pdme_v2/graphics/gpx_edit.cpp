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

#include <QGraphicsWidget>
#include <QPainter>

#include <graphics/gpx_edit.h>
#include <graphics/gpx_block.h>
#include <graphics/gpx_coupled.h>
#include <graphics/gpx_atomic.h>
#include <graphics/gpx_inport.h>
#include <graphics/gpx_outport.h>
#include <graphics/gpx_edge.h>

EditAtomic::EditAtomic(GpxAtomic *a) : _atomic(a) 
{
}

EditAtomic::~EditAtomic()
{
}

void EditAtomic::resizeTo(QPointF p,Qt::Corner c)
{
	_atomic->resizeTo(p,c);
}

void EditAtomic::resizeTo(QPoint p)
{
	_atomic->resizeTo(p);
}

void EditAtomic::setWidth(unsigned int w)
{
	_atomic->setWidth(w);
}

void EditAtomic::setHeight(unsigned int h)
{
	_atomic->setHeight(h);
}

int EditAtomic:: height()
{
	_atomic->getHeight();
}

int EditAtomic::width()
{
	_atomic->getWidth();
}

void EditAtomic::setName(QString name)
{
	_atomic->setName(name);
}

QString EditAtomic::name()
{
	_atomic->name();
}

void EditAtomic::copy()
{
}

void EditAtomic::paste()
{
}

void EditAtomic::select()
{
}

void EditAtomic::cut()
{
}

void EditAtomic::rotate()
{
}

void EditAtomic::moveTo()
{
}

void EditAtomic::insert()
{
}

void EditAtomic::deleteItem()
{
}

QPointF EditAtomic::pos()
{
	return _atomic->scenePos();
}

void EditAtomic::setPos(QPointF p)
{
	_atomic->setPos(p);
}

EditCoupled::EditCoupled(GpxCoupled *c) : _coupled(c)
{
}

EditCoupled::~EditCoupled()
{
}

void EditCoupled::resizeTo(QPointF p,Qt::Corner c)
{
	_coupled->resizeTo(p,c);
}

void EditCoupled::resizeTo(QPoint p)
{
	_coupled->resizeTo(p);
}

void EditCoupled::setWidth(unsigned int w)
{
	_coupled->setWidth(w);
}

void EditCoupled::setHeight(unsigned int h) 
{
	_coupled->setHeight(h);
}

int EditCoupled::height()
{
	_coupled->getHeight();
}

int EditCoupled::width()
{
	_coupled->getWidth();
}

void EditCoupled::setName(QString name)
{
	_coupled->setName(name);
}

QString EditCoupled::name()
{
	_coupled->name();
}

void EditCoupled::copy()
{
}

void EditCoupled::paste()
{
}

void EditCoupled::select()
{
}

void EditCoupled::cut()
{
}

void EditCoupled::rotate()
{
}

void EditCoupled::moveTo()
{
}

void EditCoupled::insert()
{
}

void EditCoupled::deleteItem()
{
}

QPointF EditCoupled::pos()
{
	return _coupled->scenePos();
}

void EditCoupled::setPos(QPointF p)
{
	_coupled->setPos(p);
}


EditInport::EditInport(GpxInport *i) : _inport(i) 
{
}

EditInport::~EditInport()
{
}

void EditInport::resizeTo(QPointF p,Qt::Corner c)
{
	_inport->resizeTo(p,c);
}

void EditInport::resizeTo(QPoint p)
{
	_inport->resizeTo(p);
}

void EditInport::setWidth(unsigned int w)
{
	_inport->setWidth(w);
}

void EditInport::setHeight(unsigned int h)
{
	_inport->setHeight(h);
}

int EditInport::height()
{
	return _inport->getHeight();
}

int EditInport::width()
{
	return _inport->getWidth();
}

void EditInport::setName(QString name)
{
	_inport->setName(name);
}

QString EditInport::name()
{
	return _inport->name();
}

void EditInport::copy()
{
}

void EditInport::paste()
{
}

void EditInport::select()
{
}

void EditInport::cut()
{
}

void EditInport::rotate()
{
	_inport->rotate();
}

void EditInport::moveTo()
{
}

void EditInport::insert()
{
}

void EditInport::deleteItem()
{
}

QPointF EditInport::pos()
{
	return _inport->scenePos();
}

void EditInport::setPos(QPointF p)
{
	_inport->setPos(p);
}


EditOutport::EditOutport(GpxOutport *o) : _outport(o)
{
}

EditOutport::~EditOutport()
{
}

void EditOutport::resizeTo(QPointF p,Qt::Corner c)
{
	_outport->resizeTo(p,c);
}

void EditOutport::resizeTo(QPoint p)
{
	_outport->resizeTo(p);
}

void EditOutport::setWidth(unsigned int w)
{
	_outport->setWidth(w);
}

void EditOutport::setHeight(unsigned int h)
{
	_outport->setHeight(h);
}

int EditOutport::height()
{
	_outport->getHeight();
}

int EditOutport::width()
{
	return _outport->getWidth();
}

void EditOutport::setName(QString name)
{
	_outport->setName(name);
}

QString EditOutport::name()
{
	return _outport->name();
}

void EditOutport::copy()
{
}

void EditOutport::paste()
{
}

void EditOutport::select()
{
}

void EditOutport::cut()
{
}

void EditOutport::rotate()
{
	_outport->rotate();
}

void EditOutport::moveTo()
{
}

void EditOutport::insert()
{
}

void EditOutport::deleteItem()
{
}

QPointF EditOutport::pos()
{
	return _outport->scenePos();
}

void EditOutport::setPos(QPointF p)
{
	_outport->setPos(p);
}

EditEdge::EditEdge(GpxEdge *e) : _edge(e)
{
}

EditEdge::~EditEdge()
{
}

void EditEdge::resizeTo(QPointF p,Qt::Corner c)
{
}

void EditEdge::resizeTo(QPoint p)
{
}

void EditEdge::setWidth(unsigned int w)
{
}

void EditEdge::setHeight(unsigned int h)
{
}

int EditEdge::height()
{
}

int EditEdge::width()
{
}

void EditEdge::setName(QString name)
{
}

QString EditEdge::name()
{
}

void EditEdge::copy()
{
}

void EditEdge::paste()
{
}

void EditEdge::select()
{
}

void EditEdge::cut()
{
}

void EditEdge::rotate()
{
}

void EditEdge::moveTo()
{
}

void EditEdge::insert()
{
}

void EditEdge::deleteItem()
{
}

QPointF EditEdge::pos()
{
	//_origPos = _edge->polygon();
	//return _edge->pos();
}

void EditEdge::setPos(QPointF p)
{
	//_edge->setPoly(_origPos);
}
