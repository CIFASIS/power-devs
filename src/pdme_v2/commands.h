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

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QClipboard>

#include <graphics/gpx_edition_window.h>
#include <graphics/gpx_edit.h>

static const int copyCommandId 		= 1000;
static const int moveCommandId 		= 1001;
static const int selectCommandId 	= 1002;
static const int resizeCommandId 	= 1003;
static const int insertCommandId 	= 1004;
static const int deleteCommandId 	= 1005;
static const int rotateCommandId 	= 1006;

class CopyCmd : public QUndoCommand
{
  public:
	CopyCmd(GpxEditionWindow *gew);
	void undo();
	void redo();
	bool mergeWith(const QUndoCommand *command);
	int id() const { return copyCommandId; }
  private:
    GpxEditionWindow *_gew;
};

class MoveCmd : public QUndoCommand
{
  public:
	MoveCmd(GpxEditionScene *ges);
	void undo();
	void redo();
	bool mergeWith(const QUndoCommand *command);
	int id() const { return moveCommandId; }
  private:
    GpxEditionScene *_ges;
	QList<GpxEdit*> _items;
	QList<QPointF> _pos;
	QList<QPointF> _oldPos;
};

class SelectCmd : public QUndoCommand
{
  public:
	SelectCmd(GpxEditionScene *ges, QPointF start);
	void undo();
	void redo();
	bool mergeWith(const QUndoCommand *command);
	int id() const { return selectCommandId; }
  private:
    GpxEditionScene *_ges;
	QPointF _polygonStart;
	QPointF _polygonEnd;
};

class ResizeCmd : public QUndoCommand
{
  public:
	ResizeCmd(GpxEditionScene *);
	void undo();
	void redo();
	bool mergeWith(const QUndoCommand *);
	int id() const { return resizeCommandId; }
  private:
    GpxEditionScene *_ges;
	QList <QPoint> _pos;
	QList <QPoint> _oldPos;
	QList <GpxEdit *> _items;
};

class InsertCmd : public QUndoCommand
{
  public:
	InsertCmd(GpxEditionScene *ges);
	void undo();
	void redo();
	bool mergeWith(const QUndoCommand *command);
	int id() const { return insertCommandId; }
  private:
  	GpxEditionScene *_ges;
};

class DeleteCmd : public QUndoCommand
{
  public:
	DeleteCmd(GpxEditionScene *ges);
	void undo();
	void redo();
	bool mergeWith(const QUndoCommand *command);
	int id() const { return deleteCommandId; }
  private:
};

class RotateCmd : public QUndoCommand
{
  public:
	RotateCmd(GpxEditionScene *ges);
	void undo();
	void redo();
	bool mergeWith(const QUndoCommand *command);
	int id() const { return rotateCommandId; }
  private:
};

#endif
