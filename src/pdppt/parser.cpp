/****************************************************************************
**
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**		       Universidad Nacional de Rosario - Argentina.
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

#include <QDebug>
#include <QMessageBox>
#include <QVector>
#include <QList>
#include "parser.h"

int lineNumber;
QFile *fd;
int version = 2;

QString getLine()
{
	lineNumber++;
	if (fd->atEnd()) {
		printf("ERROR: End of file reached\n");
		exit(-1);
	}
	// Chequae fin de archivo!!!
	QString line=(fd->readLine()).trimmed();
  if (line.contains("//")) {
    return line.left(line.indexOf("//")).trimmed();
  }
  return line;
  
}

QList<QString> parseExtra() {
  QList<QString> ret;
  int i=0;
	QString strLine = getLine();
	if (strLine.trimmed()!=TOKOBRACE)
		return ret;
	do {
		//printf("SKIP: %s\n",strLine.toLatin1().constData());
		strLine = getLine();
	  if (strLine == TOKCBRACE) 
      break;
    ret << strLine.trimmed();
	} while (strLine != TOKCBRACE);
  return ret;
}


int checkEqual(QString s, QString t)
{
	if (s != t) {
		printf("Error: Expecting: '%s' found: '%s' at line %d\n",
		       QSTR(t), QSTR(s), lineNumber);
		exit(-1);
	}
	return 1;
}

void skipSection()
{
	QString strLine = getLine();
	checkEqual(strLine, TOKOBRACE);
	do {
		//printf("SKIP: %s\n",strLine.toLatin1().constData());
		strLine = getLine();
	} while (strLine != TOKCBRACE);
}

QString getValue(QString s, QString t)
{
	if (!s.startsWith(t + " " + TOKEQUAL)) {
		printf("Error: Expecting: '%s' found: '%s' at line %d\n",
		       QSTR(t), QSTR(s), lineNumber);
		exit(-1);
	}
	return s.mid(s.indexOf(TOKEQUAL) + 1).trimmed();
}

modelLine *parseLine()
{

	modelLine *ret = new modelLine();

	QString strLine = getLine();
	checkEqual(strLine, TOKOBRACE);

	strLine = getLine();
	QString source = getValue(strLine, TOKSOURCE);

	strLine = getLine();
	QString sink = getValue(strLine, TOKSINK);

	strLine = getLine();
	//QString pointx=getValue(strLine,TOKPOINTX);

	strLine = getLine();
	//QString pointy=getValue(strLine,TOKPOINTY);

	strLine = getLine();
	checkEqual(strLine, TOKCBRACE);

	QStringList src = source.split(TOKCOLON, QString::SkipEmptyParts);
	ret->sourceType = src.takeFirst().trimmed();
	QStringList::iterator i;
	QList < int >sources;
	for (i = src.begin(); i != src.end(); ++i)
		sources.append(i->toInt());
	ret->sources = sources;

	src = sink.split(TOKCOLON, QString::SkipEmptyParts);
	ret->sinkType = src.takeFirst().trimmed();
	QList < int >sinks;
	for (i = src.begin(); i != src.end(); ++i)
		sinks.append(i->toInt());
	ret->sinks = sinks;
	return ret;

}

modelPoint *parsePoint()
{

	modelPoint *ret = new modelPoint();
	QString strLine = getLine();
	checkEqual(strLine, TOKOBRACE);

	strLine = getLine();
	QString connectedlines = getValue(strLine, TOKCONECTEDLINES);

	strLine = getLine();
	QString connectedextrems = getValue(strLine, TOKCONECTEDEXTREMS);

	strLine = getLine();
	//QString position=getValue(strLine,TOKPOSITION);

	strLine = getLine();
	checkEqual(strLine, TOKCBRACE);

	ret->extrems =
	    connectedextrems.split(TOKCOLON, QString::SkipEmptyParts);
	QStringList t = connectedlines.split(TOKCOLON, QString::SkipEmptyParts);
	QStringList::iterator i;
	QList < int >ls;
	for (i = t.begin(); i != t.end(); ++i)
		ls.append(i->toInt());
	ret->lines = ls;
	return ret;
}

QStringList unescapeParams(QString str)
{
  QStringList ret,lsv;
  QString val;
  lsv = str.split(TOKCOLON);
  ret << lsv.takeFirst();
  ret << lsv.takeLast();
  val = lsv.join(QString(";"));
  val = val.replace("\\;",";");
  ret.insert(1,val);
  return ret;
}

QList < modelParameter * >parseParameters()
{
	bool ok;
	QList < modelParameter * >ret;
	QString strLine = getLine();
	checkEqual(strLine, TOKOBRACE);
	QString par;
	QString name, value, desc, type;
	QStringList lsn, lsv, lss;
	strLine = getLine();
	while (strLine != TOKCBRACE) {
		lsn << strLine.left(strLine.indexOf(TOKEQUAL))
		    << strLine.mid(strLine.indexOf(TOKEQUAL) + 2);
		name = lsn.first().trimmed();
    if (version > 1)
		  lsv = unescapeParams(lsn.last());
    else
		  lsv = lsn.last().split(TOKCOLON);
		type = lsv.takeFirst().trimmed();
		value = lsv.takeFirst().trimmed();
		desc = lsv.takeFirst().trimmed();
		modelParameter *p = new modelParameter();
		p->name = name;
		if (type == TOKVAL) {
			p->type = VAL;
			p->dValue = value.toDouble(&ok);
			//printf("Parameter '%s' de tipo val valor %g\n",QSTR(name),p->dValue);
		}
		if (type == TOKSTR) {
			p->type = STR;
			p->strValue = value;
			//printf("Parameter '%s' de tipo str valor '%s'\n",QSTR(name),QSTR(p->strValue));
		}
		if (type == TOKLST) {
			QStringList qs =
			    value.split(TOKPERC, QString::SkipEmptyParts);
			p->type = LST;
			p->lsValue = qs.takeFirst().trimmed().toInt(&ok);
			p->lsValues = qs;
			//printf("Parameter '%s' de tipo lst valor '%d'\n",QSTR(name),p->lsValue);
			/*
			   QStringList::iterator i;
			   for (i=qs.begin();i!=qs.end();i++) {
			   QString t=*i;
			   printf("\t%s\n",QSTR(t));
			   }
			 */
		}
		//p->desc=desc;
		ret.append(p);
		strLine = getLine();
		lsn.clear();
	}
	return ret;
}

