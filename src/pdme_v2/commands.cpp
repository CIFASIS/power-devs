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

#include <QApplication>

#include "commands.h"

CopyCmd::CopyCmd(GpxEditionWindow *gew) : QUndoCommand("Copy command")
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(gew->getSelection());
	_gew = gew;
}

void CopyCmd::undo()
{
	qDebug() << "UNDO";
}

void CopyCmd::redo()
{
	qDebug() << "REDO";
}

bool CopyCmd::mergeWith(const QUndoCommand *command)
{
	qDebug() << "MERGE";
	return true;
}

MoveCmd::MoveCmd(GpxEditionScene *ges) : QUndoCommand("Move command")
{
	_ges = ges;
	_items = ges->editItems();
	foreach(GpxEdit *item,_items)
		_pos << item->pos();
	foreach(GpxEdit *item,_items)
		_oldPos << item->pos();
}

void MoveCmd::undo()
{
	qDebug() << "Move command UNDO";
	int i = 0;
	foreach(GpxEdit *item,_items){
		item->setPos(_oldPos[i]);
		i++;
	}
}

void MoveCmd::redo()
{
	qDebug() << "Move command REDO";
	int i = 0;
	foreach(GpxEdit *item,_items){
		item->setPos(_pos[i]);
		i++;
	}
}

bool MoveCmd::mergeWith(const QUndoCommand *command)
{
	qDebug() << "Move command MERGE";
	const MoveCmd *merge = static_cast<const MoveCmd*>(command);
	if(_items != merge->_items && _pos == merge->_pos)
		return false;
	qDebug() << "New Pos: ";
	qDebug() << _pos;
	_pos = merge->_pos;
	return true;
}

SelectCmd::SelectCmd(GpxEditionScene *ges, QPointF start) : QUndoCommand("Select command")
{
	_ges = ges;
    _polygonStart = start;
	_polygonEnd = ges->mousePos();
}

void SelectCmd::undo()
{
	qDebug() << "Select command UNDO";
	_ges->clearSelection();
}

void SelectCmd::redo()
{
	qDebug() << "Select command REDO";
	QPainterPath qp;
    qp.addPolygon(QPolygonF (QRectF(_polygonStart, _polygonEnd).normalized())); 
    //_ges->setSelectionArea(qp);
}

bool SelectCmd::mergeWith(const QUndoCommand *command)
{
	qDebug() << "Select command MERGE";
	const SelectCmd *merge = static_cast<const SelectCmd*>(command);
	if(_polygonEnd == merge->_polygonEnd)
		return false;
	_polygonEnd = merge->_polygonEnd;
	return true;
}

ResizeCmd::ResizeCmd(GpxEditionScene *ges) : QUndoCommand("Resize command")
{
	_ges = ges;
	_items = ges->editItems();
	foreach(GpxEdit *item, _items){
		QPoint itemPos;
		itemPos.setX(item->width());
		itemPos.setY(item->height());
		_pos << itemPos;
		_oldPos << itemPos;
		qDebug() << "Block original pos: " << itemPos.x();
		qDebug() << "Mouse original pos: " << itemPos.y();
	}
}

void ResizeCmd::undo()
{
	qDebug() << "Resize command UNDO";
	int i = 0;
	foreach(GpxEdit *item, _items){
		item->resizeTo(_oldPos[i]);
		i++;
	}
}

void ResizeCmd::redo()
{
	qDebug() << "Resize command REDO";
	int i = 0;
	foreach(GpxEdit *item, _items){
		item->resizeTo(_pos[i]);
		i++;
	}
}

bool ResizeCmd::mergeWith(const QUndoCommand *command)
{
	qDebug() << "Resize command MERGE";
	const ResizeCmd *merge = static_cast<const ResizeCmd*>(command);
	if(_items != merge->_items && _pos == merge->_pos)
		return false;
	_pos = merge->_pos;
	qDebug() << "Merging pos:  " << merge->_pos;
	return true;
}
