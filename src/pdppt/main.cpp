/****************************************************************************
**
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**  Universidad Nacional de Rosario - Argentina.
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

#include <stdio.h>
#include <stdlib.h>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QPushButton>

#include "parser.h"
#include "modelcoupled.h"
#include "codegenerator.h"

void showLog();
int main(int argc, char **argv)
{

	QApplication app(argc, argv);
  QStringList args=QCoreApplication::arguments();
	if (args.size()==1) {
		printf("Usage: pdppt [options] file\n");
		printf("	Options =  \n");
		printf("	           -r Only generate structure file\n");
		printf ("	           -m Generate structure and generate code\n");
		printf ("	           -x Generate structure, code and run\n");
		printf ("	           -s Generate structure, code and run silent\n");
		printf ("	           -f file Use the supplied Makefile\n");
		printf ("	           -pdif binary Use the supplied interface for running\n");
		printf ("	file can be a model (pdm) or a model structure(pds)\n");
		return 0;
	}
	// Parse command line args
	QString filename;
	QString makefilename("");
	QString pdif_command("../bin/pdif");
	bool generateCCode = true;
	bool silent = false;
	bool runSimulation = false;
  for (int i=1;i<args.size();i++) {
    if (args.at(i) == "-x") 
			runSimulation = true;
    else if (args.at(i) == "-m") 
			runSimulation = false;
    else if (args.at(i) == "-s") {
			silent = true;
			runSimulation = true;
    } else if (args.at(i)=="-f") {
      if (i+1<args.size())
        makefilename=args.at(++i);
    } else if (args.at(i)=="-pdif") {
      if (i+1<args.size())
        pdif_command=args.at(++i);
    } else filename = args.at(i);
	}
	// Hack to run under wine

	if (filename.contains(":\\powerdevs")) {
		filename = filename.mid(filename.indexOf("powerdevs") + 9).replace("\\", "/");
		filename = ".." + filename;
	}
	modelCoupled *c = NULL;
	if (filename.endsWith(".pdm", Qt::CaseInsensitive)) {
		c = parseModel(filename);
		generateCode(c, filename.replace(".pdm", ".pds", Qt::CaseInsensitive), generateCCode, true);
	} else if (filename.endsWith(".pds", Qt::CaseInsensitive)) {
		c = parsePDS(filename);
		generateCode(c, filename.replace(".pdm", ".pds", Qt::CaseInsensitive), generateCCode, false);
	} else {
		printf("File must be either a .pdm or .pds\n");
		return -1;
	}
  QString path = QCoreApplication::applicationDirPath();
  QDir::setCurrent(path + "/../build");
  QProcess make;
  QStringList argsmake;
  if (makefilename != "") {
    argsmake << "-f";
    argsmake << makefilename;
  }
  qDebug()<< "Calling make with " << argsmake;
#ifdef Q_OS_LINUX
  make.start("/usr/bin/make",argsmake);
#else
  QStringList env = QProcess::systemEnvironment();
  env << "PATH=" + path + "/gcc/bin" ;
  make.setEnvironment(env);
  make.start("../bin/gcc/bin/make.exe",argsmake);
#endif
  make.waitForFinished(-1);

    QByteArray log(make.readAllStandardError());
		if (make.exitCode() == 0) {
	    if (runSimulation) {
			  QDir::setCurrent(path + "/../output");
			  QProcess pdif;
#ifdef RTAIOS
			  pdif.startDetached("/usr/bin/kdesudo", QStringList() << pdif_command << filename.left(filename.  lastIndexOf(".")) + ".stm");
#else
			  if (!silent) {
#ifdef Q_OS_LINUX
          bool pdifRunning = system("ps -e | grep pdif") == 0;
          qDebug() << "PDIF running = " << pdifRunning;
          if (pdifRunning) {
            system("killall pdif");
          }
#endif
          if (pdif_command != "null") 
				    pdif.startDetached(pdif_command, QStringList() << filename.  left(filename.lastIndexOf(".")) + ".stm");
			  } else {
				  QFile stm(filename.left(filename.lastIndexOf(".")) + ".stm");
				  //qDebug() << (filename.left(filename.lastIndexOf(".")) + ".stm");
				  double tf,ti;
				  bool ok=false;
				  if (stm.open(QIODevice::ReadOnly | QIODevice::Text)) {
					  QString s(stm.readAll());
					  QStringList ls = s.split('\n');
					  ti = ls.first().trimmed().toDouble();
					  tf = ls.at(1).trimmed().toDouble(&ok);
				  }
				  if (ok)
				  	pdif.startDetached("./model", QStringList() << "-tf" << QString("%1").arg(tf));
				  else
					  pdif.startDetached(pdif_command, QStringList() << filename.  left(filename.lastIndexOf(".")) + ".stm");
	      }
			}
#endif
		} else {
			QMessageBox msgBox;
			msgBox.setWindowState(Qt::WindowNoState);
      
			//msgBox.setDetailedText(QString:: fromUtf8(log));
      QPushButton viewLog("View Compilation Log");
  
      msgBox.addButton(&viewLog,QMessageBox::HelpRole);
      msgBox.addButton(QMessageBox::Ok);
			msgBox.setText ("Error: The compilation process has reported an error.");
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setWindowTitle("PowerDEVS");
			msgBox.show();
			msgBox.showNormal();

      QFile logfile(path + "/../output/compile.log");
      logfile.open(QIODevice::WriteOnly);
      logfile.write(log);
      logfile.close();
      msgBox.exec();
			if (msgBox.clickedButton()==&viewLog) {
#ifndef Q_OS_WIN32
        QProcess::startDetached("/usr/bin/xdg-open", QStringList() << "../output/compile.log");
#else
        QProcess::startDetached("notepad", QStringList() << "../output/compile.log");
#endif
      }
			return -1;
		}
    QFile logfile(path + "/../output/compile.log");
    logfile.open(QIODevice::WriteOnly);
    logfile.write(log);
    logfile.close();
  	return 0;
}

void showLog() {

}
