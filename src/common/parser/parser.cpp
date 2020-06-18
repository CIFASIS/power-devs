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
#include "parser.h"
#include <vector>

using namespace std;


int lineNumber;
int version = 2;
QFile *fd;
QString getLine()
{
	lineNumber++;
	if (fd->atEnd()) {
		printf("ERROR: End of file reached\n");
		return QString("");
	}
	QString line=(fd->readLine()).trimmed();
  if (line.contains("//")) {
    return line.left(line.indexOf("//")).trimmed();
  }
  //qDebug() << "Reading " << line;
	// Chequae fin de archivo!!!
	return line;
}

qint64 writeLine(QString s, int tab)
{
	return fd->write(TAB(tab) + s.toLatin1() + "\n");
}

int checkEqual(QString s, QString t)
{
	if (s != t) {
		printf("Error: Expecting: '%s' found: '%s' at line %d\n",
		       QSTR(t), QSTR(s), lineNumber);
		return 0;
	}
	return 1;
}

vector<string> parseExtra() {
  vector<string> ret;
  int i=0;
	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return ret;
	do {
		//printf("SKIP: %s\n",strLine.toLatin1().constData());
		strLine = getLine();
	  if (strLine == TOKCBRACE) 
      break;
    ret.resize(i+1);
    ret[i++] = string(qPrintable(strLine.trimmed()));
	} while (strLine != TOKCBRACE);
  return ret;
}

void skipSection()
{
	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return;
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
		return NULL;
	}
	return s.mid(s.indexOf(TOKEQUAL) + 1).trimmed();
}

/*
bool writeParameters(int depth, QList < modelParameter * >p);
bool writeGraphics(int depth, modelGraphics * g, bool forCoupled);
bool writeCoupled(int depth, modelCoupled * c);
*/
Line *parseLine()
{

	Line *ret = new Line();

	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return NULL;

	strLine = getLine();
	QString source = getValue(strLine, TOKSOURCE);

	strLine = getLine();
	QString sink = getValue(strLine, TOKSINK);

	strLine = getLine();
	QString pointxstr = getValue(strLine, TOKPOINTX);
	QStringList pointXls =
	    pointxstr.split(TOKCOLON, QString::SkipEmptyParts);
	QStringList::iterator i;
	vector< int >pointX;
	for (i = pointXls.begin(); i != pointXls.end(); ++i)
		pointX.push_back(i->toInt()/TWIPS_TO_PIXEL);

	strLine = getLine();
	QString pointystr = getValue(strLine, TOKPOINTY);
	QStringList pointYls =
	    pointystr.split(TOKCOLON, QString::SkipEmptyParts);
	vector< int >pointY;
	for (i = pointYls.begin(); i != pointYls.end(); ++i)
		pointY.push_back(i->toInt()/TWIPS_TO_PIXEL);
	ret->setPointsXY(pointX,pointY);

	strLine = getLine();
	if (!checkEqual(strLine, TOKCBRACE))
		return NULL;

	QStringList src = source.split(TOKCOLON, QString::SkipEmptyParts);
	QString srcType  = src.takeFirst().trimmed();
	if (srcType=="Cmp")
		ret->setSourceType(Line::COMPONENT); 
	else if (srcType=="Prt")
		ret->setSourceType(Line::PORT); 
	else if (srcType=="Pnt")
		ret->setSourceType(Line::NODE); 
	else 
		ret->setSourceType(Line::CROSS); 

	vector< int >sources;
	for (i = src.begin(); i != src.end(); ++i)
		sources.push_back(i->toInt());
	ret->setSources(sources);

	src = sink.split(TOKCOLON, QString::SkipEmptyParts);
	QString sinkType = src.takeFirst().trimmed();
	if (sinkType=="Cmp")
		ret->setSinkType(Line::COMPONENT); 
	else if (sinkType=="Prt")
		ret->setSinkType(Line::PORT); 
	else if (sinkType=="Pnt")
		ret->setSinkType(Line::NODE); 
	else 
		ret->setSinkType(Line::CROSS); 
	vector< int >sinks;
	for (i = src.begin(); i != src.end(); ++i)
		sinks.push_back(i->toInt());
	ret->setSink(sinks);
	return ret;
}

