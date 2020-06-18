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

#include <QTextDocument>
#include <graphics/gpx_inport.h>
#include <graphics/gpx_edition_scene.h>

GpxInport::GpxInport(QGraphicsScene * scene, Port *data): GpxBlock(), _portData(data) 
{
  Graphic &gpx = data->graphic();
  _width=gpx.width();
  _myDirection = gpx.direction();
  _height=_width;
  addOutport();
  setName(data->name().c_str());
  setDirection(_myDirection);
  _name_item.setTextWidth(_width+10);
  _name_item.document()->setTextWidth(_width+10);
  QTextOption o;
  o.setWrapMode(QTextOption::WordWrap);
  o.setAlignment(Qt::AlignCenter);
  _name_item.document()->setDefaultTextOption(o);
  _edit = new EditInport(this);
}

GpxInport::~GpxInport()
{
}

void GpxInport::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *)
{
  QFontMetricsF font_metrics(painter->font());
  QPointF pos_number;
  QString number = QString("%1").arg(_portData->portNumber());
  _myPolygon.clear();
	_myPolygon << QPointF(-(_width / 2), (_height / 2))
            << QPointF((_width / 2), (_height / 2)) 
            << QPointF((_width / 2), -(_height / 2)) 
            << QPointF(-(_width / 2), -(_height / 2)) 
            << QPointF(-(_width / 2), (_height / 2));
	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawEllipse(_myPolygon.boundingRect());
  painter->rotate(-rotationAngle(Graphic::RIGHT,_myDirection));
  pos_number = QPointF(-font_metrics.width(number)/2,font_metrics.lineSpacing()/2-2);
  painter->drawText(pos_number.x(),pos_number.y(),number);
  paintAsSelected(painter);
}

void GpxInport::rotate()
{
  GpxBlock::rotate();
  _portData->graphic().setDirection(_myDirection);
}

void GpxInport::resizeTo(QPointF p,Qt::Corner c)
{
  GpxBlock::resizeTo(p,c);
  _width=std::max(_height,_width);
  _height=std::max(_height,_width);
  _portData->graphic().setWidth(_width);
  _portData->graphic().setHeight(_height);
}

void GpxInport::resizeTo(QPoint p)
{
  GpxBlock::resizeTo(p);
  _width=std::max(_height,_width);
  _height=std::max(_height,_width);
  _portData->graphic().setWidth(_width);
  _portData->graphic().setHeight(_height);
}

QVariant GpxInport::itemChange(GraphicsItemChange change, const QVariant & value)
{
  QVariant v=GpxBlock::itemChange(change,value);
	if (change == QGraphicsItem::ItemPositionHasChanged) {
		setPos((int)x()-(int)x() % GRID,
		(int)y()-(int)y() % GRID);
    _portData->graphic().setX(x()-getWidth()/2);
    _portData->graphic().setY(y());
  }
  return v;
}
