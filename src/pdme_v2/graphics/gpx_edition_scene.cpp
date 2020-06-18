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
#include <iostream>
#include <sstream>
#include <math.h>

#include <QApplication>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>
#include <QGraphicsView>
#include <QTemporaryFile>
#include <QMessageBox>
#include <QMimeData>
#include <QDrag>
#include <QGraphicsPolygonItem>

#include <data/coupled.h>
#include <graphics/gpx_atomic.h>
#include <graphics/gpx_coupled.h>
#include <graphics/gpx_inport.h>
#include <graphics/gpx_outport.h>
#include <graphics/gpx_edition_scene.h>
#include <graphics/gpx_edge.h>
#include <graphics/gpx_connection_cross.h>
#include <graphics/gpx_connection_node.h>
#include <globals.h>
#include <commands.h>
#include "parser.h"

unsigned int OFFSET_Y=0;
unsigned int OFFSET_X=0;

using namespace std;
QPointF lastClick;

GpxEditionScene::GpxEditionScene(QObject * parent, Coupled *c) : QGraphicsScene(parent), _coupledData(c)
{
		changeDirection=false;
   _mode = None;
   _tempConnection = NULL;
   _rubberBand = NULL;
   _mayBeDragging = false;
   putStructure(c);
	setItemIndexMethod(NoIndex);
	#ifdef UNDO
	_undoStack = new QUndoStack(this);
	#endif
  override=false;
}

GpxEditionScene::GpxEditionScene(QObject * parent) : QGraphicsScene(parent), _coupledData(new Coupled())
{
		changeDirection=false;
   _mode = None;
   _tempConnection = NULL;
   _rubberBand = NULL;
	setItemIndexMethod(NoIndex);
	#ifdef UNDO
	_undoStack = new QUndoStack(this);
	#endif
  	override=false;
}


void GpxEditionScene::dragMoveEvent(QGraphicsSceneDragDropEvent * event)
{
  //QGraphicsScene::dragMoveEvent(event);
  event->setAccepted(true);
}

void GpxEditionScene::dropEvent(QGraphicsSceneDragDropEvent * event)
{
	OFFSET_X=0;
	OFFSET_Y=0;
    QString data = event->mimeData()->text();
    if (data.startsWith("Atomic"))
    {
      Atomic *a = getAtomic(data);
      if (a==NULL)
        return;
      clearSelection();
      a->graphic().setX(event->scenePos().x());
      a->graphic().setY(event->scenePos().y());
      putAtomic(a,true);
      emit modified();
   } else if (data.startsWith("Inport"))
    {
      Port *p = getPort(data);
      if (p==NULL)
        return;
      clearSelection();
      p->graphic().setX(event->scenePos().x());
      p->graphic().setY(event->scenePos().y());
      putInport(p,true);
      emit modified();
   } else if (data.startsWith("Outport"))
    {
      Port *p = getPort(data);
      if (p==NULL)
        return;
      clearSelection();
      p->graphic().setX(event->scenePos().x());
      p->graphic().setY(event->scenePos().y());
      putOutport(p,true);
      emit modified();
   } else if (data.startsWith("Coupled"))
    {
      Coupled *c = getCoupled(data);
      if (c==NULL)
        return;
      clearSelection();
      c->graphic().setX(event->scenePos().x());
      c->graphic().setY(event->scenePos().y());
      putCoupled(c,true);
      emit modified();
   } else if (data.startsWith("DRAG"))
    {
      data = data.mid(4);
			startDrag = event->scenePos() - startDrag;	
			OFFSET_X=startDrag.x();
			OFFSET_Y=startDrag.y();
      Coupled *c = getCoupled(data);
      if (c==NULL)
        return;
			clearSelection();
      putStructure(c,true,true);
      emit modified();
    }
 }

void GpxEditionScene::insert(QString data)
{
}

void GpxEditionScene::updateMouseCursor(QGraphicsSceneMouseEvent * mouseEvent)
{
  QList<QGraphicsItem *> it = items(mouseEvent->scenePos());
  QPointF pos = mouseEvent->scenePos();
  it.removeAll(_tempConnection);
  if (_mode==InsertLine) {
    if (_tempConnection!=NULL)
      _tempConnection->setCursor(Qt::ClosedHandCursor);
  }
}

void GpxEditionScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
  QList<QGraphicsItem *> it = items(mouseEvent->scenePos());
  QList<QGraphicsItem *> sel = selectedItems();
  QList<GpxBlock *> blocks;
  QList<GpxEdge *> edges;
  bool blockLine;
  QPointF pos = mouseEvent->scenePos();
  _mousePos = pos;
  switch (_mode) {
    case None:
      if (_mayBeDragging && fabs(pos.x()-startDrag.x())>3 && fabs(pos.y()-startDrag.y())>3 /*&& it.size()*/)
      { // Put a hysteresis for small movements
  			if (it.size() && it.first()->type()==GpxBlock::Type)
			  {
			    GpxBlock *block = qgraphicsitem_cast<GpxBlock*>(it.first());
					block->setSelected(true);
				}
        _mayBeDragging=false;
        QDrag *drag = new QDrag(mouseEvent->widget());  
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(QString("DRAG") + getSelection());
        drag->setMimeData(mimeData);
        drag->setPixmap(QString(":/images/copy.png"));
        drag->setHotSpot(QPoint(15, 30)); 
        drag->exec();
      }
      QGraphicsScene::mouseMoveEvent(mouseEvent);
      break;
    case InsertLine:
      Q_ASSERT(_tempConnection!=NULL);  
      {
        GpxTempConnection *newConnection;
        if (  it.size() && it.first()->type() == GpxConnectionPoint::Type)
        {
          GpxConnectionPoint *cp = qgraphicsitem_cast<GpxConnectionPoint*>(it.first());
          newConnection = new GpxTempConnection(_tempConnection-> getInitPoint(), cp->mapToScene(cp->getConnectionPoint()),_tempConnection->getInitPort(),changeDirection);
          if (!override) {
            override=true;
            QApplication::setOverrideCursor(Qt::CrossCursor);
          } else {
            QApplication::changeOverrideCursor(Qt::CrossCursor);
          }
        } else { 
          if (  it.size() && it.first()->type() == GpxEdge::Type)
          {
            if (!override) {
              override=true;
              QApplication::setOverrideCursor(Qt::CrossCursor);
            } else {
              QApplication::changeOverrideCursor(Qt::CrossCursor);
            }
          } else {
            if (override) {
              override=false;
              QApplication::restoreOverrideCursor ();
            }
          }
          QPointF p =  (mouseEvent->scenePos());
          int x=p.x();
          x-=x%GRID;
          int y=p.y();
          y-=y%GRID;
          newConnection = new GpxTempConnection(_tempConnection-> getInitPoint(), QPointF(x,y), _tempConnection->getInitPort(),changeDirection);
        }
        removeItem(_tempConnection);
        delete _tempConnection;
        _tempConnection = newConnection;
        addItem(_tempConnection);
      }
      break;
    case RubberBand:
      {
	  	
        QPainterPath qp;
        Q_ASSERT(_rubberBand!=NULL);  
        _rubberBand->setPolygon(QPolygonF (QRectF(_rubberStart, mouseEvent->scenePos()).  normalized()));
        qp.addPolygon(QPolygonF (QRectF(_rubberStart, mouseEvent->scenePos()).normalized())); 
        setSelectionArea(qp);
      }
      break;
    case ResizeItem:
      _resizingBlock->resizeTo(pos,_resizingCorner);
      emit modified();
      break;
		case MoveLine:
			_movingLine->moveTo(pos);
      emit modified();
      break;
    case MoveNode:
       QPointF p =  (mouseEvent->scenePos());
       int x=p.x();
       x-=x%GRID;
       int y=p.y();
       y-=y%GRID;
      _movingNode->setPos(x,y);
      _movingNode->updatePosition();
      emit modified();
      break;
  }
}

void GpxEditionScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
  QList<QGraphicsItem *> it = items(mouseEvent->scenePos());
  QPointF pos = mouseEvent->scenePos();
  lastClick = pos;
  _mousePos = pos;
  switch (_mode) {
    case None:
      if (mouseEvent->button() == Qt::LeftButton 
         && it.size() 
         && it.first()->type() == GpxConnectionPoint::Type)
      {   // Creates a new temporary line
         GpxConnectionPoint *p = qgraphicsitem_cast<GpxConnectionPoint *>(it.first());
         if (!p->isNode()) {
           _mode = InsertLine;
            _tempConnection = new GpxTempConnection(p->mapToScene(p->getConnectionPoint()), mouseEvent->scenePos(), p);
            addItem(_tempConnection);
        }
      }
      if (mouseEvent->button() == Qt::LeftButton 
         && !it.size()) 
      { // Starts a rubber band selection
        _rubberStart = mouseEvent->scenePos();
        _rubberBand = new QGraphicsPolygonItem(QPolygonF (QRectF (_rubberStart, QSizeF())), NULL/*, this*/);
        _rubberBand-> setPen(QPen (QBrush(Qt::SolidPattern), 1, Qt::DashLine));
        _mode=RubberBand;
      }
      if (mouseEvent->button() == Qt::LeftButton 
         && it.size() 
         && it.first()->type() == GpxBlock::Type)
      {   
        GpxBlock *block = qgraphicsitem_cast<GpxBlock*>(it.first());
        Qt::Corner c;
        if (block->isAtCorner(pos,c)) {
          _mode = ResizeItem;
          _resizingBlock = block;
          _resizingCorner =c;
	  	  #ifdef UNDO
		  	_undoStack->push(new ResizeCmd(this));
          #endif
		}
      }
      if (mouseEvent->button() == Qt::RightButton && !it.size()) {
        _lastClick = pos;
        emit contextMenu(NULL,mouseEvent->screenPos());
        return; 
      } else if (mouseEvent->button() == Qt::RightButton && it.size()) {
        _mayBeDragging = true;
		if(!it.first()->isSelected()){
			clearSelection();
			it.first()->setSelected(true);
		}
		startDrag = pos;
      }
      if (mouseEvent->button() == Qt::LeftButton && it.size() && it.first()->type()==GpxEdge::Type) {
	     			_mode = MoveLine;
					GpxEdge *e = qgraphicsitem_cast<GpxEdge*>(it.first());
					_movingLine = e;
					_movingLine->startMoving(pos);
					QList<QGraphicsItem*> sel = selectedItems();
					foreach(QGraphicsItem *i, sel){
						if(i != e)
							i->setSelected(false);
					}
			}
      if (mouseEvent->button() == Qt::LeftButton && it.size()>1) {
        foreach(QGraphicsItem *i,it)
        {
          if (i->type()==GpxConnectionPoint::Type)
          {
            GpxConnectionPoint *p=qgraphicsitem_cast<GpxConnectionPoint*>(i);
            if (p->isNode()) {
              GpxConnectionNode *n=qgraphicsitem_cast<GpxConnectionNode*>(p);
              _mode=MoveNode;
              _movingNode=n;
              break;
            }
          }
        }
    }
	#ifdef UNDO
    	if (mouseEvent->button() == Qt::LeftButton && selectedItems().size()) 
	 		_undoStack->push(new MoveCmd(this));
	#endif
    break;
    default: // shoudl not happen
      Q_ASSERT(false);
  }
  QGraphicsScene::mousePressEvent(mouseEvent);
}
 
void GpxEditionScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
  QList<QGraphicsItem *> it = items(mouseEvent->scenePos());
  QPointF pos = mouseEvent->scenePos();
  _lastClick = _mousePos = pos;
	OFFSET_X=0;
	OFFSET_Y=0;
  switch (_mode) {
    case None:
      if (_mayBeDragging && mouseEvent->button() == Qt::RightButton && it.size()) {
        _mayBeDragging=false;
        GpxBlock *block = qgraphicsitem_cast<GpxBlock*>(it.first());
        if (block && selectedItems().size()==0) {
          block->setSelected(true);
          emit contextMenu(block,mouseEvent->screenPos());
        } else if (selectedItems().size()==1) {
          GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(selectedItems().first());
          if (b==block)
            emit contextMenu(block,mouseEvent->screenPos());
        } else if (selectedItems().size()>1) {
          emit contextMenu(NULL,mouseEvent->screenPos());
        }
      } else if (!selectedItems().empty()) {
        foreach(QGraphicsItem *i,selectedItems())
        {
          if (i->type()==GpxConnectionPoint::Type)
          {
           GpxConnectionPoint *gcp = dynamic_cast<GpxConnectionPoint*>(i);
           	if (gcp->isNode()){
              GpxConnectionNode *gcn = dynamic_cast<GpxConnectionNode*>(gcp);
              gcn->updatePosition();
            }
          }
        }
	  	#ifdef UNDO
			_undoStack->push(new MoveCmd(this));
      	#endif
	  }
      break;
    case InsertLine:
      Q_ASSERT(_tempConnection!=NULL);  
      it.removeAll(_tempConnection);
      emit modified();
      if (!it.size()) { // Creating a cross
				createLineEndingInCross(_tempConnection,pos);
     	} else if (it.size() && it.first()->type()==GpxConnectionPoint::Type) {
        GpxConnectionPoint *cp = _tempConnection->getInitPort();
        GpxConnectionPoint *endPoint = qgraphicsitem_cast<GpxConnectionPoint*>(it.first());
        if (cp->isCross() && (endPoint->isInport() || endPoint->isOutport())) { 
					// Finishing a line from a cross 
					finishALineFromCross(_tempConnection, pos,endPoint);
        } else if (cp->isCross() && endPoint->isCross()) {
					// Merge two lines
					mergeLines(_tempConnection,pos,endPoint);
        } else if (!cp->isCross() && !cp->isNode() && endPoint->isCross()) {
           // From port to cross;
					fromPortToCross(_tempConnection,pos,endPoint);
        } else if (cp!=endPoint && !cp->isCross() && !cp->isNode() 
									&& !endPoint->isCross() && !endPoint->isNode()) {
						// New connection from in port to outport or viceverse
						createNormalConnection(_tempConnection,pos,endPoint);
         } 
      } else if (it.size() && it.first()->type()==GpxEdge::Type) {
				// Breaking a line
				breakEdge(_tempConnection,pos, qgraphicsitem_cast<GpxEdge*>(it.first()));
     }
      removeItem(_tempConnection);
      delete _tempConnection;
      _tempConnection = NULL;
      _mode=None;
      break;
    case RubberBand:
      Q_ASSERT(_rubberBand!=NULL);  
      removeItem(_rubberBand);
      delete _rubberBand; 
      _rubberBand = NULL;
      _mode=None;
	  setSelectedItems(); 
	  #ifdef UNDO
	  	if(selectedItems().count())
	  		_undoStack->push(new SelectCmd(this,_rubberStart));
      #endif
	  break;
    case ResizeItem:
	  #ifdef UNDO
	  	_undoStack->push(new ResizeCmd(this));
      #endif
      _resizingBlock->itemChange(QGraphicsItem::ItemPositionHasChanged,QVariant());
      _resizingBlock = NULL;
      _mode = None;
      break;
		case MoveLine:
			_movingLine->stopMoving();
      _movingLine = NULL;
      _mode = None;
	  #ifdef UNDO
	  	_undoStack->push(new MoveCmd(this));
      #endif
      break;
		case MoveNode:
      _movingNode = NULL;
      _mode = None;
      break;

  }
  QGraphicsScene::mouseReleaseEvent(mouseEvent);
  if (override) {
    override=false;
    QApplication::restoreOverrideCursor ();
  }
}

void GpxEditionScene::rotate()
{
  emit modified();
  QList<QGraphicsItem *> it = selectedItems();
  foreach(QGraphicsItem *gi,it) // First remove edges
  {
    if (gi->type()==GpxBlock::Type)
    {
      GpxBlock *block = qgraphicsitem_cast<GpxBlock*>(gi);
      block->rotate();
    }
  }
}

void GpxEditionScene::deleteSelection()
{
  emit modified();
  QList<QGraphicsItem *> it = selectedItems();
  QList<GpxEdge*> del;
  QList<GpxBlock *> blocks;
  foreach(QGraphicsItem *i,it)
  {
    if (i->type()==GpxTextBox::Type) {
      GpxTextBox *tb = dynamic_cast<GpxTextBox*>(i); 
      if (tb->parentItem()==NULL && tb->isSelected()) {
        vector<string> extra = _coupledData->getExtra();
        bool found=false;
        int j;
        for (j=0;j<extra.size();j++) {
          QString strExtra(extra[j].c_str());
          if (!strExtra.startsWith("Annotation")) continue;
          strExtra = strExtra.mid(11);
          strExtra.chop(1);
          strExtra.remove(1);
          QString an = strExtra.mid(0,strExtra.lastIndexOf("\""));
          an = an.replace("\\n","\n");
          an = an.replace("\\\"","\"");
          an = an.remove(0,1);
          if (tb->toPlainText()==an) {
            found=true;
            break;
          } 
  
        }
        if (found) {
          extra.erase(extra.begin()+j);
          _coupledData->setExtra(extra);
        }
        removeItem(i);
      }
    } else 
      qDebug() << "Ignoring item " << i << " type " << i->type();
  }
	
  foreach(QGraphicsItem *gi,it) 
  {
    if (gi->type()==GpxEdge::Type) {
    	del << qgraphicsitem_cast<GpxEdge*>(gi);
    }
    if (gi->type()==GpxBlock::Type) {
			blocks << qgraphicsitem_cast<GpxBlock*>(gi);
		}
  }
  qDebug() << "Cantidad de lineas: " << del.size();
  removeLines(del);// First remove edges
  QList<GpxEdge *> edges;
  foreach(GpxBlock *b,blocks)
  {
  	edges.clear();
    if (b->isInport()) {
      GpxInport *ip = qgraphicsitem_cast<GpxInport*>(b);
			edges = b->connectedLines();
			removeLines(edges);
      emit remove_Port(_coupledData,ip->portData());
			// Delete connection of father and if open update graphics
    } else if (b->isOutport()) {
      GpxOutport *ip = qgraphicsitem_cast<GpxOutport*>(b);
			edges = b->connectedLines();
			removeLines(edges);
      emit remove_Port(_coupledData,ip->portData());
		  // Delete connection of father and if open update graphics
    } else {
      if (b->isCoupled())
      {
        GpxCoupled *c=qgraphicsitem_cast<GpxCoupled*>(b);
        emit closeAllChildsOf(c->coupledData());
      }
      Model *m = b->model();
      _coupledData->removeChild(m);
  	  edges = b->connectedLines();
			removeLines(edges);
      delete m;
    }
  }
	foreach(GpxBlock *b,blocks)  {
		removeItem(b);
		delete b;
	}
  clearSelection();
  it=items();
  views().first()->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  _coupledData->updatePoints();
}

void GpxEditionScene::selectAll()
{
  QList<QGraphicsItem *> it = selectedItems();
  clearSelection();
  foreach(QGraphicsItem * i, items())
    i->setSelected(true);
}

Coupled *GpxEditionScene::getCoupled(QString data)
{
  QTemporaryFile f;
  f.open();
  f.write(data.toLatin1());
  QString fileName =  f.fileName();
  f.close();
  Coupled *c= parseFile(fileName);
  if (c!=NULL)
    c->setFather(_coupledData);
  return c;
}

Atomic *GpxEditionScene::getAtomic(QString data)
{
  QTemporaryFile f;
  f.open();
  data = "Coupled\n {\nType = root\n Name = atomic\n Ports = 0; 0 \nDescription =  \n \
  Graphic\n {\n Position = 0; 0\n Dimension = 555; 555\n Direction = Right\n Color = 15\n Icon = None\n Window = 4365; 3180; 6615; 5145\n }\n \
  Parameters\n {\n }\n System\n {\n " + data + " }\n  }";
  f.write(data.toLatin1());
  QString fileName =  f.fileName();
  f.close();
  Coupled *c=parseFile(fileName);
  Atomic *a = dynamic_cast<Atomic*>(c->child(0));
  a->setFather(_coupledData);
  return a;
}

Port *GpxEditionScene::getPort(QString data)
{
  Port p;
  QTemporaryFile f;
  f.open();
  bool inport = data.startsWith("Inport");
  data = "Coupled\n {\nType = root\n Name = atomic\n Ports = 0; 0 \nDescription =  \n \
  Graphic\n {\n Position = 0; 0\n Dimension = 555; 555\n Direction = Right\n Color = 15\n Icon = None\n Window = 4365; 3180; 6615; 5145\n }\n \
  Parameters\n {\n }\n System\n {\n " + data + " }\n  }";
  f.write(data.toLatin1());
  QString fileName =  f.fileName();
  f.close();
  Coupled *c=parseFile(fileName);
  assert(c!=NULL);
  if (inport)
    return c->inport(0);
  else 
    return c->outport(0); 
}

