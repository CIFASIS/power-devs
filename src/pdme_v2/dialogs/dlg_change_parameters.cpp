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

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QDoubleValidator>
#include <QLabel>
#include <QDebug>
#include <QComboBox>
#include <QSpinBox>


#include <dialogs/dlg_change_parameters.h>
#include <data/coupled.h>

DlgChangeParameters::DlgChangeParameters(Coupled *c): _c(c), _b(NULL)
{
  fromBlock =false;
  setWindowTitle(QString("Parameters: %1").arg(c->name().c_str()));
  _desc_layout = new QVBoxLayout();
  _params_layout = new QVBoxLayout();
  _desc = new QPlainTextEdit(QString(c->description().c_str()).replace("\\n","\n"));
  _params_group = new QGroupBox();
  _params_group->setTitle("Parameters");
  if (c->father()!=NULL) 
    _desc->setTextInteractionFlags(Qt::NoTextInteraction);
  else {
    QTextCursor cursor(_desc->textCursor());
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    _desc->setTextCursor(cursor);
    setWindowTitle(QString("Documentation %1").arg(c->name().c_str()));
    resize(800,600);
    _params_group->setTitle("");
 
  }
  _desc_group = new QGroupBox();
  _layout = new QVBoxLayout();
  _buttons = new QDialogButtonBox();
  _desc_layout->addWidget(_desc);
  _buttons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
  _buttons->setOrientation(Qt::Horizontal);
  connect(_buttons, SIGNAL(accepted()), this, SLOT(accept()));
  connect(_buttons, SIGNAL(rejected()), this, SLOT(reject()));
  connect(this, SIGNAL(accepted()), this, SLOT(apply()));
  _desc_group->setLayout(_desc_layout);
  _desc_group->setTitle("Description");
  _layout->addWidget(_desc_group);
  setLayout(_layout);
  _parameters = c->parameters();
  std::vector<Parameter*>::iterator i=_parameters.begin();
  for(;i!=_parameters.end();i++)
  {
    qDebug() << *i << endl;
    if ((*i)->isString()) {
      StringParameter *sp = dynamic_cast<StringParameter*>(*i); 
      qDebug() << sp << endl;
      QLabel *name;
      if (strlen(sp->description().c_str()))
        name = new QLabel(QString("%1 (%2)").arg(sp->name().c_str()).arg(QString(sp->description().c_str()).replace("\\n","\n")));
      else
        name = new QLabel(QString("%1").arg(sp->name().c_str()));
 
      QString val = sp->strValue().c_str();
      _params_layout->addWidget(name);
      QLineEdit *field = new QLineEdit();
      if (val.startsWith("\%")) {
          qDebug() << "Look for parameter in parent models";
          field->setEnabled(false);
          if (c->father())
              val = c->father()->getParameter(qPrintable(val.mid(1))).c_str();
      } 
      field->setText(val);
       _widgets.push_back(field);
       _params_layout->addWidget(field);
    } else if ((*i)->isValue()) {
      ValueParameter *vp = dynamic_cast<ValueParameter*>(*i); 
      qDebug() << vp << endl;

      QLabel *name;
      if (strlen(vp->description().c_str()))
        name = new QLabel(QString("%1 (%2)").arg(vp->name().c_str()).arg(QString(vp->description().c_str()).replace("\\n","\n")));
      else
        name = new QLabel(QString("%1").arg(vp->name().c_str()));
      QLineEdit *field = new QLineEdit(QString("%1").arg(vp->value()));
      QDoubleValidator *dv = new QDoubleValidator(field);
      field->setAlignment(Qt::AlignRight);
      field->setValidator(dv);
      _params_layout->addWidget(name);
      _params_layout->addWidget(field);
      _widgets.push_back(field);
    } else if ((*i)->isList()) {
      ListParameter *lp = dynamic_cast<ListParameter*>(*i); 
      qDebug() << lp << endl;
      QLabel *name;
      if (strlen(lp->description().c_str()))
        name = new QLabel(QString("%1 (%2)").arg(lp->name().c_str()).arg(QString(lp->description().c_str()).replace("\\n","\n")));
      else
        name = new QLabel(QString("%1").arg(lp->name().c_str()));
 
      QComboBox *field = new QComboBox();
	    vector<string> list = lp->list();
	    vector<string>::iterator is;
      for (is=list.begin();is!=list.end();is++)
      {
        field->addItem(is->c_str());
      }
      field->setCurrentIndex(lp->selected()-1);
      _widgets.push_back(field);
      _params_layout->addWidget(name);
      _params_layout->addWidget(field);
    }
  }
  if (c->parameterCount())
  {
    _params_group->setLayout(_params_layout);
    _layout->addWidget(_params_group);
  }
  _layout->addWidget(_buttons);
}