Point *parsePoint()
{

	Point *ret = new Point();
	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return NULL;

	strLine = getLine();
	QString connectedlines = getValue(strLine, TOKCONECTEDLINES);

	strLine = getLine();
	QString connectedextrems = getValue(strLine, TOKCONECTEDEXTREMS);

	strLine = getLine();
	QStringList lposition = getValue(strLine, TOKPOSITION).split(TOKCOLON);
	ret->graphic().setX(lposition.first().toInt()/TWIPS_TO_PIXEL);
	ret->graphic().setY(lposition.at(1).toInt()/TWIPS_TO_PIXEL);

	strLine = getLine();
	if (!checkEqual(strLine, TOKCBRACE))
		return NULL;

	QStringList t = connectedlines.split(TOKCOLON, QString::SkipEmptyParts);
	QStringList::iterator i;
	vector< int >ls;
	for (i = t.begin(); i != t.end(); ++i)
		ls.push_back(i->toInt());
	ret->setLines(ls);
	ls.clear();
	t = connectedextrems.split(TOKCOLON, QString::SkipEmptyParts);
	for (i = t.begin(); i != t.end(); ++i)
  {
		ls.push_back(QString(*i).trimmed()=="Dst" ? 0 : 1);
  }
	ret->setExtrems(ls);
	return ret;
}

QStringList unescapeParams(QString str)
{
  QStringList ret,lsv;
  QString val;
  lsv = str.split(TOKCOLON);
  if (lsv.size()<3)
    return ret;
  ret << lsv.takeFirst();
  ret << lsv.takeLast();
  val = lsv.join(QString(";"));
  val = val.replace("\\;",";");
  ret.insert(1,val);
  return ret;

}

vector<Parameter*> parseParameters()
{
	vector<Parameter*> ret;
	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return vector<Parameter*>();
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
    if (lsv.size()<3) {
      printf("Error while parsing parameters\n");
      return vector<Parameter*>();
    }
		type = lsv.takeFirst().trimmed();
		value = lsv.takeFirst().trimmed();
		desc = lsv.takeFirst().trimmed();
		if (type == TOKVAL) {
      ValueParameter *vp = new ValueParameter(qPrintable(name),qPrintable(desc),value.toDouble());
		  ret.push_back(vp);
		} else if (type == TOKSTR) {
      StringParameter *sp = new StringParameter(qPrintable(name),qPrintable(desc),qPrintable(value));
		  ret.push_back(sp);
		} else if (type == TOKLST) {
			QStringList qs = value.split(TOKPERC, QString::SkipEmptyParts);
      vector<string> string_list;
			unsigned selected = qs.takeFirst().trimmed().toInt();
      foreach (QString s,qs)
      {
       string_list.push_back(qPrintable(s));
      }
      ListParameter *lp = new ListParameter(qPrintable(name),qPrintable(desc),string_list,selected);
		  ret.push_back(lp);
		}
		strLine = getLine();
		lsn.clear();
	}
	return ret;
}

Graphic *parseGraphics(bool forCoupled)
{

	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return NULL;

	strLine = getLine();
	QString position = getValue(strLine, TOKPOSITION);
	QStringList slpos = position.split(TOKCOLON, QString::SkipEmptyParts);

	strLine = getLine();
	QString dimension = getValue(strLine, TOKDIMENSION);
	QStringList sldim = dimension.split(TOKCOLON, QString::SkipEmptyParts);

	strLine = getLine();
	QString direction = getValue(strLine, TOKDIRECTION);
	QStringList l;
	l << TOKRIGHT << TOKDOWN << TOKLEFT << TOKUP;

	Graphic *ret = new Graphic();
	strLine = getLine();
	ret->setColor(getValue(strLine, TOKCOLOR).toInt());

	strLine = getLine();
	ret->setIcon(qPrintable(getValue(strLine, TOKICON)));

	if (forCoupled) {

		strLine = getLine();
		// Parse window data
	}
	strLine = getLine();
	if (!checkEqual(strLine, TOKCBRACE))
		return NULL;
  ret->setX(slpos.first().toInt()/TWIPS_TO_PIXEL);
  ret->setY(slpos.at(1).toInt()/TWIPS_TO_PIXEL);
  ret->setWidth(sldim.first().toInt()/TWIPS_TO_PIXEL);
  ret->setHeight(sldim.at(1).toInt()/TWIPS_TO_PIXEL);
	ret->setDirection((Graphic::Direction)l.indexOf(direction.trimmed()));

	return ret;
};