modelAtomic *parseAtomic()
{
	int inPorts, outPorts;
	bool ok;
	QString strLine = getLine();
	checkEqual(strLine, TOKOBRACE);

	strLine = getLine();
	QString name = getValue(strLine, TOKNAME);

	strLine = getLine();
	QString ports = getValue(strLine, TOKPORTS);
	QStringList slports = ports.split(TOKCOLON, QString::SkipEmptyParts);
	inPorts = slports.first().toInt(&ok);
	outPorts = slports.last().toInt(&ok);
	//printf("Atomic %s has %d inports and %d outports\n",name.toLatin1().constData(),inPorts,outPorts);

	strLine = getLine();
	QString path = getValue(strLine, TOKPATH);
  path = path.replace("Continuous","continuous");
  path = path.replace("Discrete","discrete");
  path = path.replace("Hybrid","hybrid");
  path = path.replace("Qss","qss");
  path = path.replace("Realtime","realtime");
  path = path.replace("Sources","sources");
  path = path.replace("Source","source");
  path = path.replace("Sinks","sinks");
  path = path.replace("Sink","sink");



	strLine = getLine();
	QString desc = getValue(strLine, TOKDESCRIPTION);

	strLine = getLine();
	checkEqual(strLine, TOKGRAPHIC);
	skipSection();

	strLine = getLine();
	checkEqual(strLine, TOKPARAMETERS);
	QList < modelParameter * >params = parseParameters();

	strLine = getLine();
	modelAtomic *ret = new modelAtomic();
  if (strLine == TOKEXTRA) {
			ret->extra = parseExtra();
	    strLine = getLine();
  }
	checkEqual(strLine, TOKCBRACE);

	ret->inPorts = inPorts;
	ret->outPorts = outPorts;
	ret->name = name;
	ret->path = path;
	ret->desc = desc;
	ret->params = params;
	return ret;
}

modelPort *parseInport()
{

	modelPort *ret = new modelPort();
	ret->type = INPORT;
	QString strLine = getLine();
	checkEqual(strLine, TOKOBRACE);

	strLine = getLine();
	QString name = getValue(strLine, TOKNAME);

	strLine = getLine();
	QString coupledport = getValue(strLine, TOKCOUPLEDPORT);

	strLine = getLine();
	QString description = getValue(strLine, TOKDESCRIPTION);

	strLine = getLine();
	checkEqual(strLine, TOKGRAPHIC);
	skipSection();

	strLine = getLine();
	checkEqual(strLine, TOKCBRACE);
	ret->name = name;
	ret->coupledPort = coupledport.trimmed().toInt();
	return ret;
}