void GpxEditionScene::putStructure(Coupled *coupled, bool paste, bool draggin)
{
  int child;
  QList<Model*> childs;
  QList<Port*>  ports;
  QList<Point*>  points;
  if (paste || draggin)
    clearSelection();
  for(child=0; child<coupled->childCount(); child++)
  {
    Model *chld = coupled->child(child);
    if (chld->isAtomic()) {
        Atomic *a = dynamic_cast<Atomic*>(chld);
        putAtomic(a,paste,draggin);
        childs << a;
    } else {
        Coupled *c = dynamic_cast<Coupled*>(chld);
        putCoupled(c,paste,draggin);
        childs << c;
    }
  }
  for(int port=0; port<coupled->portCount(); port++)
  {
    Port *p =coupled->port(port);
    if (p->type()==Port::Inport)
      putInport(p,paste,draggin);
    else
      putOutport(p,paste,draggin);
    ports << p;
  }
  for(int point=0; point<coupled->pointCount(); point++)
	{
		Point *p = (coupled->pointAt(point));
    		points << p;
		putPoint(p,paste,draggin);
	}
  for(int line=0; line<coupled->lineCount(); line++)
	{
		Line *l = coupled->lineAt(line);
/*
		if ((paste || draggin) && l->sourceType() == Line::NODE)
		{
			vector<int> li = l->sources();
			li[0] = _coupledData->pointIndex(points.at(li[0]-1)) +1;
			l->setSources(li);
			cout << *l;
		}
		if ((paste || draggin) && l->sinkType() == Line::NODE)
		{
			vector<int> li = l->sinks();
			li[0] = _coupledData->pointIndex(points.at(li[0]-1)) +1;
			l->setSink(li);
			cout << *l;
		}
*/
		putConnection(l,paste,draggin,childs,ports,points);
	}
  /* Annotations */
  vector<string> extra = coupled->getExtra();
  for (int i=0;i<extra.size();i++) {
    QString strExtra(extra[i].c_str());
    qDebug() << "Found an extra " << strExtra;
    if (strExtra.startsWith("Annotation")) {
      GpxTextBox *tb = new GpxTextBox(_coupledData);
      int start= strExtra.indexOf("(");
      strExtra = strExtra.mid(start, strExtra.lastIndexOf(")")-start+1);
      strExtra.chop(1);
      strExtra.remove(0,2);
      QString an = strExtra.mid(0,strExtra.lastIndexOf("\""));
      QString pos = strExtra.mid(strExtra.lastIndexOf("\"")+2);
      QStringList ls = pos.split(",");
      an = an.replace("\\n","\n");
      an = an.replace("\\\"","\"");
      tb->setPlainText(an);
      addItem(tb);
      tb->setPos(ls.at(0).toInt()/TWIPS_TO_PIXEL,ls.at(1).toInt()/TWIPS_TO_PIXEL);
    }
  }
  

	//cout << _coupledData;
}

GpxCoupled *GpxEditionScene::putCoupled(Coupled *c, bool paste, bool draggin)
{
      if (paste || draggin) 
      {
        c->setName(findName(c->name().c_str()).toLatin1().constData());
        _coupledData->addChild(c);
        c->setFather(_coupledData);
        c->graphic().setX(c->graphic().x()+OFFSET_X);
        c->graphic().setY(c->graphic().y()+OFFSET_Y);
      }
      GpxCoupled *item = new GpxCoupled(this, c);
      if (paste || draggin)
        item->setSelected(true);
      addItem(item);
      item->setPos(c->graphic().x()+c->graphic().width()/2,c->graphic().y()+c->graphic().height()/2);
      item->updateTextPosition();
      connect(item,SIGNAL(parametersDialog(GpxBlock*)),this,SLOT(parameterDialog(GpxBlock*)));
      connect(item,SIGNAL(modified()),this,SLOT(on_modified()));
      _childs.insert(c,item);
    return item;
}

void GpxEditionScene::putOutport(Port *p, bool paste, bool draggin)
{
      if (paste || draggin) 
      {
        p->setName(findName(p->name().c_str()).toLatin1().constData());
        emit add_Port(_coupledData,p);
        p->graphic().setX(p->graphic().x()+OFFSET_X);
        p->graphic().setY(p->graphic().y()+OFFSET_Y);
      }
      GpxOutport *item = new GpxOutport(this, p);
      if (paste || draggin)
        item->setSelected(true);
      addItem(item);
      item->updateTextPosition();
      item->setPos(p->graphic().x()+p->graphic().width()/2,p->graphic().y());
      connect(item,SIGNAL(modified()),this,SLOT(on_modified()));
      _ports.insert(p,item);
}

void GpxEditionScene::putInport(Port *p, bool paste, bool draggin)
{
      if (paste || draggin) 
      {
        p->setName(findName(p->name().c_str()).toLatin1().constData());
        //_coupledData->addInport(p);
        emit add_Port(_coupledData,p);
		    p->graphic().setX(p->graphic().x()+OFFSET_X);
        p->graphic().setY(p->graphic().y()+OFFSET_Y);
      }
      GpxInport *item = new GpxInport(this, p);
      if (paste || draggin)
        item->setSelected(true);
      item->setPos(p->graphic().x()+p->graphic().width()/2,p->graphic().y());
      addItem(item);
      item->updateTextPosition();
      connect(item,SIGNAL(modified()),this,SLOT(on_modified()));
      _ports.insert(p,item);
}

void GpxEditionScene::putConnection(Line *l, bool paste, bool draggin, QList<Model*> childs, QList<Port*> ports, QList<Point*> points)
{
      //cout << l[0];
      if (paste || draggin) 
      {
        _coupledData->addLine(l);
				vector<int> x,y;
				for(unsigned i=0;i<l->pointCount();i++) {
					x.push_back(l->pointX(i)+OFFSET_X);
					y.push_back(l->pointY(i)+OFFSET_Y);
				}	
				l->setPointsXY(x,y);
        if (l->sourceType()==Line::COMPONENT) {
          Model *m=childs[l->sourceAt(0)-1];
          int child = _coupledData->childIndex(m);
          vector<int> li = l->sources();
          li[0] = child+1;
          l->setSources(li);
        }
        if (l->sinkType()==Line::COMPONENT) {
          Model *m=childs[l->sinkAt(0)-1];
          int child = _coupledData->childIndex(m);
          vector<int> li = l->sinks();
          li[0] = child+1;
          l->setSink(li);
        }
        if (l->sourceType()==Line::PORT) {
          Port *p=ports[l->sourceAt(0)-1];
          int port = _coupledData->portIndex(p);
          vector<int> li = l->sources();
          li[0] = port+1;
          l->setSources(li);
        }
        if (l->sinkType()==Line::PORT) {
          Port *p=ports[l->sinkAt(0)-1];
          int port = _coupledData->portIndex(p);
          vector<int> li = l->sinks();
          li[0] = port+1;
          l->setSink(li);
        }
        if (l->sourceType()==Line::NODE) {
          Point *p=points[l->sourceAt(0)-1];
          int point = _coupledData->pointIndex(p);
          vector<int> li = l->sources();
          li[0] = point+1;
          l->setSources(li);
        }
        if (l->sinkType()==Line::NODE) {
          Point *p=points[l->sinkAt(0)-1];
          int point = _coupledData->pointIndex(p);
          vector<int> li = l->sinks();
          li[0] = point+1;
          l->setSink(li);
        }
 
      }
      GpxConnectionPoint *src=NULL,*dst=NULL;
      switch (l->sourceType()) 
      {
        case Line::PORT:
          {
            //cout << *l;
            const int port = l->sourceAt(0)-1;
            const bool inport = l->sourceAt(2)==0;
            GpxBlock *b=_ports[_coupledData->port(port)];
						if (inport) 
							src=b->outport(0);
						else
							src=b->inport(0);
          }
        break;
        case Line::NODE:
          {
            const int point = l->sourceAt(0)-1;
            src=_points[_coupledData->pointAt(point)];
          }
        break;
        case Line::CROSS:
          {
          Graphic::Direction d; 
          if (l->pointX(0)==l->pointX(1))
            d=Graphic::DOWN;
          else
            d=Graphic::RIGHT;
          GpxConnectionCross *item = new GpxConnectionCross(d);
          addItem(item);
          item->setPos(l->pointX(0),l->pointY(0));
          src=item;
          }
        break;
        case Line::COMPONENT:
          const int child = l->sourceAt(0)-1;
          const int port= l->sourceAt(1)-1;
          const bool inport = l->sourceAt(2)==-1;
          GpxBlock *b=_childs[_coupledData->child(child)];
          if (inport)
            src=b->inport(port);
          else
            src=b->outport(port);
        break;
      }
      switch (l->sinkType()) 
      {
        case Line::PORT:
          {
            const int port = l->sinkAt(0)-1;
            GpxBlock *b=_ports[_coupledData->port(port)];
						if (b->isInport())
							dst=b->outport(0);
						else
							dst=b->inport(0);
 
            //GpxBlock *b=_ports[_coupledData->portAt(port)];
          }
        break;
        case Line::NODE:
          {
            const int point = l->sinkAt(0)-1;
            dst=_points[_coupledData->pointAt(point)];
          }
        break;
        case Line::CROSS:
          {
          Graphic::Direction d; 
          if (l->pointX(l->pointCount()-1)==l->pointX(l->pointCount()-2))
            d=Graphic::DOWN;
          else
            d=Graphic::RIGHT;
          GpxConnectionCross *item = new GpxConnectionCross(d);
          addItem(item);
          item->setPos(l->pointX(l->pointCount()-1),l->pointY(l->pointCount()-1));
          dst=item;
          }
 
        break;
        case Line::COMPONENT:
          const int child = l->sinkAt(0)-1;
          const int port= l->sinkAt(1)-1;
          const bool inport = l->sinkAt(2)==-1;
          GpxBlock *b=_childs[_coupledData->child(child)];
          if (inport)
            dst=b->inport(port);
          else
            dst=b->outport(port);
        break;
      }
      Q_ASSERT(src!=NULL);
      Q_ASSERT(dst!=NULL);
      GpxEdge *item = new GpxEdge(src,dst,l);
      if (paste || draggin)
      {
	      if (src->isCross())
      	  src->setSelected(true);
      	if (dst->isCross())
      	  dst->setSelected(true);
        item->setSelected(true);
      }
      addItem(item);
}