Atomic *parseAtomic()
{
	int inPorts, outPorts;
	bool ok;
	Graphic *graphics;
	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return NULL;

	strLine = getLine();
	QString name = getValue(strLine, TOKNAME);

	strLine = getLine();
	QString ports = getValue(strLine, TOKPORTS);
	QStringList slports = ports.split(TOKCOLON, QString::SkipEmptyParts);
	inPorts = slports.first().toInt(&ok);
	outPorts = slports.last().toInt(&ok);
	strLine = getLine();
	QString path = getValue(strLine, TOKPATH);

	strLine = getLine();
	QString desc = getValue(strLine, TOKDESCRIPTION);

	strLine = getLine();
	if (!checkEqual(strLine, TOKGRAPHIC))
		return NULL;
	graphics = parseGraphics(false);

	strLine = getLine();
	if (!checkEqual(strLine, TOKPARAMETERS))
		return NULL;
	vector< Parameter * >params = parseParameters();
  

	strLine = getLine();
  Atomic *ret = new Atomic(NULL,qPrintable(name),qPrintable(desc),qPrintable(path));
  if (strLine == TOKEXTRA) {
			ret->setExtra(parseExtra());
	    strLine = getLine();
  }
  if (!checkEqual(strLine, TOKCBRACE))
		return NULL;

  ret->setGraphics(*graphics);
  ret->setInports(inPorts);
  ret->setOutports(outPorts);
	ret->setParameters(params);
  delete graphics;
	return ret;
}

Port *parseInport()
{

	Port *ret = new Port();
  ret->setType(Port::Inport);
	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return NULL;

	strLine = getLine();
	QString name = getValue(strLine, TOKNAME);

	strLine = getLine();
	QString coupledport = getValue(strLine, TOKCOUPLEDPORT);

	strLine = getLine();
	QString description = getValue(strLine, TOKDESCRIPTION);

	strLine = getLine();
	if (!checkEqual(strLine, TOKGRAPHIC))
		return NULL;

	strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return NULL;

	strLine = getLine();
	QString position = getValue(strLine, TOKPOSITION);
  ret->graphic().setX(position.split(";").first().toInt()/TWIPS_TO_PIXEL);
  ret->graphic().setY(position.split(";").last().toInt()/TWIPS_TO_PIXEL);
	strLine = getLine();
	QString dimension = getValue(strLine, TOKDIMENSION);
	ret->graphic().setWidth(dimension.toInt()/TWIPS_TO_PIXEL);
	//qDebug("Dimension %d",ret->dimension);

	strLine = getLine();
	QString direction = getValue(strLine, TOKDIRECTION);
	QStringList l;
	l << TOKRIGHT << TOKDOWN << TOKLEFT << TOKUP;
  ret->graphic().setDirection((Graphic::Direction)l.indexOf(direction.trimmed()));

	strLine = getLine();
	if (!checkEqual(strLine, TOKCBRACE))
		return NULL;

	strLine = getLine();
	if (!checkEqual(strLine, TOKCBRACE))
		return NULL;
	ret->setName(qPrintable(name));
	return ret;
}