modelPort *parseOutport()
{
	modelPort *ret = new modelPort();

	ret->type = OUTPORT;
	QString strLine = getLine();
	checkEqual(strLine, TOKOBRACE);

	strLine = getLine();
	QString name = getValue(strLine, TOKNAME);

	strLine = getLine();
	QString coupledport = getValue(strLine, TOKCOUPLEDPORT);

	strLine = getLine();
	QString description = getValue(strLine, TOKDESCRIPTION);

	strLine = getLine();
	checkEqual(strLine, TOKGRAPHIC);
	skipSection();

	strLine = getLine();
	checkEqual(strLine, TOKCBRACE);
	ret->name = name;
	ret->coupledPort = coupledport.trimmed().toInt();
	return ret;
}

void checkLinesWithPoints(modelCoupled *c)
{
  QVector<QList<int> > pointsL(c->lines.length());
  for( int i=0;i< c->lines.length();i++)
  {
    if (c->lines.at(i)->sourceType == TOKPNT)
      pointsL[c->lines.at(i)->sources.first()-1].append(i+1);
    if (c->lines.at(i)->sinkType == TOKPNT)
      pointsL[c->lines.at(i)->sinks.first()-1].append(i+1);
  } 
  for( int i=0;i< c->points.length();i++)
  {
    QList<int> points = c->points.at(i)->lines;
    qSort(points);
    qSort(pointsL[i]);
    if (points!=pointsL[i])
    {
      qDebug() << "Points " << i << " has lines connected " << pointsL[i];
      qDebug() << "-->Points " << i << " has lines connected " << points;
      QMessageBox::critical(NULL,"PowerDEVS", "The model is corrupt. Saving it may correct this.");
      exit(-1);
    }
  }
  
}

modelCoupled *parseCoupled()
{
	bool ok;
	int inPorts, outPorts;
	modelCoupled *ret = new modelCoupled();
	QList < modelChild * >childs;
	QList < modelPoint * >points;
	QList < modelLine * >lines;
	QList < modelPort * >lsPorts;

	QString strLine = getLine();
	checkEqual(strLine, TOKOBRACE);
	strLine = getLine();
	QString type = getValue(strLine, TOKTYPE);
	strLine = getLine();
	QString name = getValue(strLine, TOKNAME);
	//printf("Parsing a coupled named:%s\n", QSTR(name));

	strLine = getLine();
	QString ports = getValue(strLine, TOKPORTS);
	QStringList slports = ports.split(TOKCOLON, QString::SkipEmptyParts);
	inPorts = slports.first().toInt(&ok);
	outPorts = slports.last().toInt(&ok);

	strLine = getLine();
	QString desc = getValue(strLine, TOKDESCRIPTION);

	strLine = getLine();
	checkEqual(strLine, TOKGRAPHIC);
	skipSection();

	strLine = getLine();
	checkEqual(strLine, TOKPARAMETERS);
	QList < modelParameter * >params = parseParameters();

	strLine = getLine();
	checkEqual(strLine, TOKSYSTEM);
	strLine = getLine();
	checkEqual(strLine, TOKOBRACE);
	do {
		strLine = getLine();
		if (strLine == TOKATOMIC) {
			modelChild *c = new modelChild();
			c->childType = ATOMIC;
			c->atomic = parseAtomic();
			c->atomic->father = ret;
			childs.append(c);
		}
		if (strLine == TOKCOUPLED) {
			modelChild *c = new modelChild();
			c->childType = COUPLED;
			c->coupled = parseCoupled();
			c->coupled->father = ret;
			childs.append(c);
		}
		if (strLine == TOKINPORT) {
			lsPorts.append(parseInport());
		}
		if (strLine == TOKOUTPORT) {
			lsPorts.append(parseOutport());
		}
		if (strLine == TOKPOINT) {
			points.append(parsePoint());
		}
		if (strLine == TOKLINE) {
			lines.append(parseLine());
		}
		if (strLine == TOKEXTRA) {
			ret->extra = parseExtra();
    }
	} while (strLine != TOKCBRACE);
	strLine = getLine();
	checkEqual(strLine, TOKCBRACE);

	ret->name = name;
	ret->type = type;
	ret->lsPorts = lsPorts;
	ret->params = params;
	ret->childs = childs;
	ret->points = points;
	ret->lines = lines;

  checkLinesWithPoints(ret);
	return ret;
}