void GpxEditionScene::putPoint(Point *p, bool paste, bool draggin)
{
      if (paste || draggin) 
      {
        _coupledData->addPoint(p);
        p->graphic().setX(p->graphic().x()+OFFSET_X);
        p->graphic().setY(p->graphic().y()+OFFSET_Y);
      }
      GpxConnectionNode *item = new GpxConnectionNode(this, p);
      addItem(item);
      if (paste || draggin)
        item->setSelected(true);
      item->setPos(p->graphic().x(),p->graphic().y());
      _points.insert(p,item);
}

void GpxEditionScene::putAtomic(Atomic *a, bool paste, bool draggin)
{
      if (paste || draggin) 
      {
        a->setName(qPrintable(findName(a->name().c_str())));
        _coupledData->addChild(a);
        a->setFather(_coupledData);
        a->graphic().setX(a->graphic().x()+OFFSET_X);
        a->graphic().setY(a->graphic().y()+OFFSET_Y);
      }
      GpxAtomic *item = new GpxAtomic(this, a);
      if (paste || draggin)
        item->setSelected(true);
      addItem(item);
      item->setPos(a->graphic().x()+a->graphic().width()/2,a->graphic().y()+a->graphic().height()/2);
      item->updateTextPosition();
      //item->setPos(a->graphic().x(),a->graphic().y());
      connect(item,SIGNAL(parametersDialog(GpxBlock*)),this,SLOT(parameterDialog(GpxBlock*)));
      connect(item,SIGNAL(modified()),this,SLOT(on_modified()));
      _childs.insert(a,item);
}

bool GpxEditionScene::nameExists(QString propossedName)
{
  int  chld,port;
  for (chld=0;chld<_coupledData->childCount();chld++)
  {
    Model *m = _coupledData->child(chld);
    if (propossedName == QString(m->name().c_str())) {
      return true;
    }
  }
  for (port=0;port<_coupledData->inportCount();port++)
  {
    if (QString(_coupledData->inport(port)->name().c_str())==propossedName) {
      return true;
    }
  }
  for (port=0;port<_coupledData->outportCount();port++)
  {
    if (QString(_coupledData->outport(port)->name().c_str())==propossedName) {
      return true;
    }
  }
  return false;
}

bool GpxEditionScene::canUseName(QString proposedName, GpxBlock*b)
{
  QList<QGraphicsItem *> it=items();
  foreach(QGraphicsItem *i, it)
  {
    if (i->type()==GpxBlock::Type) 
    {
      GpxBlock *bi = qgraphicsitem_cast<GpxBlock*>(i);
      if (bi==b)
        continue;
      if (proposedName==bi->name())
        return false;
    }
  }
  return true;
}

void GpxEditionScene::changeBlockName(GpxBlock* b)
{
  emit setBlockName(b);
} 

QString GpxEditionScene::findName(QString propossedName)
{
  int number=0;
  if (!propossedName.at(propossedName.length()-1).isDigit())
    propossedName = propossedName + QString("0");
  if (!nameExists(propossedName))
    return propossedName;
  do {
    // Find number
    int p=propossedName.length()-1;
    while (propossedName.at(p).isDigit() && p>0)
      p--;
    number = propossedName.mid(p+1).toInt()+1;
    propossedName = propossedName.left(p+1) + QString("%1").arg(number);
  } while(nameExists(propossedName));
  return propossedName;
}

void GpxEditionScene::parameterDialog(GpxBlock *b)
{
  emit modified();
  emit showParameterDialog(b);
}

void GpxEditionScene::removePort(Coupled *c, Port *p)
{
  QList<QGraphicsItem *> it=items();
  foreach(QGraphicsItem *i, it)
  {
    if (i->type()==GpxBlock::Type)
    {
      GpxBlock *b=qgraphicsitem_cast<GpxBlock*>(i);
      if (b->isCoupled())
      {
        GpxCoupled *ic = qgraphicsitem_cast<GpxCoupled*>(b);
        if (ic->coupledData() == c)
        {
          ic->removePort(p);
          break;
        }
      }
    }
  }
}

void GpxEditionScene::addPort(Coupled *c, Port *p)
{
  QList<QGraphicsItem *> it=items();
  foreach(QGraphicsItem *i, it)
  {
    if (i->type()==GpxBlock::Type)
    {
      GpxBlock *b=qgraphicsitem_cast<GpxBlock*>(i);
      if (b->isCoupled())
      {
        GpxCoupled *ic = qgraphicsitem_cast<GpxCoupled*>(b);
        if (ic->coupledData() == c)
        {
          ic->addPort(p);
          break;
        }
      }
    }
  }
}

void GpxEditionScene::on_modified()
{
  emit modified();
}

QString GpxEditionScene::getSelection()
{
  OFFSET_X=0;
  OFFSET_Y=0;
  Coupled c;
	QList<QGraphicsItem *> items = selectedItems();
  QMap<Point*,Point*> newNodes;
  QMap<Port*,Port*> newPorts;
	foreach (QGraphicsItem *i, items)
	{
		if (i->type()==GpxBlock::Type){
			GpxBlock *b=qgraphicsitem_cast<GpxBlock*>(i);
			if (b->isAtomic()) {
				GpxAtomic *ga = qgraphicsitem_cast<GpxAtomic*>(b);
        ga->atomicData()->setPriority(_coupledData->childIndex(ga->atomicData())); 
				c.addChild(ga->atomicData());
			} else if (b->isCoupled()) {
				GpxCoupled *gc = qgraphicsitem_cast<GpxCoupled*>(b);
        gc->coupledData()->setPriority(_coupledData->childIndex(gc->coupledData())); 
				c.addChild(gc->coupledData());
			} else if (b->isInport()) {
				GpxInport *p = qgraphicsitem_cast<GpxInport*>(b);
				// TODO: Temporal, it works, but I don't konw 
				// if this is the best way to fix this. Same fix applied 
				// with the outports.
        Port *port = new Port(*p->portData());
        newPorts.insert(p->portData(),port);
				c.addInport(port);
			} else {
				GpxOutport *p = qgraphicsitem_cast<GpxOutport*>(b);
        Port *port = new Port(*p->portData());
        newPorts.insert(p->portData(),port);
				c.addOutport(port);
			}
		}
    c.sortChilds();
    
		if (i->type()==GpxConnectionPoint::Type){
      GpxConnectionPoint *gp = qgraphicsitem_cast<GpxConnectionPoint*>(i);
      if (gp->isNode()) {
        GpxConnectionNode *node = qgraphicsitem_cast<GpxConnectionNode*>(i);
        Point *point = new Point(*node->dataPoint());
        newNodes.insert(node->dataPoint(),point);
        point->clearLines();
        c.addPoint(point);
      }
    }
	}
	// Put points in the structure
	foreach (QGraphicsItem *i, items)
	{
		if (i->type()==GpxEdge::Type){
			GpxEdge *e=qgraphicsitem_cast<GpxEdge*>(i);
			GpxConnectionPoint *src=e->sourcePort();
			GpxConnectionPoint *dst=e->destPort();
			Line *l=new Line(*e->dataLine()); // Copy the line
			if (l->sourceType()==Line::COMPONENT) 
			{
				GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(src->parentItem());
				if (!b->isSelected()) 
					continue;
				vector<int> li = l->sources();
				li[0]=c.childIndex(b->model())+1;
				l->setSources(li);
			}
			if (l->sinkType()==Line::COMPONENT) 
			{
				GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(dst->parentItem());
				if (!b->isSelected()) 
					continue;
				vector<int> li = l->sinks();
				li[0]=c.childIndex(b->model())+1;
				l->setSink(li);
			}
      if (l->sourceType()==Line::PORT)
      {
				GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(src->parentItem());
				if (!b->isSelected()) 
					continue;
        int port;
        if (b->isInport()) {
          GpxInport *p = qgraphicsitem_cast<GpxInport*>(b);
          port = c.portIndex(newPorts[p->portData()])+1;
        } else {
          GpxOutport *p = qgraphicsitem_cast<GpxOutport*>(b);
          port = c.portIndex(newPorts[p->portData()])+1;
        }
				vector<int> li = l->sources();
        li[0] = port;
				l->setSources(li);
      }
      if (l->sinkType()==Line::PORT)
      {
				GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(dst->parentItem());
				if (!b->isSelected()) 
					continue;
        int port;
        if (b->isInport()) {
          GpxInport *p = qgraphicsitem_cast<GpxInport*>(b);
          port = c.portIndex(newPorts[p->portData()])+1;
        } else {
          GpxOutport *p = qgraphicsitem_cast<GpxOutport*>(b);
          port = c.portIndex(newPorts[p->portData()])+1;
        }
				vector<int> li = l->sinks();
        li[0] = port;
				l->setSink(li);
      }
      if (l->sourceType()==Line::NODE)
      {
				GpxConnectionNode *node = qgraphicsitem_cast<GpxConnectionNode*>(src);
				if (!node->isSelected()) {
					continue;
        }
				vector<int> li = l->sources();
        li[0] = c.pointIndex(newNodes[node->dataPoint()])+1;
        assert(li[0]>0 && li[0]<newNodes.size()+1);
				l->setSources(li);
      }
	    if (l->sinkType()==Line::NODE)
      {
				GpxConnectionNode *node = qgraphicsitem_cast<GpxConnectionNode*>(dst);
				if (!node->isSelected()) {
					continue;
        }
				vector<int> li = l->sinks();
        li[0] = c.pointIndex(newNodes[node->dataPoint()])+1;
        assert(li[0]>0 && li[0]<newNodes.size()+1);
				l->setSink(li);
      }
			c.addLine(l);
		}
	}
  if (c.childCount()) 
  {
    int x = (c.child(0)->isAtomic() ? dynamic_cast<Atomic*>(c.child(0))->graphic().x() : dynamic_cast<Coupled*>(c.child(0))->graphic().x());
    int y = (c.child(0)->isAtomic() ? dynamic_cast<Atomic*>(c.child(0))->graphic().y() : dynamic_cast<Coupled*>(c.child(0))->graphic().y());
    c.graphic().setX(x);
    c.graphic().setY(y);
  }
  c.graphic().setIcon("%basicelements%coupled.svg");
  c.graphic().setWidth(675/TWIPS_TO_PIXEL);
  c.graphic().setHeight(720/TWIPS_TO_PIXEL);
  c.updatePoints(false);
  c.simplifyLines();
  c.updatePoints();
	ostringstream stream;
  stream << c;
	QString ret = stream.str().c_str();
  // Delete lines, point and ports
  for (int i=0;i<c.inportCount();i++)
    delete c.inport(i);
  for (int i=0;i<c.outportCount();i++)
    delete c.outport(i);
  for (int i=0;i<c.pointCount();i++)
    delete c.pointAt(i);
  for (int i=0;i<c.lineCount();i++)
    delete c.lineAt(i);
  c.clearModel();
  return ret;
}