DlgChangeParameters::DlgChangeParameters(GpxBlock *b): _b(b), _c(NULL)
{
  fromBlock =true;
  Model *m = b->model();
  setWindowTitle(QString("Parameters: %1").arg(m->name().c_str()));
  _desc_layout = new QVBoxLayout();
  _params_layout = new QVBoxLayout();
  _desc = new QPlainTextEdit(QString(m->description().c_str()).replace("\\n","\n"));
  _desc->setTextInteractionFlags(Qt::NoTextInteraction);
  _desc_group = new QGroupBox();
  _params_group = new QGroupBox();
  _params_group->setTitle("Parameters");
  _layout = new QVBoxLayout();
  _buttons = new QDialogButtonBox();
  _desc_layout->addWidget(_desc);
  _buttons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
  _buttons->setOrientation(Qt::Horizontal);
  connect(_buttons, SIGNAL(accepted()), this, SLOT(accept()));
  connect(_buttons, SIGNAL(rejected()), this, SLOT(reject()));
  connect(this, SIGNAL(accepted()), this, SLOT(apply()));
  _desc_group->setLayout(_desc_layout);
  _desc_group->setTitle("Description");
  _layout->addWidget(_desc_group);
  setLayout(_layout);
  _parameters = m->parameters();
  std::vector<Parameter*>::iterator i=_parameters.begin();
  for(;i!=_parameters.end();i++)
  {
    qDebug() << *i << endl;
    if ((*i)->isString()) {
      StringParameter *sp = dynamic_cast<StringParameter*>(*i); 
      qDebug() << sp << endl;
      QLabel *name;
      if (strlen(sp->description().c_str()))
        name = new QLabel(QString("%1 (%2)").arg(sp->name().c_str()).arg(QString(sp->description().c_str()).replace("\\n","\n")));
      else
        name = new QLabel(QString("%1").arg(sp->name().c_str()));
 
      QString val = sp->strValue().c_str();
      _params_layout->addWidget(name);
        if (b->isAtomic() && val.startsWith("\%inputs",Qt::CaseInsensitive)) {
          QSpinBox *field = new QSpinBox();
          field->setMinimum(0);
          field->setMaximum(20);
          field->setValue(m->inPorts());
          _widgets.push_back(field);
          _params_layout->addWidget(field);
        } else if (b->isAtomic() && val.startsWith("\%outputs",Qt::CaseInsensitive)) {
          QSpinBox *field = new QSpinBox();
          field->setMinimum(0);
          field->setMaximum(20);
          field->setValue(m->outPorts());
          _widgets.push_back(field);
          _params_layout->addWidget(field);
        } else {
        QLineEdit *field = new QLineEdit();
        if (val.startsWith("\%",Qt::CaseInsensitive)) {
          field->setEnabled(false);
          if (m->father())
            val = m->father()->getParameter(qPrintable(val.mid(1))).c_str();
        }
        field->setText(val);

        _widgets.push_back(field);
        _params_layout->addWidget(field);
        }
    } else if ((*i)->isValue()) {
      ValueParameter *vp = dynamic_cast<ValueParameter*>(*i); 
      qDebug() << vp << endl;

      QLabel *name;
      if (strlen(vp->description().c_str()))
        name = new QLabel(QString("%1 (%2)").arg(vp->name().c_str()).arg(QString(vp->description().c_str()).replace("\\n","\n")));
      else
        name = new QLabel(QString("%1").arg(vp->name().c_str()));
      QLineEdit *field = new QLineEdit(QString("%1").arg(vp->value()));
      QDoubleValidator *dv = new QDoubleValidator(field);
      field->setAlignment(Qt::AlignRight);
      field->setValidator(dv);
      _params_layout->addWidget(name);
      _params_layout->addWidget(field);
      _widgets.push_back(field);
    } else if ((*i)->isList()) {
      ListParameter *lp = dynamic_cast<ListParameter*>(*i); 
      qDebug() << lp << endl;
      QLabel *name;
      if (strlen(lp->description().c_str()))
        name = new QLabel(QString("%1 (%2)").arg(lp->name().c_str()).arg(QString(lp->description().c_str()).replace("\\n","\n")));
      else
        name = new QLabel(QString("%1").arg(lp->name().c_str()));
 
      QComboBox *field = new QComboBox();
	    vector<string> list = lp->list();
	    vector<string>::iterator is;
      for (is=list.begin();is!=list.end();is++)
      {
        field->addItem(is->c_str());
      }
      field->setCurrentIndex(lp->selected()-1);
      _widgets.push_back(field);
      _params_layout->addWidget(name);
      _params_layout->addWidget(field);
    }
  }
  if (m->parameterCount())
  {
    _params_group->setLayout(_params_layout);
    _layout->addWidget(_params_group);
  }
  _layout->addWidget(_buttons);
}

void DlgChangeParameters::apply()
{
  std::vector<Parameter*>::iterator i=_parameters.begin();
	bool block=_b!=NULL;
  int p=0;
  for(;i!=_parameters.end();i++,p++)
  {
    if ((*i)->isString()) {
      StringParameter *sp = dynamic_cast<StringParameter*>(*i); 
      QString val = sp->strValue().c_str();
      if (val.startsWith("\%")) {
        if (block && _b->isAtomic() && val.startsWith("\%inputs",Qt::CaseInsensitive)) {
          QSpinBox *field = (QSpinBox*) _widgets[p];
          unsigned int ports = field->value();
          _b->setInportsTo(ports);
        } else if (block && _b->isAtomic() && val.startsWith("\%outputs",Qt::CaseInsensitive)) {
          QSpinBox *field = (QSpinBox*) _widgets[p];
          unsigned int ports = field->value();
          _b->setOutportsTo(ports);
        } else {
          qDebug() << "Look for parameter in parent models";
        }
 
      } else {
        QLineEdit *field = (QLineEdit*) _widgets[p];
        sp->setValue(field->text().toLatin1().constData());
      }
    } else if ((*i)->isValue()) {
      ValueParameter *vp = dynamic_cast<ValueParameter*>(*i); 
      QLineEdit *field = (QLineEdit*) _widgets[p];
      vp->setValue(field->text().toDouble());
    } else if ((*i)->isList()) {
      ListParameter *lp = dynamic_cast<ListParameter*>(*i); 
      QComboBox *field = (QComboBox*) _widgets[p];
      lp->setSelected(field->currentIndex()+1);
    } 
  }
  if (_b==NULL && _c!=NULL) 
    if (_c->father()==NULL) {
      _c->setDescription(qPrintable(_desc->toPlainText().replace("\n","\\n")));
    }
}