Port *parseOutport()
{
	Port *ret = new Port();
  ret->setType(Port::Outport);

	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return NULL;

	strLine = getLine();
	QString name = getValue(strLine, TOKNAME);

	strLine = getLine();
	QString coupledport = getValue(strLine, TOKCOUPLEDPORT);
  ret->setCoupledPort(coupledport.toInt());

	strLine = getLine();
	QString description = getValue(strLine, TOKDESCRIPTION);

	strLine = getLine();
	if (!checkEqual(strLine, TOKGRAPHIC))
		return NULL;

	strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return NULL;

	strLine = getLine();
	QString position = getValue(strLine, TOKPOSITION);
  ret->graphic().setX(position.split(";").first().toInt()/TWIPS_TO_PIXEL);
  ret->graphic().setY(position.split(";").last().toInt()/TWIPS_TO_PIXEL);

	strLine = getLine();
	QString dimension = getValue(strLine, TOKDIMENSION);
	ret->graphic().setWidth(dimension.toInt()/TWIPS_TO_PIXEL);

	strLine = getLine();
	QString direction = getValue(strLine, TOKDIRECTION);
	QStringList l;
	l << TOKRIGHT << TOKDOWN << TOKLEFT << TOKUP;
  ret->graphic().setDirection((Graphic::Direction)l.indexOf(direction.trimmed()));

	strLine = getLine();
	if (!checkEqual(strLine, TOKCBRACE))
		return NULL;

	strLine = getLine();
	if (!checkEqual(strLine, TOKCBRACE))
		return NULL;
	ret->setName(qPrintable(name));
	return ret;
}
/*

bool writeAtomic(int depth, modelAtomic * a)
{

	writeLine(TOKATOMIC, depth);
	writeLine(TOKOBRACE, depth + 1);
	writeLine(TOKNAME + QString(" = ") + a->name, depth + 1);
	writeLine(TOKPORTS + QString(" = ") +
		  QString("%1 ; %2").arg(a->inPorts).arg(a->outPorts),
		  depth + 1);
	writeLine(TOKPATH + QString(" = ") + a->path, depth + 1);
	writeLine(TOKDESCRIPTION + QString(" = ") + a->desc, depth + 1);
	writeGraphics(depth + 1, a->graphics, false);
	writeParameters(depth + 1, a->params);
	writeLine(TOKCBRACE, depth + 1);
}

bool writeLines(int depth, modelLine * l)
{
	writeLine(TOKLINE, depth);
	writeLine(TOKOBRACE, depth + 1);
	QString src = " = ";
	src.append(l->sourceType + " ; ");
	QList < int >::iterator i;
	for (i = l->sources.begin(); i != l->sources.end(); i++) {
		src.append(QString("%1").arg(*i));
		if (i + 1 != l->sources.end())
			src.append(" ; ");
	}
	QString snk = " = ";
	snk.append(l->sinkType + " ; ");
	for (i = l->sinks.begin(); i != l->sinks.end(); i++) {
		snk.append(QString("%1").arg(*i));
		if (i + 1 != l->sinks.end())
			snk.append(" ; ");
	}
	QString px = " = ";
	for (i = l->pointX.begin(); i != l->pointX.end(); i++) {
		px.append(QString("%1").arg(*i));
		if (i + 1 != l->pointX.end())
			px.append(" ; ");
	}
	QString py = " = ";
	for (i = l->pointY.begin(); i != l->pointY.end(); i++) {
		py.append(QString("%1").arg(*i));
		if (i + 1 != l->pointY.end())
			py.append(" ; ");
	}
	writeLine(TOKSOURCE + src, depth + 1);
	writeLine(TOKSINK + snk, depth + 1);
	writeLine(TOKPOINTX + px, depth + 1);
	writeLine(TOKPOINTY + py, depth + 1);
	writeLine(TOKCBRACE, depth + 1);
}

bool writePoints(int depth, modelPoint * p)
{

	writeLine(TOKPOINT, depth);
	writeLine(TOKOBRACE, depth + 1);
	QString cl = " = ";
	foreach(int l, p->lines) {
		cl.append(QString("%1").arg(l));
		if (l != p->lines.last())
			cl.append(" ; ");
	}
	writeLine(TOKCONECTEDLINES + cl, depth + 1);
	writeLine(TOKCONECTEDEXTREMS +
		  QString(" = %1").arg(p->extrems.join(";")), depth + 1);
	writeLine(TOKPOSITION + QString(" = %1 ; %2 ").arg(p->x).arg(p->y),
		  depth + 1);
	writeLine(TOKCBRACE, depth + 1);
}

bool writePorts(int depth, modelPort * mp)
{
	QStringList l;
	l << TOKRIGHT << TOKDOWN << TOKLEFT << TOKUP;
	if (mp->type == INPORT)
		writeLine(TOKINPORT, depth);
	else
		writeLine(TOKOUTPORT, depth);
	writeLine(TOKOBRACE, depth + 1);
	writeLine(TOKNAME + QString(" = ") + mp->name, depth + 1);
	writeLine(TOKCOUPLEDPORT + QString(" = %1").arg(mp->coupledPort),
		  depth + 1);
	writeLine(TOKDESCRIPTION + QString(" = ") + mp->desc, depth + 1);
	writeLine(TOKGRAPHIC, depth + 1);
	writeLine(TOKOBRACE, depth + 2);
	writeLine(TOKPOSITION + QString(" = %1 ; %2").arg(mp->x).arg(mp->y),
		  depth + 2);
	writeLine(TOKDIMENSION + QString(" = %1").arg(mp->dimension),
		  depth + 2);
	writeLine(TOKDIRECTION + QString(" = ") + l.at(mp->direction),
		  depth + 2);
	writeLine(TOKCBRACE, depth + 2);
	writeLine(TOKCBRACE, depth + 1);
}

bool writeSystem(int depth, QList < modelChild * >c, QList < modelPoint * >p,
		 QList < modelLine * >l, QList < modelPort * >lp)
{

	writeLine(TOKSYSTEM, depth);
	writeLine(TOKOBRACE, depth + 1);
	foreach(modelChild * mc, c)
	    if (mc->childType == ATOMIC)
		writeAtomic(depth + 1, mc->atomic);
	else
		writeCoupled(depth + 1, mc->coupled);
	foreach(modelPort * mp, lp)
	    writePorts(depth + 1, mp);
	foreach(modelPoint * mp, p)
	    writePoints(depth + 1, mp);
	foreach(modelLine * ml, l)
	    writeLines(depth + 1, ml);
	writeLine(TOKCBRACE, depth + 1);
}

bool writeParameters(int depth, QList < modelParameter * >p)
{

	writeLine(TOKPARAMETERS, depth);
	writeLine(TOKOBRACE, depth + 1);
	foreach(modelParameter * mp, p) {
		QString s;
		s = mp->name + QString(" = ") + (mp->type ==
						 STR ? TOKSTR : (mp->type ==
								 LST ? TOKLST :
								 TOKVAL));
		if (mp->type == STR)
			s = s.append("; " + mp->strValue + " ; ");
		if (mp->type == VAL)
			s = s.append("; " + QString("%1").arg(mp->dValue) +
				     " ; ");
		if (mp->type == LST) {
			s = s.append("; " + QString("%1").arg(mp->lsValue) +
				     "\%");
			foreach(QString st, mp->lsValues)
			    s = s.append(st + "\%");
			s = s.append(" ; ");
		}
		s = s.append(mp->desc);
		writeLine(s, depth + 1);
	}
	writeLine(TOKCBRACE, depth + 1);
}

bool writeGraphics(int depth, modelGraphics * g, bool forCoupled)
{

	QStringList l;
	l << TOKRIGHT << TOKDOWN << TOKLEFT << TOKUP;
	if (g->direction<0 || g->direction>3)
		g->direction=0;

	writeLine(TOKGRAPHIC, depth);
	writeLine(TOKOBRACE, depth + 1);
	writeLine(TOKPOSITION + QString(" = %1 ; %2").arg(g->x).arg(g->y),
		  depth + 1);
	writeLine(TOKDIMENSION +
		  QString(" = %1 ; %2").arg(g->width).arg(g->height),
		  depth + 1);
	writeLine(TOKDIRECTION + QString(" = ") + l.at(g->direction),
		  depth + 1);
	writeLine(TOKCOLOR + QString(" = %1").arg(g->color), depth + 1);
	writeLine(TOKICON + QString(" = ") +
		  (g->icon.isEmpty()? TOKNONE : g->icon), depth + 1);
	if (forCoupled) {	// TODO
		writeLine(TOKWINDOW + QString(" = 5000; 5000; 5000; 5000 "),
			  depth + 1);
	}
	writeLine(TOKCBRACE, depth + 1);
}

bool writeCoupled(int depth, modelCoupled * c)
{
	writeLine(TOKCOUPLED, depth);
	writeLine(TOKOBRACE, depth + 1);
	writeLine(TOKTYPE + QString(" = ") + c->type, depth + 1);
	writeLine(TOKNAME + QString(" = ") + c->name, depth + 1);
	writeLine(TOKPORTS + QString(" = ") +
		  QString("%1; %2").arg(c->lsInPorts.count()).arg(c->lsOutPorts.count()),
		  depth + 1);
	writeLine(TOKDESCRIPTION + QString(" = ") + c->desc, depth + 1);
	writeGraphics(depth + 1, c->graphics, true);
	writeParameters(depth + 1, c->params);
	writeSystem(depth + 1, c->childs, c->points, c->lines, c->lsInPorts+c->lsOutPorts);
	writeLine(TOKCBRACE, depth + 1);

	return true;

}

*/