void GpxEditionScene::paste(QString clip)
{
  static QPointF lclick=_lastClick;
  Coupled *c = getCoupled(clip);
  if (c!=NULL) {
    if (lclick == _lastClick) {
      OFFSET_X+=20;
      OFFSET_Y+=20;
    } else {
      OFFSET_X=_lastClick.x()-c->graphic().x();
      OFFSET_Y=_lastClick.y()-c->graphic().y();
    } 
    //cout << c[0];
    putStructure(c,true);
    lclick = _lastClick;
  }
}

void GpxEditionScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
  QList<QGraphicsItem *> it = items(mouseEvent->scenePos());
  it.removeAll(_tempConnection);
  if (!it.size())
	{
		emit modelParameter(coupledData());
	} else if (it.first()->type()==GpxBlock::Type) {
    GpxBlock *block = qgraphicsitem_cast<GpxBlock*>(it.first());
    parameterDialog(block);  
	} else {
	  QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	}
}

void GpxEditionScene::setSourceTo(Line *l, GpxConnectionPoint *p)
{
	vector<int> li;
	if (p->isInport()) {
		GpxBlock *b = qgraphicsitem_cast<GpxBlock*> (p->parentItem());
   	if (b->isAtomic() || b->isCoupled()) 
   	{
   		l->setSourceType(Line::COMPONENT);
     	int child = _coupledData->childIndex(b->model())+1; 
     	int inport = b->inportIndex(p)+1;
     	li.push_back(child);
     	li.push_back(inport);
     	li.push_back(-1);
    } else if (b->isInport()) { // Inport
   		l->setSourceType(Line::PORT);
			GpxInport *p = qgraphicsitem_cast<GpxInport*>(b);
			int port= _coupledData->portIndex(p->portData())+1;
     	li.push_back(port);
     	li.push_back(1);
     	li.push_back(0);
		} else { // Outport
			GpxOutport *p = qgraphicsitem_cast<GpxOutport*>(b);
   		l->setSourceType(Line::PORT);
			int port= _coupledData->portIndex(p->portData())+1;
     	li.push_back(port);
     	li.push_back(0);
     	li.push_back(-1);
		}
  } else if (p->isOutport()) {
  	GpxBlock *b = qgraphicsitem_cast<GpxBlock*> (p->parentItem());
    if (b->isAtomic() || b->isCoupled()) 
    {
    	l->setSourceType(Line::COMPONENT);
     	int child = _coupledData->childIndex(b->model())+1; 
     	int outport = b->outportIndex(p)+1;
 			li.push_back(child);
     	li.push_back(outport);
     	li.push_back(0);
  	} else if (b->isInport()) { // Inport
   		l->setSourceType(Line::PORT);
			GpxInport *p = qgraphicsitem_cast<GpxInport*>(b);
			int port= _coupledData->portIndex(p->portData())+1;
     	li.push_back(port);
     	li.push_back(1);
     	li.push_back(0);
		} else { // Outport
   		l->setSourceType(Line::PORT);
			GpxOutport *p = qgraphicsitem_cast<GpxOutport*>(b);
			int port= _coupledData->portIndex(p->portData())+1;
     	li.push_back(port);
     	li.push_back(0);
     	li.push_back(-1);
		}

 	} else if (p->isNode()) {
   		l->setSourceType(Line::NODE);
			GpxConnectionNode *node = qgraphicsitem_cast<GpxConnectionNode*>(p);
			int point=_coupledData->pointIndex(node->dataPoint())+1;
     	li.push_back(point);
     	li.push_back(-1);
     	li.push_back(0);

	}
 	l->setSources(li);
}

void GpxEditionScene::setSinkTo(Line *l, GpxConnectionPoint *p)
{
	vector<int> li;
	if (p->isCross()) {
 		l->setSinkType(Line::CROSS);
 		li.push_back(0);
   	li.push_back(0);
   	li.push_back(0);
	} else if (p->isInport()) {
		GpxBlock *b = qgraphicsitem_cast<GpxBlock*> (p->parentItem());
   	if (b->isAtomic() || b->isCoupled()) 
   	{
   		l->setSinkType(Line::COMPONENT);
     	int child = _coupledData->childIndex(b->model())+1; 
     	int inport = b->inportIndex(p)+1;
     	li.push_back(child);
     	li.push_back(inport);
     	li.push_back(-1);
     } else {
				l->setSinkType(Line::PORT);
				GpxOutport *o = qgraphicsitem_cast<GpxOutport*>(b);
     		int port = _coupledData->portIndex(o->portData())+1; 
     		li.push_back(port);
     		li.push_back(1);
     		li.push_back(-1);
		 }			
  } else if (p->isOutport()) {
  	GpxBlock *b = qgraphicsitem_cast<GpxBlock*> (p->parentItem());
    if (b->isAtomic() || b->isCoupled()) 
    {
    	l->setSinkType(Line::COMPONENT);
     	int child = _coupledData->childIndex(b->model())+1; 
     	int outport = b->outportIndex(p)+1;
 			li.push_back(child);
     	li.push_back(outport);
     	li.push_back(0);
		} else {
				l->setSinkType(Line::PORT);
				GpxInport *o = qgraphicsitem_cast<GpxInport*>(b);
     		int port = _coupledData->portIndex(o->portData())+1; 
     		li.push_back(port);
     		li.push_back(1);
     		li.push_back(0);
		}
 	} else if (p->isNode()) {
   		l->setSinkType(Line::NODE);
			GpxConnectionNode *node = qgraphicsitem_cast<GpxConnectionNode*>(p);
			int point=_coupledData->pointIndex(node->dataPoint())+1;
     	li.push_back(point);
     	li.push_back(-1);
     	li.push_back(0);
	}
	l->setSink(li);
}

void	GpxEditionScene::keyPressEvent ( QKeyEvent * keyEvent )
{
	if (_mode==InsertLine &&  keyEvent->modifiers() & Qt::ControlModifier)	{
		changeDirection=true;
	}
}

void	GpxEditionScene::keyReleaseEvent ( QKeyEvent * keyEvent )
{
	if (_mode==InsertLine && changeDirection) {
		changeDirection=false;
	}

}

void GpxEditionScene::mergeEdge(GpxEdge *e, QList<GpxEdge*> &del)
{
  GpxConnectionNode *node=NULL,*node2=NULL;
	GpxEdge *edgedel;
  if (e->destPort()->isNode()) {
      node = qgraphicsitem_cast<GpxConnectionNode*>(e->destPort());
			e->destPort()->removeEdge(e);
      removeItem(node);
			GpxEdge *e1 = node->edgeAt(0);
			GpxEdge *e2 = node->edgeAt(1);
      QPolygonF p = e1->polygon();
      bool endsInNode=true;
      if (e1->sourcePort()==node) {
        endsInNode=false;
        QPolygonF temp;
        for(int i=0;i<p.size();i++) // Reverse the polygon
          temp.append(p.at(p.size()-1-i));
        p=temp;
      }
      QPolygonF p2 = e2->polygon();
      GpxConnectionPoint *newEnd;
      if (e2->sourcePort()!=node) { // e2 ends in the node to be removed
        newEnd=e2->sourcePort();
        QPolygonF temp;
        for(int i=0;i<p2.size();i++) // Reverse the polygon
          temp.append(p2.at(p2.size()-1-i));
        p2=temp;
      } else {
        newEnd=e2->destPort();
      }
      _coupledData->removePoint(node->dataPoint());
      newEnd->removeEdge(e2);
      newEnd->addEdge(e1);
      if (newEnd->isNode()) 
      {
        (qgraphicsitem_cast<GpxConnectionNode*>(newEnd))->dataPoint()->removeLine(_coupledData->lineIndex(e2->dataLine())+1);
        (qgraphicsitem_cast<GpxConnectionNode*>(newEnd))->dataPoint()->addLine(_coupledData->lineIndex(e1->dataLine())+1);
      }
      _coupledData->removeLine(e2->dataLine());
      if (endsInNode)
      {
        e1->setDestPort(newEnd);
        setSinkTo(e1->dataLine(),newEnd);
      } else {
        e1->setSourcePort(newEnd);
        setSourceTo(e1->dataLine(),newEnd);
      }
      p = p << p2;
      if (endsInNode==false)
      { // Reverse the result polygon
        QPolygonF temp;
        for(int i=0;i<p.size();i++) // Reverse the polygon
          temp.append(p.at(p.size()-1-i));
        p=temp;
      }
			del.removeAll(e2);
			del.removeAll(e);
 
      e2->destPort()->removeEdge(e2);
      e2->sourcePort()->removeEdge(e2);
      e2->setDestPort(NULL);
      e2->setSourcePort(NULL);
      node->clearEdges();
      removeItem(e2);
			invalidate(e2->boundingRect());
			update();
			edgedel = e2;
     	e1->setPolygon(p);
  }
  if (e->sourcePort()->isNode()) {
      node2 = qgraphicsitem_cast<GpxConnectionNode*>(e->sourcePort());
			e->sourcePort()->removeEdge(e);
			GpxEdge *e1 = e->sourcePort()->edgeAt(0);
			GpxEdge *e2 = e->sourcePort()->edgeAt(1);
      QPolygonF p = e1->polygon();
      bool endsInNode=true;
      if (e1->sourcePort()==node2) {
        endsInNode=false;
        QPolygonF temp;
        for(int i=0;i<p.size();i++) // Reverse the polygon
          temp.append(p.at(p.size()-1-i));
        p=temp;
      }
      QPolygonF p2 = e2->polygon();
      GpxConnectionPoint *newEnd;
      if (e2->sourcePort()!=node2) { // e2 ends in the node to be removed
        newEnd=e2->sourcePort();
        QPolygonF temp;
        for(int i=0;i<p2.size();i++) // Reverse the polygon
          temp.append(p2.at(p2.size()-1-i));
        p2=temp;
      } else {
        newEnd=e2->destPort();
      }
      e2->sourcePort()->removeEdge(e2);
      e2->destPort()->removeEdge(e2);
      e2->setDestPort(NULL);
      e2->setSourcePort(NULL);
      p = p << p2;
      if (endsInNode)
      {
        e1->setDestPort(newEnd);
        setSinkTo(e1->dataLine(),newEnd);
      } else {
        e1->setSourcePort(newEnd);
        setSourceTo(e1->dataLine(),newEnd);
      }
      newEnd->addEdge(e1);
      if (newEnd->isNode()) 
      {
        (qgraphicsitem_cast<GpxConnectionNode*>(newEnd))->dataPoint()->removeLine(_coupledData->lineIndex(e2->dataLine())+1);
        (qgraphicsitem_cast<GpxConnectionNode*>(newEnd))->dataPoint()->addLine(_coupledData->lineIndex(e1->dataLine())+1);
      }
      if (endsInNode==false)
      { // Reverse the result polygon
        QPolygonF temp;
        for(int i=0;i<p.size();i++) // Reverse the polygon
          temp.append(p.at(p.size()-1-i));
        p=temp;
      }
      e1->setPolygon(p);
      _coupledData->removeLine(e2->dataLine());
      node2->clearEdges();
      removeItem(node2);
      removeItem(e2);
			invalidate(e2->boundingRect());
			update();
			del.removeAll(e2);
			del.removeAll(e);
			edgedel = e2;
      _coupledData->removePoint(node2->dataPoint());
  }
	removeItem(e);
	invalidate(e->boundingRect());
	update();
	e->setSelected(false);
	e->sourcePort()->removeEdge(e);
	e->destPort()->removeEdge(e);
	if (e->sourcePort()->isCross())
	{
		removeItem(e->sourcePort());
		delete e->sourcePort();
	}
	if (e->destPort()->isCross())
	{
		removeItem(e->destPort());
		delete e->destPort();
	}
	
  if (node)
    delete node;
  if (node2)
    delete node2;
	//delete edgedel;
	_coupledData->updatePoints();
  qDebug() << e->polygon();
}



