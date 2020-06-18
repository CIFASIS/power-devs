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

#ifndef COUPLED_H
#define COUPLED_H

#include "modelatomic.h"
#include "modelparameter.h"
#include "modelchild.h"

typedef enum { INPORT, OUTPORT } typePort;

class modelPoint {
 public:
	QList < int >lines;
	QStringList extrems;
	bool valid;
};

class modelLine {
 public:
	QString sourceType;
	QList < int >sources;
	QString sinkType;
	 QList < int >sinks;
};

class modelConnection {
 public:
	int childSource;
	int sourcePort;
	int childSink;
	int sinkPort;
};

class modelPort {
 public:
	QString name;
	int coupledPort;
	typePort type;
};
class modelChild;

class modelCoupled {
 public:
	QString name;
	QString type;
	int inPorts;
	int outPorts;
	modelCoupled *father;
	 QList < modelParameter * >params;
	 QList < modelChild * >childs;
	 QList < modelPoint * >points;
	 QList < modelLine * >lines;
	 QList < modelPort * >lsPorts;
	 QList < modelConnection * >lsIC;
	 QList < modelConnection * >lsEIC;
	 QList < modelConnection * >lsEOC;
	 QList < QString > extra;
};
#endif
