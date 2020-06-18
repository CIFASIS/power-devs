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

#ifndef GPXEDITIONSCENE_H
#define GPXEDITIONSCENE_H

#include <QGraphicsScene>
#include <QRubberBand>
#ifdef UNDO
	#include <QUndoStack>
	#include <QUndoGroup>
#endif
#include <graphics/gpx_block.h>
#include <graphics/gpx_temp_connection.h>
#include <data/coupled.h>

#define GRID (5)

class GpxConnectionNode;
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QFont;
class QGraphicsTextItem;
class QColor;
class GpxInport;
class GpxOutport;

class GpxEditionScene:public QGraphicsScene 
{
 Q_OBJECT 
public:

  GpxEditionScene(QObject * parent, Coupled * data);
  GpxEditionScene(QObject * parent);
  void rotate();
  void deleteSelection();
  void selectAll();
  Coupled *coupledData() { return _coupledData; }
  void removePort(Coupled *, Port*);
  void addPort(Coupled *, Port *);
  QString getSelection();
  void paste(QString s);
  bool canUseName(QString, GpxBlock*);
  void removeLines(QList<GpxEdge*>);
  void changeBlockName(GpxBlock*);
  void changeRubberBand(); 
  void addAnnotation(QString,QPoint); 
  void convertToCoupled(); 
#ifdef UNDO
  QUndoStack *undoStack() { return _undoStack; };
#endif
  GpxBlock *resizingBlock() { return _resizingBlock; };
  QPointF mousePos() { return _mousePos; };
  Qt::Corner resizingCorner() { return _resizingCorner; };
  QList <GpxEdit *>editItems() ;
  void insert(QString);
  void updateMouseCursor(QGraphicsSceneMouseEvent*);
protected:
  void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );
  void dragMoveEvent(QGraphicsSceneDragDropEvent * event);
  void dropEvent(QGraphicsSceneDragDropEvent * event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
  void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
  void keyPressEvent ( QKeyEvent * keyEvent );
  void keyReleaseEvent ( QKeyEvent * keyEvent );
  void focusOutEvent ( QFocusEvent * focusEvent ); 

  Atomic *getAtomic(QString data);
  Coupled *getCoupled(QString data);
  Port *getPort(QString data);

  void putStructure(Coupled *c, bool paste=false, bool draggin=false);
  void putAtomic(Atomic *a, bool paste=false, bool draggin=false);
  GpxCoupled *putCoupled(Coupled *c, bool paste=false, bool draggin=false);
  void putOutport(Port *p, bool paste=false, bool draggin=false);
  void putInport(Port *p, bool paste=false, bool draggin=false);
  void putPoint(Point *p, bool paste=false, bool draggin=false);
  void putConnection(Line *l, bool paste=false, bool draggin=false, QList<Model*> = QList<Model*>(), QList<Port*> = QList<Port*>(), QList<Point*> = QList<Point*>());
  QString findName(QString propossedName);
  bool nameExists(QString);

signals:
  void contextMenu(GpxBlock *,QPoint);
  void modelParameter(Coupled*);
  void showParameterDialog(GpxBlock *);
  void modified();
  void closeAllChildsOf(Coupled *);
  void remove_Port(Coupled *, Port *);
  void add_Port(Coupled *, Port *);
  void setBlockName(GpxBlock*);

private slots:
  void parameterDialog(GpxBlock*);
public slots:
  void on_modified();

private:
  bool shouldCreatePort(GpxEdge *e);
	void createLineEndingInCross(GpxTempConnection *, QPointF);
	void createNormalConnection(GpxTempConnection *, QPointF, GpxConnectionPoint *);
	void finishALineFromCross(GpxTempConnection *, QPointF, GpxConnectionPoint *);
	void mergeLines(GpxTempConnection *, QPointF, GpxConnectionPoint *);
  	void fromPortToCross( GpxTempConnection *, QPointF, GpxConnectionPoint *);
	void breakEdge(GpxTempConnection *, QPointF, GpxEdge*);
  	void mergeEdge(GpxEdge *e, QList<GpxEdge*> &);
	void setSinkTo(Line *, GpxConnectionPoint*);
	void setSourceTo(Line *, GpxConnectionPoint*);
  	void setSelectedItems();
	enum Mode { None, InsertLine, MoveNode, MoveLine, ResizeItem, RubberBand };
  	Mode _mode;
  	GpxTempConnection *_tempConnection;
  	QGraphicsPolygonItem *_rubberBand;
  	GpxConnectionNode *_movingNode;
  	QPointF _rubberStart;
  	bool _mayBeDragging;
  	GpxBlock *_resizingBlock;
  	GpxEdge *_movingLine;
  	Qt::Corner _resizingCorner;
  	Coupled *_coupledData;
	QPointF startDrag;
  	QMap<Model*,GpxBlock*> _childs;
  	QMap<Point*,GpxConnectionNode*> _points;
  	QMap<Port*,GpxBlock*> _ports;
	bool changeDirection;
#ifdef UNDO
	QUndoStack *_undoStack;
#endif
	QPointF _mousePos,_lastClick;
  	bool override;
};
#endif