Coupled *parseCoupled()
{
	bool ok;
	int inPorts, outPorts;
	Graphic *graphics;
	QList < Model * >childs;
	QList < Point * >points;
	QList < Line * >lines;

	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return NULL;
	strLine = getLine();
	QString type = getValue(strLine, TOKTYPE);
	Coupled *ret = new Coupled();
	if (type == TOKROOT)
		ret->setFather(NULL);
	strLine = getLine();
	QString name = getValue(strLine, TOKNAME);
  ret->setName(qPrintable(name));
	//printf("Parsing a coupled named:%s of type %s\n", QSTR(name),QSTR(type));

	strLine = getLine();
	QString ports = getValue(strLine, TOKPORTS);
	QStringList slports = ports.split(TOKCOLON, QString::SkipEmptyParts);
	inPorts = slports.first().toInt(&ok);
	outPorts = slports.last().toInt(&ok);
  ret->setInports(inPorts);
  ret->setOutports(outPorts);

	strLine = getLine();
	QString desc = getValue(strLine, TOKDESCRIPTION);

	strLine = getLine();
	if (!checkEqual(strLine, TOKGRAPHIC))
		return NULL;
	graphics = parseGraphics(true);

	strLine = getLine();
	if (!checkEqual(strLine, TOKPARAMETERS))
		return NULL;
	vector< Parameter * >params = parseParameters();

	strLine = getLine();
	if (!checkEqual(strLine, TOKSYSTEM))
		return NULL;
	strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return NULL;
	do {
		strLine = getLine();
		if (strLine == TOKATOMIC) {
      Atomic *a = parseAtomic();
      if (a==NULL)
        return NULL;
      a->setFather(ret);
			ret->addChild(a);
		} else if (strLine == TOKCOUPLED) {
			Coupled *cp= parseCoupled();
      if (cp==NULL)
        return NULL;
      qDebug() << "Cp = " << cp;
			cp->setFather(ret);
			ret->addChild(cp);
		} else if (strLine == TOKINPORT) {
			ret->addInport(parseInport());
		} else if (strLine == TOKOUTPORT) {
		  ret->addOutport(parseOutport());
		} else if (strLine == TOKPOINT) {
			ret->addPoint(parsePoint());
		} else if (strLine == TOKLINE) {
			ret->addLine(parseLine());
		} else if (strLine == TOKEXTRA) {
			ret->setExtra(parseExtra());
		} else if (strLine != TOKCBRACE) {	// Error
      qDebug() << strLine << "ERRRORRRRRRRR";
			return NULL;
		}
	} while (strLine != TOKCBRACE);
	strLine = getLine();
	if (!checkEqual(strLine, TOKCBRACE))
		return NULL;
	ret->setGraphic(*graphics);
  delete graphics;

	ret->setName(qPrintable(name));
	ret->setDescription(qPrintable(desc));
	ret->setParameters(params);

	return ret;
}

