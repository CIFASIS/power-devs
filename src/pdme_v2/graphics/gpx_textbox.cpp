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

#include <QtGui>
#include <QInputDialog>
#include <QMessageBox>

#include "gpx_textbox.h"
#include "gpx_edition_scene.h"
#include "gpx_edition_window.h"
#include "gpx_block.h"
#include "gpx_outport.h"
#include "gpx_inport.h"
#include <dialogs/dlg_change_annotation.h>
#include "parser.h"

GpxTextBox::GpxTextBox(QGraphicsItem * parent, QGraphicsScene * scene) : QGraphicsTextItem(parent)
{
	setAcceptedMouseButtons(Qt::LeftButton);
  if (parent==NULL) {
    setFlags(QGraphicsItem::ItemIsMovable |  QGraphicsItem::ItemIsSelectable);
  }
}

GpxTextBox::GpxTextBox(Coupled *c) : QGraphicsTextItem(NULL), _coupledData(c)
{
	setAcceptedMouseButtons(Qt::LeftButton);
  setFlags(QGraphicsItem::ItemIsMovable |  QGraphicsItem::ItemIsSelectable| QGraphicsItem::QGraphicsItem::ItemSendsGeometryChanges);
}


void GpxTextBox::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{
  bool ok=false;
  
  QString newName;
  if (parentItem()==NULL) {
    DlgChangeAnnotation dlg(this);
    QString annotation = dlg.getAnnotation();
    if (annotation.isNull())
      return;
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
      an = an.replace("\\n","\n");
      an = an.replace("\\\"","\"");
      if (toPlainText()==an) {
        found=true;
        break;
      } 
    }
    setPlainText(annotation);
    if (found) {
      QString e("Annotation(\"%1\",%2,%3)");
      annotation = annotation.replace("\n","\\n");
      annotation = annotation.replace("\"","\\\"");
      e=e.arg(annotation).arg(pos().x()*TWIPS_TO_PIXEL).arg(pos().y()*TWIPS_TO_PIXEL);
      extra[j] = qPrintable(e);
      _coupledData->setExtra(extra);
      GpxEditionScene *ges = dynamic_cast<GpxEditionScene*>(scene());
      GpxEditionWindow *gew = dynamic_cast<GpxEditionWindow*>(ges->parent());
      gew->setDirty();
    } else {
    }
    return;
  }
  newName = QInputDialog::getText(NULL,"PowerDEVS","Change name",QLineEdit::Normal,toPlainText(),&ok).trimmed();
  if (!ok || newName.isEmpty()) return;
  GpxEditionScene *ges = dynamic_cast<GpxEditionScene*>(scene());
  GpxBlock *b = qgraphicsitem_cast<GpxBlock*>(parentItem());
  if (ges && b) 
  {
    if (ges->canUseName(newName,b)) 
    {
      b->setName(newName);
      if (b->isAtomic() || b->isCoupled()) 
      {
        Model *m = b->model();
        m->setName(qPrintable(newName));
		ges->changeBlockName(b);
	  } else if (b->isInport()) {
        GpxInport *p = qgraphicsitem_cast<GpxInport*>(b);
        p->portData()->setName(qPrintable(newName));
      } else {
        GpxOutport *p = qgraphicsitem_cast<GpxOutport*>(b);
        p->portData()->setName(qPrintable(newName));
      }
    } else {
      QMessageBox::warning(NULL,"PowerDEVS","Name already in use");
    }
  }
}

QVariant GpxTextBox::itemChange(GraphicsItemChange change, const QVariant & value)
{
	if (change == QGraphicsItem::ItemPositionHasChanged) {
    if (parentItem()==NULL) {
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
      an = an.replace("\\n","\n");
      an = an.replace("\\\"","\"");
      if (toPlainText()==an) {
        found=true;
        break;
      } 
    }
    if (found) {
      QString e("Annotation(\"%1\",%2,%3)");
      QString an = toPlainText();
      an = an.replace("\n","\\n");
      an = an.replace("\"","\\\"");
      e=e.arg(an).arg(pos().x()*TWIPS_TO_PIXEL).arg(pos().y()*TWIPS_TO_PIXEL);
      extra[j] = qPrintable(e);
      _coupledData->setExtra(extra);
      GpxEditionScene *ges = dynamic_cast<GpxEditionScene*>(scene());
      GpxEditionWindow *gew = dynamic_cast<GpxEditionWindow*>(ges->parent());
      gew->setDirty();
    } else {
    }
 
    }
  }
  return value;
}