void GpxEditionScene::removeLines(QList<GpxEdge*> del) 
{
	  while(del.size())  {
		GpxEdge *e = del.first();
		Line *ln = e->dataLine();
    _coupledData->removeLine(ln);
		if ( e->destPort()->isNode() || e->sourcePort()->isNode()) {
      		mergeEdge(e,del);
		} else {
			del.removeAll(e);
		      removeItem(e);
			invalidate(e->boundingRect());
    		  Line *l=e->dataLine();
      delete l;
			update();
      delete e;
    }
	}
}

void GpxEditionScene::createLineEndingInCross(GpxTempConnection *tempConnection, QPointF pos)
{
	GpxConnectionPoint *cp = tempConnection->getInitPort();
	if (cp->isNode()) 
		return;
 	if (cp->isCross())
 	{ // Extending a line
		pos.rx() = (int)pos.x();
		pos.rx() -= (int)pos.x() % GRID;
		pos.ry() = (int)pos.y();
		pos.ry() -= (int)pos.y() % GRID;
		int x= tempConnection->getMiddlePoint().x();
		x-=x % GRID;
		int y= tempConnection->getMiddlePoint().y();
		y-= y % GRID;
    GpxEdge *e=cp->edgeAt(0);
    e->addPoint(QPointF(x,y),false);
    cp->setPos(pos);
    cp->setDirection(e->findDirection(e->polygon().last(), e->polygon().at(e->polygon().size()-2)));
	} else { 
		// Creating a new line ending on a cross
 		GpxConnectionCross *cross = new GpxConnectionCross(Graphic::DOWN);
		pos.rx() = (int)pos.x();
		pos.rx() -= (int)pos.x() % GRID;
		pos.ry() = (int)pos.y();
		pos.ry() -= (int)pos.y() % GRID;
		int x = tempConnection->getMiddlePoint().x();
		x-=x%GRID;
   	int y = tempConnection->getMiddlePoint().y();
		y-=y%GRID;
 		cross->setPos(pos);
		addItem(cross);
		Line *l = new Line();
		setSinkTo(l,cross);
		QPointF p=cp->parentItem()->mapToScene(cp->pos());
    l->addPoint(p.x(),p.y());
    l->addPoint(x,y);
    l->addPoint(pos.x(),pos.y());
		setSourceTo(l,cp);
    GpxEdge *edge = new GpxEdge(cp,cross,l);
    edge->addPort(cp);
    _coupledData->addLine(l);
    addItem(edge);
	}
}

void GpxEditionScene::finishALineFromCross(GpxTempConnection *tempConnection, QPointF pos, GpxConnectionPoint *endPoint)
{
	GpxConnectionPoint *cp = tempConnection->getInitPort();
	GpxEdge *e = cp->edgeAt(0);
	QPolygonF p = e->polygon();
	p << tempConnection->getMiddlePoint();
 	p << endPoint->getSceneConnectionPoint();
	e->setDestPort(endPoint);
	e->setPolygon(p);
	endPoint->addEdge(e);
	cp->removeEdge(e);
  e->addPort(endPoint);
	setSinkTo(e->dataLine(),endPoint);
	removeItem(cp);
	delete cp;
}


void GpxEditionScene::mergeLines(GpxTempConnection *tempConnection,QPointF pos,GpxConnectionPoint *endPoint)
{
	GpxConnectionPoint *cp = tempConnection->getInitPort();
	GpxEdge *e = cp->edgeAt(0);
	Line *l=endPoint->edgeAt(0)->dataLine();
	if (cp!=endPoint) {
    GpxEdge *r = endPoint->edgeAt(0);
    e->addPorts(r->ports());
		e->mergeWith(endPoint->edgeAt(0),tempConnection->getMiddlePoint());
		setSinkTo(e->dataLine(),e->destPort());
		// Merge lines and check if source/dest is a node add remove extrems
		_coupledData->removeLine(l);
	}
}

void GpxEditionScene::fromPortToCross( GpxTempConnection *tempConnection, QPointF pos, GpxConnectionPoint *endPoint)
{
	GpxConnectionPoint *cp = tempConnection->getInitPort();
	GpxEdge *e = endPoint->edgeAt(0);
 	QPolygonF p = e->polygon();
	p << tempConnection->getMiddlePoint();
  p << tempConnection->getInitPort()->getSceneConnectionPoint();
  e->setDestPort(cp);
  e->setPolygon(p);
  cp->addEdge(e);
  e->addPort(cp);
  endPoint->removeEdge(e);
  Line *l = e->dataLine();
	setSinkTo(l,cp);
  removeItem(endPoint);
  delete endPoint;
}


void GpxEditionScene::createNormalConnection(GpxTempConnection *tempConnection, QPointF pos, GpxConnectionPoint *endPoint)
{
	GpxConnectionPoint *cp = tempConnection->getInitPort();
	if (cp->isInport()==endPoint->isInport())
		return;
  Line *l = new Line();
	QPointF p=cp->getSceneConnectionPoint();
	l->addPoint(p.x(),p.y());
  l->addPoint(tempConnection->getMiddlePoint().x(),tempConnection->getMiddlePoint().y());
	p=endPoint->getSceneConnectionPoint();
  l->addPoint(p.x(),p.y());
	setSourceTo(l,cp);
	setSinkTo(l,endPoint);
	GpxEdge *edge = new GpxEdge(tempConnection->getInitPort(),endPoint,l);
  edge->addPort(cp);
  edge->addPort(endPoint);
	_coupledData->addLine(l);
  addItem(edge);
}