Atomic *parseSimulator()
{

	Atomic *ret = new Atomic();

	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return NULL;

	strLine = getLine();

	strLine = getLine();

	strLine = getLine();
	if (!checkEqual(strLine, TOKCBRACE))
		return NULL;

	return NULL;
}

QList < Connection * >parseConnections()
{
	QList < Connection * >ret;

	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return QList < Connection * >();

	strLine = getLine();
	while (strLine != TOKCBRACE) {
		QStringList lsLine = strLine.remove("(").remove(")").split(";");
		QStringList lsSource = lsLine.first().split(",");
		QStringList lsSink = lsLine.at(1).split(",");
		Connection *c = new Connection();
		c->sourceChild = lsSource.first().toInt();
		c->sourcePort = lsSource.at(1).toInt();
		c->sinkChild = lsSink.first().toInt();
		c->sinkPort = lsSink.at(1).toInt();
		ret.append(c);

		strLine = getLine();
	}

	if (!checkEqual(strLine, TOKCBRACE))
		return QList < Connection * >();
	return ret;
}

Coupled *parseCoordinator()
{
	Coupled *ret = new Coupled();

	QString strLine = getLine();
	if (!checkEqual(strLine, TOKOBRACE))
		return NULL;

	do {
		strLine = getLine();
		if (strLine.trimmed() == TOKSIMULATOR) {
			ret->addChild(parseSimulator());
		} else if (strLine.trimmed() == TOKCOORDINATOR) {
			ret->addChild(parseCoordinator());
		} else if (strLine.trimmed() == TOKEIC) {
			QList < Connection * >l = parseConnections();
#ifdef USE_CONNECTIONS
			ret->lsEIC = l;
#endif
		} else if (strLine.trimmed() == TOKEOC) {
			QList < Connection * >l = parseConnections();
#ifdef USE_CONNECTIONS
			ret->lsEOC = l;
#endif
		} else if (strLine.trimmed() == TOKIC) {
			QList < Connection * >l = parseConnections();
#ifdef USE_CONNECTIONS
			ret->lsIC = l;
      qDebug() << "I found " << l.size() << " connections";
#endif
		} else if (strLine != TOKCBRACE) {
			printf("Expecting {%s, %s, %s, %s, %s}: found %s\n",
			       TOKSIMULATOR, TOKCOORDINATOR, TOKEIC, TOKEOC,
			       TOKIC, QSTR(strLine));
			exit(-1);
		}
	} while (strLine != TOKCBRACE);

	return ret;
}
/*
bool saveModelToFile(modelCoupled * c, QString filename)
{
	qDebug("Saving model to file %s", qPrintable(filename));
	fd = new QFile(filename);
	if (!fd->open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}
	writeCoupled(0, c);
	fd->close();
	return true;
}
*/

Coupled *parsePDS(QString filename)
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
	if (!checkEqual(strLine, TOKROOTCOORD))
		return NULL;
	Coupled *r = parseCoordinator();
	fd->close();
	return r;
}

Coupled *parseModel(QString filename)
{
	fd = new QFile(filename);
	if (!fd->open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug("Error opening file %s\n",
		       filename.toLatin1().constData());
		return NULL;
	}
	QString strLine = getLine();
	if (!checkEqual(strLine, TOKCOUPLED))
		return NULL;
	Coupled *r = parseCoupled();
	fd->close();
	delete fd;
	return r;
}