modelCoupled *parseModel(QString filename)
{
	fd = new QFile(filename);
	if (!(filename.endsWith(".pdm", Qt::CaseInsensitive))) {
		printf("File must be either a .pdm or .pds\n");
		exit(-1);
	}
	if (!fd->open(QIODevice::ReadOnly | QIODevice::Text)) {
		printf("Error opening file %s\n",
		       filename.toLatin1().constData());
		exit(-1);
	}
	QString strLine = getLine();
	checkEqual(strLine, TOKCOUPLED);
	modelCoupled *r = parseCoupled();
	fd->close();
	return r;
}

modelAtomic *parseSimulator()
{

	modelAtomic *ret = new modelAtomic();

	QString strLine = getLine();
	checkEqual(strLine, TOKOBRACE);

	strLine = getLine();
	ret->path = getValue(strLine, TOKPATH);
  ret->path = ret->path.replace("Continuous","continuous");
  ret->path = ret->path.replace("Discrete","discrete");
  ret->path = ret->path.replace("Hybrid","hybrid");
  ret->path = ret->path.replace("Qss","qss");
  ret->path = ret->path.replace("Realtime","realtime");
  ret->path = ret->path.replace("Sources","sources");
  ret->path = ret->path.replace("Sinks","sinks");
  ret->path = ret->path.replace("Source","source");
  ret->path = ret->path.replace("Sink","sink");

	strLine = getLine();
	ret->paramsString = getValue(strLine, TOKPARAMETERS);

	strLine = getLine();
	checkEqual(strLine, TOKCBRACE);

	return ret;
}

QList < modelConnection * >parseConnections()
{
	QList < modelConnection * >ret;

	QString strLine = getLine();
	checkEqual(strLine, TOKOBRACE);

	strLine = getLine();
	while (strLine != TOKCBRACE) {
		QStringList lsLine = strLine.remove("(").remove(")").split(";");
		QStringList lsSource = lsLine.first().split(",");
		QStringList lsSink = lsLine.at(1).split(",");
		modelConnection *c = new modelConnection();
		c->childSource = lsSource.first().toInt();
		c->sourcePort = lsSource.at(1).toInt();
		c->childSink = lsSink.first().toInt();
		c->sinkPort = lsSink.at(1).toInt();
		ret.append(c);

		strLine = getLine();
	}

	checkEqual(strLine, TOKCBRACE);
	return ret;
}

modelCoupled *parseCoordinator()
{
	modelCoupled *ret = new modelCoupled();

	QString strLine = getLine();
	checkEqual(strLine, TOKOBRACE);

	do {
		strLine = getLine();
		if (strLine.trimmed() == TOKSIMULATOR) {
			modelChild *c = new modelChild();
			c->childType = ATOMIC;
			c->atomic = parseSimulator();
			ret->childs.append(c);
		} else if (strLine.trimmed() == TOKCOORDINATOR) {
			modelChild *c = new modelChild();
			c->childType = COUPLED;
			c->coupled = parseCoordinator();
			ret->childs.append(c);
		} else if (strLine.trimmed() == TOKEIC) {
			QList < modelConnection * >l = parseConnections();
			ret->lsEIC = l;
		} else if (strLine.trimmed() == TOKEOC) {
			QList < modelConnection * >l = parseConnections();
			ret->lsEOC = l;
		} else if (strLine.trimmed() == TOKIC) {
			QList < modelConnection * >l = parseConnections();
			ret->lsIC = l;
		} else if (strLine != TOKCBRACE) {
			printf("Expecting {%s, %s, %s, %s, %s}: found %s\n",
			       TOKSIMULATOR, TOKCOORDINATOR, TOKEIC, TOKEOC,
			       TOKIC, QSTR(strLine));
			exit(-1);
		}
	} while (strLine != TOKCBRACE);

	return ret;
}

modelCoupled *parsePDS(QString filename)
{
	fd = new QFile(filename);
	if (!filename.endsWith(".pds", Qt::CaseInsensitive)) {
		printf("File must be either a .pdm or .pds\n");
		exit(-1);
	}
	if (!fd->open(QIODevice::ReadOnly | QIODevice::Text)) {
		printf("Error opening file %s\n",
		       filename.toLatin1().constData());
		exit(-1);
	}
	QString strLine = getLine();
	checkEqual(strLine, TOKROOTCOORD);
	modelCoupled *r = parseCoordinator();
	fd->close();
	return r;
}