void GpxEditionScene::breakEdge(GpxTempConnection *tempConnection, QPointF pos,  GpxEdge *e)
{
	GpxConnectionPoint *cp = tempConnection->getInitPort();
  if (cp->edgeCount()) {
    QList<GpxConnectionPoint*> l = cp->edgeAt(0)->ports();
    foreach(GpxConnectionPoint *p,l)
      if (e->isConnectedToPort(p))
        return;
  }
	Point *mp=new Point();
    GpxConnectionNode *n = new GpxConnectionNode(this,mp);
	QPointF pnode = e->breakAt(pos);
	int x= pnode.x();
	int y= pnode.y();
	n->setPos(x,y);
	_coupledData->addPoint(mp);
	addItem(n);
	QPolygonF poly = e->polygon(),p1,p2;
	bool found=false;
	for (int i=0;i<poly.size()-1;i++) {
		QPointF init = poly.at(i), end = poly.at(i + 1);
		QRectF rectangle;
		if (init.x() == end.x())
		  rectangle = QRect(init.x() - 3, init.y(), 6, end.y() - init.y()) .normalized();
		else
  		rectangle = QRectF(init.x(), init.y() - 3, end.x() - init.x(), 6) .normalized();
		if (!found)
			p1 << init;
		else
			p2 << init;
		if (rectangle.contains(pnode)) {
			p1 << pnode;
			found = true;
			p2 << pnode;
		}
	}
	p2 << poly.last();
	if(p2.size() == 2)
		p2 << poly.last();
    if(p1.size() == 2)
		p1 << p1.last();
	GpxConnectionPoint *end = e->destPort();
	e->destPort()->removeEdge(e);
	e->setDestPort(n);
	e->setPolygon(p1);
  	if (end->isNode()) {
  	GpxConnectionNode *node = qgraphicsitem_cast<GpxConnectionNode*>(end);
	  int line = _coupledData->lineIndex(e->dataLine())+1;
  	node->dataPoint()->removeLine(line);
  }
	n->addEdge(e);
	setSinkTo(e->dataLine(),n);
	
	QPointF p=cp->getSceneConnectionPoint();
	GpxEdge *edge;
	if (!cp->isCross()) 
	{ // Create a new line
		Line *line1 = new Line();
		line1->addPoint(p.x(),p.y());
		int x= tempConnection->getMiddlePoint().x();
		x= (fabs(x-pnode.x()) < 1.05*GRID ? pnode.x() : x - x % GRID);   
		int y= tempConnection->getMiddlePoint().y();
		y= (fabs(y-pnode.y()) < 1.05*GRID ? pnode.y() : y - y % GRID);   
		line1->addPoint(x,y);
		line1->addPoint(pnode.x(),pnode.y());
		setSourceTo(line1,cp);
		setSinkTo(line1,n);
		edge = new GpxEdge(tempConnection->getInitPort(),n,line1);
    	edge->addPort(tempConnection->getInitPort());
		_coupledData->addLine(line1);
		addItem(edge);
	} else {
    // Finish a line
		edge = cp->edgeAt(0);
		QPolygonF po = edge->polygon();
		Line *line1 = edge->dataLine();
		int x= tempConnection->getMiddlePoint().x();
		x= (fabs(x-pnode.x()) < 1.05*GRID ? pnode.x() : x - x % GRID);   
		int y= tempConnection->getMiddlePoint().y();
		y= (fabs(y-pnode.y()) < 1.05*GRID ? pnode.y() : y - y % GRID);   
		po << QPointF(x,y);
		po << QPointF(pnode.x(),pnode.y());
		setSinkTo(line1,n);
		edge->setDestPort(n);
		edge->setPolygon(po);
		cp->removeEdge(edge);
		removeItem(cp);
		delete cp;
	}
  n->addEdge(edge);
  // Create the new line
	Line *line2 = new Line();
	foreach(QPointF pf, p2) 
		line2->addPoint(pf.x(),pf.y());
	setSourceTo(line2,n);
	setSinkTo(line2,end);
	_coupledData->addLine(line2);
	if (end->isNode()) {
		GpxConnectionNode *node = qgraphicsitem_cast<GpxConnectionNode*>(end);
		int	line = _coupledData->lineIndex(line2)+1;
		node->dataPoint()->addLine(line);
	}
	GpxEdge *edge2 = new GpxEdge(n,end,line2);
	addItem(edge2);
	n->addEdge(edge2);
	vector<int> ext;
	ext.push_back(_coupledData->lineIndex(edge->dataLine())+1);
	ext.push_back(_coupledData->lineIndex(edge2->dataLine())+1);
	ext.push_back(_coupledData->lineIndex(e->dataLine())+1);
	mp->setLines(ext);
	mp->setExtrems(ext);
  e->addPorts(edge->ports());
  e->addPorts(edge2->ports());
  edge->addPorts(e->ports());
  edge->addPorts(edge2->ports());
  edge2->addPorts(edge->ports());
  edge2->addPorts(e->ports());
  Q_ASSERT(n->edgeCount()==3);
}

QList <GpxEdit *> GpxEditionScene::editItems()
{
	QList<QGraphicsItem *> _items = selectedItems();
	QList<GpxEdit *> editItems;
	foreach(QGraphicsItem *item, _items){
		switch(item->type()){
			case GpxBlock::Type:
    			editItems << qgraphicsitem_cast<GpxBlock*>(item)->edit();
				break;
			case GpxEdge::Type:
    			editItems << qgraphicsitem_cast<GpxEdge*>(item)->edit();
				break;
		}
	}
	return editItems;
}

void GpxEditionScene::addAnnotation(QString s, QPoint p) {
  GpxTextBox *tb = new  GpxTextBox(_coupledData);
	tb->setPlainText(s);
  addItem(tb);
  if(!p.isNull() && !lastClick.isNull()) {
    tb->setPos(lastClick);
    lastClick = QPointF();
  }
  vector<string> extra = _coupledData->getExtra();
  int i=extra.size();
  extra.resize(i+1);
  s = s.replace("\n","\\n");
  s = s.replace("\"","\\\"");
  QString e("Annotation(\"%1\",%2,%3)");
  e=e.arg(s).arg(tb->pos().x()*TWIPS_TO_PIXEL).arg(tb->pos().y()*TWIPS_TO_PIXEL);
  extra[i] = qPrintable(e);
  _coupledData->setExtra(extra);
}

void GpxEditionScene::focusOutEvent ( QFocusEvent * focusEvent ) 
{
}

void GpxEditionScene::setSelectedItems()
{
	QList<QGraphicsItem *> items = selectedItems();
	// Special case, to check if there's only lines selected.
	int j = 0;
	bool lines_only = true;
	while(j < items.size()){
		QGraphicsItem *i = items.at(j);
		if (i->type()==GpxEdge::Type){
			GpxEdge *e=qgraphicsitem_cast<GpxEdge*>(i);
			GpxConnectionPoint *src=e->sourcePort();
			GpxConnectionPoint *dst=e->destPort();
			Line *l= e->dataLine(); 
			if (l->sourceType()==Line::COMPONENT || l->sourceType()==Line::PORT) 
			{
				GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(src->parentItem());
				if (b->isSelected()){ 
					lines_only = false;
					break;
				}
			}
			if (l->sinkType()==Line::COMPONENT || l->sinkType()==Line::PORT) 
			{
				GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(dst->parentItem());
				if (b->isSelected()){ 
					lines_only = false;
					break;
				}
			}
      		/*if (l->sourceType()==Line::CROSS)
      		{
				GpxConnectionPoint *p = qgraphicsitem_cast<GpxConnectionPoint*>(src);
				if (p->isSelected()){ 
					lines_only = false;
					break;
				}
        	}
	    	if (l->sinkType()==Line::CROSS)
      		{
				GpxConnectionPoint *p = qgraphicsitem_cast<GpxConnectionPoint*>(dst);
				if (!p->isSelected()){ 
					lines_only = false;
					break;
				}
      		}
			if (l->sourceType()==Line::NODE)
      		{
				GpxConnectionPoint *p = qgraphicsitem_cast<GpxConnectionPoint*>(src);
				if (!p->isSelected()){ 
					lines_only = false;
					break;
				}
        	}
	    	if (l->sinkType()==Line::NODE)
      		{
				GpxConnectionPoint *p = qgraphicsitem_cast<GpxConnectionPoint*>(dst);
				if (!p->isSelected()){ 
					lines_only = false;
					break;
				}
      		}*/
		}else if (i->type()==GpxConnectionPoint::Type){
      		GpxConnectionPoint *gp = qgraphicsitem_cast<GpxConnectionPoint*>(i);
			if(gp->isInport() || gp->isOutport()){
				lines_only = false;
				break;
			}
		} else {
			lines_only = false;
			break;
		}
		j++;
	}
	if(lines_only)
		return;
	foreach (QGraphicsItem *i, items)
	{
		if (i->type()==GpxEdge::Type){
			GpxEdge *e=qgraphicsitem_cast<GpxEdge*>(i);
			GpxConnectionPoint *src=e->sourcePort();
			GpxConnectionPoint *dst=e->destPort();
			Line *l= e->dataLine(); 
			if (l->sourceType()==Line::COMPONENT || l->sourceType()==Line::PORT) 
			{
				GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(src->parentItem());
				if (!b->isSelected()){ 
					e->setSelected(false);
					continue;
				}
			}
			if (l->sinkType()==Line::COMPONENT || l->sinkType()==Line::PORT) 
			{
				GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(dst->parentItem());
				if (!b->isSelected()){ 
					e->setSelected(false);
					continue;
				}
			}
      		if (l->sourceType()==Line::NODE || l->sourceType()==Line::CROSS)
      		{
				GpxConnectionPoint *p = qgraphicsitem_cast<GpxConnectionPoint*>(src);
				if (!p->isSelected()){ 
					e->setSelected(false);
					continue;
				}
        	}
	    	if (l->sinkType()==Line::NODE || l->sinkType()==Line::CROSS)
      		{
				GpxConnectionPoint *p = qgraphicsitem_cast<GpxConnectionPoint*>(dst);
				if (!p->isSelected()){ 
					e->setSelected(false);
					continue;
				}
      		}
		}
	}
}

