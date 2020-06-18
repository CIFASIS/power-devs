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

#ifndef GPXEDIT_H
#define GPXEDIT_H 

class GpxAtomic;
class GpxCoupled;
class GpxInport;
class GpxOutport;
class GpxEdge;

class GpxEdit 
{
 public:
  virtual void resizeTo(QPointF,Qt::Corner) = 0;
  virtual void resizeTo(QPoint) = 0;
  virtual void setWidth(unsigned int) = 0;
  virtual void setHeight(unsigned int) = 0;
  virtual int height() = 0;
  virtual int width() = 0;
  virtual void setName(QString) = 0;
  virtual QString name() = 0;
  virtual void copy() = 0;
  virtual void paste() = 0;
  virtual void select() = 0;
  virtual void cut() = 0;
  virtual void rotate() = 0;
  virtual void moveTo() = 0;
  virtual void insert() = 0;
  virtual void deleteItem() = 0;
  virtual QPointF pos() = 0;
  virtual void setPos(QPointF) = 0;
};

class EditAtomic : public GpxEdit
{
 public:
 	EditAtomic(GpxAtomic *);
   	~EditAtomic();
    void resizeTo(QPointF,Qt::Corner);
    void resizeTo(QPoint);
    void setWidth(unsigned int);
    void setHeight(unsigned int);
    int height();
    int width();
    void setName(QString);
    QString name();
    void copy();
    void paste();
    void select();
    void cut();
    void rotate();
    void moveTo();
    void insert();
    void deleteItem();
  	QPointF pos();
  	void setPos(QPointF);
 private:
 	GpxAtomic *_atomic;
};

class EditCoupled : public GpxEdit
{
 public:
 	EditCoupled(GpxCoupled *);
   	~EditCoupled();
	void resizeTo(QPointF,Qt::Corner);
    void resizeTo(QPoint);
    void setWidth(unsigned int);
    void setHeight(unsigned int);
    int height();
    int width();
    void setName(QString);
    QString name();
    void copy();
    void paste();
    void select();
    void cut();
    void rotate();
    void moveTo();
    void insert();
    void deleteItem();
  	QPointF pos();
  	void setPos(QPointF);
 private:
 	GpxCoupled *_coupled;
};

class EditInport : public GpxEdit
{
 public:
 	EditInport(GpxInport *);
   	~EditInport();
    void resizeTo(QPointF,Qt::Corner);
    void resizeTo(QPoint);
    void setWidth(unsigned int);
    void setHeight(unsigned int);
    int height();
    int width();
    void setName(QString);
    QString name();
    void copy();
    void paste();
    void select();
    void cut();
    void rotate();
    void moveTo();
    void insert();
    void deleteItem();
  	QPointF pos();
  	void setPos(QPointF);
 private:
 	GpxInport *_inport;
};

class EditOutport : public GpxEdit
{
 public:
 	EditOutport(GpxOutport *);
   	~EditOutport();
	void resizeTo(QPointF,Qt::Corner);
    void resizeTo(QPoint);
    void setWidth(unsigned int);
    void setHeight(unsigned int);
    int height();
    int width();
    void setName(QString);
    QString name();
    void copy();
    void paste();
    void select();
    void cut();
    void rotate();
    void moveTo();
    void insert();
    void deleteItem();
  	QPointF pos();
  	void setPos(QPointF);
 private:
 	GpxOutport *_outport;
};

class EditEdge : public GpxEdit
{
 public:
 	EditEdge(GpxEdge *);
   	~EditEdge();
	void resizeTo(QPointF,Qt::Corner);
    void resizeTo(QPoint);
    void setWidth(unsigned int);
    void setHeight(unsigned int);
    int height();
    int width();
    void setName(QString);
    QString name();
    void copy();
    void paste();
    void select();
    void cut();
    void rotate();
    void moveTo();
    void insert();
    void deleteItem();
  	QPointF pos();
  	void setPos(QPointF);
 private:
 	GpxEdge *_edge;
	QPolygonF _origPos;
};
#endif
