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

#ifndef PARSER_H
#define PARSER_H

#define  TWIPS_TO_PIXEL (15)
#include <QFile>
#include <QStringList>
#include <QList>

#include "coupled.h"

#include <stdio.h>
#include <stdlib.h>

#define TAB(N) QSTR(QString("").fill(' ',4*N))

#define TOKCOUPLED 		"Coupled"
#define TOKROOT  			"Root"
#define TOKOBRACE  		"{"
#define TOKCBRACE  		"}"
#define TOKTYPE    		"Type"
#define TOKNAME    		"Name"
#define TOKPORTS   		"Ports"
#define TOKGRAPHIC   		"Graphic"
#define TOKDESCRIPTION 		"Description"
#define TOKPARAMETERS  		"Parameters"
#define TOKSYSTEM      		"System"
#define TOKATOMIC		"Atomic"
#define TOKPATH	 		"Path"
#define TOKPOINT		"Point"
#define TOKLINE		 	"Line"
#define TOKINPORT  		"Inport"
#define TOKOUTPORT		"Outport"
#define TOKCOUPLEDPORT	 	"CoupledPort"
#define TOKCONECTEDLINES	"ConectedLines"
#define TOKCONECTEDEXTREMS 	"ConectedExtrems"
#define TOKPOSITION 		"Position"
#define TOKSOURCE 		"Source"
#define TOKSINK 		"Sink"
#define TOKPOINTX 		"PointX"
#define TOKPOINTY 		"PointY"
#define TOKVAL 			"Val"
#define TOKSTR	 		"Str"
#define TOKLST	 		"Lst"
#define TOKCOLON		";"
#define TOKPERC			"%"
#define TOKEQUAL		"="
#define TOKPNT			"Pnt"
#define TOKCMP			"Cmp"
#define TOKPRT			"Prt"
#define TOKNOC			"Noc"
#define TOKNONE			"None"
#define TOKDST			"Dst"
#define TOKORG			"Org"

#define TOKROOTCOORD	 "Root-Coordinator"
#define TOKSIMULATOR "Simulator"
#define TOKCOORDINATOR "Coordinator"
#define TOKEIC 				"EIC"
#define TOKEOC 				"EOC"
#define TOKIC 				"IC"
#define TOKOPAR				"("
#define TOKCPAR				")"
#define TOKDIMENSION		"Dimension"
#define TOKDIRECTION		"Direction"
#define TOKCOLOR		"Color"
#define TOKICON			"Icon"
#define TOKWINDOW 		"Window"
#define TOKLEFT			"Left"
#define TOKRIGHT		"Right"
#define TOKUP			"Up"
#define TOKDOWN			"Down"
#define TOKEXTRA		"Extra"

#define QSTR(a)			((a).toLatin1().constData())

Coupled *parseModel(QString filename);
Coupled *parsePDS(QString filename);
//bool saveModelToFile(modelCoupled * c, QString filename);

#endif
