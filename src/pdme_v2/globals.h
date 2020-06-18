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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <QVariant>
#include <QDebug>
#include <QString>
#include <QColor>
#include <data/coupled.h>

QVariant getSetting(QString key);
void setSetting(QString key,QVariant);
void removeSetting(QString key);

extern unsigned int depth;
Coupled *parseFile(QString fileName);
QString getImage(QString fileName);
int colorToInt(QColor c);
QColor intToColor(int c);
QString colorToName(int c);
#endif