void GpxEditionScene::convertToCoupled() {
  Coupled *c = new Coupled();
  double x=0,y=0;
  int count=0;
	QList<QGraphicsItem *> items = selectedItems();
  QMap<Point*,Point*> newNodes;
  QMap<Port*,Port*> newPorts;
	foreach (QGraphicsItem *i, items)
	{
		if (i->type()==GpxBlock::Type){
			GpxBlock *b=qgraphicsitem_cast<GpxBlock*>(i);
			if (b->isAtomic()) {
				GpxAtomic *ga = qgraphicsitem_cast<GpxAtomic*>(b);
        ga->atomicData()->setPriority(_coupledData->childIndex(ga->atomicData())); 
        x+=ga->atomicData()->graphic().x();
        y+=ga->atomicData()->graphic().y();
        count++;
				c->addChild(ga->atomicData());
			} else if (b->isCoupled()) {
				GpxCoupled *gc = qgraphicsitem_cast<GpxCoupled*>(b);
        gc->coupledData()->setPriority(_coupledData->childIndex(gc->coupledData())); 
				c->addChild(gc->coupledData());
        x+=gc->coupledData()->graphic().x();
        y+=gc->coupledData()->graphic().y();
        count++;
			} else if (b->isInport()) {
				GpxInport *p = qgraphicsitem_cast<GpxInport*>(b);
				// TODO: Temporal, it works, but I don't konw 
				// if this is the best way to fix this. Same fix applied 
				// with the outports.
        Port *port = new Port(*p->portData());
        newPorts.insert(p->portData(),port);
				c->addInport(port);
			} else {
				GpxOutport *p = qgraphicsitem_cast<GpxOutport*>(b);
        Port *port = new Port(*p->portData());
        newPorts.insert(p->portData(),port);
				c->addOutport(port);
			}
		}
    c->sortChilds();
    
		if (i->type()==GpxConnectionPoint::Type){
      GpxConnectionPoint *gp = qgraphicsitem_cast<GpxConnectionPoint*>(i);
      if (gp->isNode()) {
        GpxConnectionNode *node = qgraphicsitem_cast<GpxConnectionNode*>(i);
        Point *point = new Point(*node->dataPoint());
        newNodes.insert(node->dataPoint(),point);
        point->clearLines();
        c->addPoint(point);
      }
    }
	}
  foreach (QGraphicsItem *i, items)
	{
		if (i->type()==GpxEdge::Type){
			GpxEdge *e=qgraphicsitem_cast<GpxEdge*>(i);
			GpxConnectionPoint *src=e->sourcePort();
			GpxConnectionPoint *dst=e->destPort();
      GpxInport *in=NULL; GpxOutport *out=NULL;
      bool externConnectio=false;
			Line *l=new Line(*e->dataLine()); // Copy the line
			if (l->sourceType()==Line::COMPONENT) 
			{
				GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(src->parentItem());
				if (!b->isSelected()) {
					continue;
        }
				vector<int> li = l->sources();
				li[0]=c->childIndex(b->model())+1;
				l->setSources(li);
			}
			if (l->sinkType()==Line::COMPONENT) 
			{
				GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(dst->parentItem());
				if (!b->isSelected()) 
					continue;
				vector<int> li = l->sinks();
				li[0]=c->childIndex(b->model())+1;
				l->setSink(li);
			}
      if (l->sourceType()==Line::PORT)
      {
				GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(src->parentItem());
				if (!b->isSelected()) 
					continue;
        int port;
        if (b->isInport()) {
          GpxInport *p = qgraphicsitem_cast<GpxInport*>(b);
          port = c->portIndex(newPorts[p->portData()])+1;
        } else {
          GpxOutport *p = qgraphicsitem_cast<GpxOutport*>(b);
          port = c->portIndex(newPorts[p->portData()])+1;
        }
				vector<int> li = l->sources();
        li[0] = port;
				l->setSources(li);
      }
      if (l->sinkType()==Line::PORT)
      {
				GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(dst->parentItem());
				if (!b->isSelected()) 
					continue;
        int port;
        if (b->isInport()) {
          GpxInport *p = qgraphicsitem_cast<GpxInport*>(b);
          port = c->portIndex(newPorts[p->portData()])+1;
        } else {
          GpxOutport *p = qgraphicsitem_cast<GpxOutport*>(b);
          port = c->portIndex(newPorts[p->portData()])+1;
        }
				vector<int> li = l->sinks();
        li[0] = port;
				l->setSink(li);
      }
      if (l->sourceType()==Line::NODE)
      {
				GpxConnectionNode *node = qgraphicsitem_cast<GpxConnectionNode*>(src);
				if (!node->isSelected()) {
					continue;
        }
				vector<int> li = l->sources();
        li[0] = c->pointIndex(newNodes[node->dataPoint()])+1;
        //qDebug() << "Looking for " << node->dataPoint() << " and result is " << newNodes[node->dataPoint()] << " index " << c->pointIndex(newNodes[node->dataPoint()])+1 <<  endl;
        assert(li[0]>0 && li[0]<newNodes.size()+1);
				l->setSources(li);
      }
	    if (l->sinkType()==Line::NODE)
      {
				GpxConnectionNode *node = qgraphicsitem_cast<GpxConnectionNode*>(dst);
				if (!node->isSelected()) {
					continue;
        }
				vector<int> li = l->sinks();
        li[0] = c->pointIndex(newNodes[node->dataPoint()])+1;
        assert(li[0]>0 && li[0]<newNodes.size()+1);
				l->setSink(li);
      }
      c->removeLine(e->dataLine());
			c->addLine(l);
      removeItem(e);
      delete e;
		}
	}
 
  QList<QGraphicsItem *> it = selectedItems();
  QList<GpxEdge*> del;
  QList<GpxBlock *> blocks;
  foreach(QGraphicsItem *i,it)
  {
    if (i->type()==GpxTextBox::Type) {
      GpxTextBox *tb = dynamic_cast<GpxTextBox*>(i); 
      if (tb->parentItem()==NULL && tb->isSelected()) {
        // Removing an annotation
        vector<string> extra = _coupledData->getExtra();
        bool found=false;
        int j;
        for (j=0;j<extra.size();j++) {
          QString strExtra(extra[j].c_str());
          if (!strExtra.startsWith("Annotation")) continue;
          strExtra = strExtra.mid(11);
          strExtra.chop(1);
          QStringList ls = strExtra.split(",");
          QString an = ls.first();
          an.chop(1);
          an = an.mid(1);
          if (tb->toPlainText()==an) {
            vector<string> e = c->getExtra();
            e.resize(e.size()+1);
            e[e.size()-1] = extra[j].c_str(); 
            c->setExtra(e);
            found=true;
            break;
          } 
  
        }
        if (found) {
          extra.erase(extra.begin()+j);
          _coupledData->setExtra(extra);
        }
        removeItem(i);
      }
    } 
  }
	
  foreach(QGraphicsItem *gi,it) 
  {
    if (gi->type()==GpxEdge::Type) {
    	del << qgraphicsitem_cast<GpxEdge*>(gi);
    }
    if (gi->type()==GpxBlock::Type) {
			blocks << qgraphicsitem_cast<GpxBlock*>(gi);
		}
  }
  //removeLines(del);// First remove edges
  QList<GpxEdge *> edges;
  QList<QPair<GpxEdge *,int> > inputs;
  QList<QPair<GpxEdge *,int> > outputs;
  foreach(GpxBlock *b,blocks)
  {
  	edges.clear();
    if (b->isInport()) {
      GpxInport *ip = qgraphicsitem_cast<GpxInport*>(b);
			edges = b->connectedLines();
			removeLines(edges);
      emit remove_Port(_coupledData,ip->portData());
			// Delete connection of father and if open update graphics
    } else if (b->isOutport()) {
      GpxOutport *ip = qgraphicsitem_cast<GpxOutport*>(b);
			edges = b->connectedLines();
			removeLines(edges);
      emit remove_Port(_coupledData,ip->portData());
		  // Delete connection of father and if open update graphics
    } else {
      if (b->isCoupled())
      {
        GpxCoupled *c=qgraphicsitem_cast<GpxCoupled*>(b);
        emit closeAllChildsOf(c->coupledData());
      }
      Model *m = b->model();
      _coupledData->removeChild(m);
      for (int i=0;i<m->inPorts();i++) 
        if (b->inport(i)->edgeCount()) {
          bool portCreated=false;
          for (int j=0;j<b->inport(i)->edgeCount();j++) {
            GpxEdge *e=b->inport(i)->edgeAt(j);
            /*************************************************************/
            if (shouldCreatePort(e) && !portCreated) {
              Graphic g;
              
              //qDebug() <<  "***************************" << e->polygon() << "****************************\n";
              g.setWidth(450/TWIPS_TO_PIXEL);
              if (b->direction() == Graphic::RIGHT) {
                g.setY(b->inport(i)->getConnectionPoint().y());
                g.setX(b->inport(i)->getConnectionPoint().x()-30);
              }
              Port *p=new Port(Port::Inport,"Port1","",g);
              c->addInport(p);
              portCreated=true;
            }
            if (e->destPort()->parentItem()!=NULL) 
              if (e->destPort()->parentItem()->isSelected()) 
                e->setDestPort(NULL);
            if (e->destPort()->parentItem()==NULL) 
              if (e->destPort()->isSelected()) 
                e->setDestPort(NULL);
            if (e->sourcePort()->parentItem()!=NULL) 
              if (e->sourcePort()->parentItem()->isSelected()) 
                e->setSourcePort(NULL);
            if (e->sourcePort()->parentItem()==NULL) 
              if (e->sourcePort()->isSelected()) 
                e->setSourcePort(NULL);
            inputs << QPair<GpxEdge*,int>(e,c->inportCount()-1);
          }
        }
   for (int i=0;i<m->outPorts();i++) 
        if (b->outport(i)->edgeCount()) {
          bool portCreated=false;
          for (int j=0;j<b->outport(i)->edgeCount();j++) {
            GpxEdge *e=b->outport(i)->edgeAt(j);
            /*************************************************************/
            if (shouldCreatePort(e) && !portCreated) {
              Graphic g;
              g.setWidth(450/TWIPS_TO_PIXEL);
              if (b->direction() == Graphic::RIGHT) {
                g.setY(b->outport(i)->getConnectionPoint().y());
                g.setX(b->outport(i)->getConnectionPoint().x()+30);
              }
              Port *p=new Port(Port::Outport,"Port1","",g);
              c->addOutport(p);
              portCreated=true;
            }
            outputs << QPair<GpxEdge*,int>(e,c->outportCount()-1);
          }
        }
    }
  }
  _coupledData->updatePoints();

  c->graphic().setIcon("%basicelements%coupled.svg");
  c->graphic().setWidth(675/TWIPS_TO_PIXEL);
  c->graphic().setHeight(720/TWIPS_TO_PIXEL);
  if (count) {
    c->graphic().setX(x/count);
    c->graphic().setY(y/count);
  }
  c->updatePoints(false);
  c->simplifyLines();
  c->updatePoints();
  GpxCoupled *gpx_c=putCoupled(c);
  for(int i=0;i<inputs.length();i++)
  {
    GpxEdge *e = inputs.at(i).first;
    int inport = inputs.at(i).second;
    qDebug() << e->destPort() << " " << e->sourcePort();
    if (e->destPort()!=NULL) {
      qDebug()<< "ACA";
      //e->setSourcePort(gpx_c->inport(inport));
      //gpx_c->inport(inport)->addEdge(e);
    }
    if (e->sourcePort()!=NULL) {
      e->setDestPort(gpx_c->inport(inport));
      gpx_c->inport(inport)->addEdge(e);
    }
  }
	foreach(GpxBlock *b,blocks)  {
		removeItem(b);
	}
  clearSelection();
}

bool GpxEditionScene::shouldCreatePort(GpxEdge *e) {
  GpxConnectionPoint *src=e->sourcePort();
  GpxConnectionPoint *dst=e->destPort();
  if (src->parentItem() != NULL) 
    if (dst->parentItem() !=NULL) 
      return src->parentItem()->isSelected() ^ dst->parentItem()->isSelected();
  if (src->parentItem() == NULL) 
    if (dst->parentItem() !=NULL) 
      return src->isSelected() ^ dst->parentItem()->isSelected();
  if (src->parentItem() != NULL) 
    if (dst->parentItem() ==NULL) 
      return src->parentItem()->isSelected() ^ dst->isSelected();
  if (src->parentItem() == NULL) 
    if (dst->parentItem() ==NULL) 
      return src->isSelected() ^ dst->isSelected();
}
