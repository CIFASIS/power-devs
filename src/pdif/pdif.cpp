/***************************************************************************
o*
**  Copyright (C) 2009 Facultad de Ciencia Exactas Ingeniería y Agrimensura
**       	       Universidad Nacional de Rosario - Argentina.
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
#include <signal.h>
#include <errno.h>

#include <QVBoxLayout>
#include <QDialog>
#include <QBitmap>
#include <QDesktopWidget>
#include <QApplication>
#include <QThread>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QRegExp>
#include <QProcessEnvironment>

#include "pdif.h"

#define STEPTIMES 10000
void msg(QString m)
{
	QMessageBox msgBox;
	msgBox.setText(m);
	msgBox.setWindowTitle("PowerDEVS - Message");
	msgBox.exec();
}

 PDIF::PDIF(QWidget * parent):QDialog(parent)
{
	int x, y;
	int screenWidth;
	int screenHeight;
	int WIDTH = 220;
	int HEIGHT = 370;
	readtimes = false;
	state = STOPPED;
	percentage = 0;

	justFinished = false;
	setWindowIcon(QIcon(":/pd.png"));
	setWindowTitle("PowerDEVS");
	QDesktopWidget *desktop = QApplication::desktop();
	screenWidth = desktop->width();
	screenHeight = desktop->height();
	x = (screenWidth - WIDTH) / 2;
	y = (screenHeight - HEIGHT) / 2;
	setGeometry(x, y, WIDTH, HEIGHT);
	setFixedSize(QSize(WIDTH, HEIGHT));
	procStarted = false;

	layout = new QVBoxLayout;
	runButton = new QPushButton("Run Simulation");
	timedButton = new QPushButton("Run Timed");
	stepButton = new QPushButton("Step(s)");
	stopButton = new QPushButton("Stop");
	viewLogButton = new QPushButton("View Log");
	exitButton = new QPushButton("Exit");
	//closePlots = new QPushButton("Close GNUPlot windows");
	hLayoutWidget = new QWidget();
	hLayout = new QHBoxLayout(hLayoutWidget);
	label = new QLabel("Final Time");
	logo = new QLabel();
	runTimes = new QLabel("Run N simulations:");
	tf = new QLineEdit();
  tf->setValidator(new QDoubleValidator(this));
	pg = new QProgressBar();
	ilegitimal = new QLabel("Ilegitmate check\nBreak After");
	steps = new QLabel("Perform ");
	nmbsteps = new QLineEdit("100");
  nmbsteps->setValidator(new QIntValidator(this));
	runTimesEdit = new QLineEdit("1");
  runTimesEdit->setValidator(new QIntValidator(this));
	ilegitimalCondition = new QLineEdit("1000000");
  ilegitimalCondition->setValidator(new QIntValidator(this));
	hLayoutWidgetSteps = new QWidget();
	hLayoutWidgetTimes = new QWidget();
	hLayoutWidgetIleg = new QWidget();
	hLayoutSteps = new QHBoxLayout();
	hLayoutTimes = new QHBoxLayout();
	hLayoutIlegit = new QHBoxLayout();

	layout->setSizeConstraint(QLayout::SetFixedSize);
	stopButton->setDisabled(true);
	tf->setAlignment(Qt::AlignRight);
	ilegitimalCondition->setAlignment(Qt::AlignRight);
	ilegitimalCondition->setMinimumWidth(90);
	nmbsteps->setAlignment(Qt::AlignRight);
	nmbsteps->setMaximumWidth(50);
	runTimesEdit->setAlignment(Qt::AlignRight);
	runTimesEdit->setMaximumWidth(50);
	ilegitimalCondition->setMaximumWidth(50);
	pg->setAlignment(Qt::AlignCenter);
	pg->setMinimum(0);
	pg->setMaximum(100);
	logo->setPixmap(QPixmap(":/pdlogo.bmp"));
	logo->setAlignment(Qt::AlignCenter);

	strcpy(stmFile, "");

	hLayoutIlegit->addWidget(ilegitimal);
	hLayoutIlegit->addWidget(ilegitimalCondition);
	hLayoutWidgetIleg->setLayout(hLayoutIlegit);

	hLayoutSteps->addWidget(steps);
	hLayoutSteps->addWidget(nmbsteps);
	hLayoutSteps->addWidget(stepButton);
	hLayoutWidgetSteps->setLayout(hLayoutSteps);

	hLayoutTimes->addWidget(runTimes);
	hLayoutTimes->addWidget(runTimesEdit);
	hLayoutWidgetTimes->setLayout(hLayoutTimes);

	layout->addWidget(runButton);
	layout->addWidget(timedButton);

	layout->addWidget(stopButton);
	layout->addWidget(viewLogButton);
	//layout->addWidget(closePlots);
	layout->addWidget(exitButton);

	hLayout->addWidget(label);
	hLayout->addWidget(tf);

	layout->addWidget(hLayoutWidget);
	layout->addWidget(hLayoutWidgetTimes);
	layout->addWidget(hLayoutWidgetSteps);
	layout->addWidget(hLayoutWidgetIleg);
	layout->addWidget(pg);
	setLayout(layout);

	qProc = new QProcess();
	QObject::connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(exitModel()));

	QObject::connect(runButton, SIGNAL(clicked()), this,
			 SLOT(runSimulation()));
	QObject::connect(timedButton, SIGNAL(clicked()), this,
			 SLOT(runTimed()));
	QObject::connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
	QObject::connect(stopButton, SIGNAL(clicked()), this, SLOT(stopSimulation()));
	QObject::connect(stepButton, SIGNAL(clicked()), SLOT(stepSimulation()));
	QObject::connect(viewLogButton, SIGNAL(clicked()), SLOT(viewLog()));
	stopMode();
	startModel();
}

void PDIF::binaryFinish(int exitCode, QProcess::ExitStatus exitStatus)
{
	if (exitStatus != QProcess::NormalExit || exitCode!=0) {
    if (exitCode == 255) {
		  msg(QString("The process terminated abnormally. Opening Log"));
      viewLog();
      qApp->quit();
    }
#ifdef Q_OS_WIN32
		msg(QString("The process terminated abnormally.\n"));
#else
		msg(QString("The process terminated abnormally.\n%1").arg(strsignal(exitCode)));
#endif
    qApp->quit();
	} else {
		pg->setFormat("Simulation Completed");
	}
	stopMode();
};

void PDIF::startModel()
{

	QString outputDir = QCoreApplication::applicationDirPath() + "/../output/";
	QDir::setCurrent(outputDir);

	QObject::connect(qProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(binaryFinish(int, QProcess::ExitStatus)));
	QObject::connect(qProc, SIGNAL(readyRead()), this, SLOT(modelWrote()));
#ifdef Q_OS_WIN32
        QString path = QCoreApplication::applicationDirPath();
        QStringList env = QProcess::systemEnvironment();
        env << "PATH=" + path + "/gcc/bin" ;
        qProc->setEnvironment(env);

	qProc->start("./model.exe", QStringList() << "-i");
#else
#ifndef RTAIOS
	qProc->start("./model", QStringList() << "-i");
#endif
#endif
#ifndef RTAIOS
	if (!qProc->waitForStarted()) {
		msg("Could not execute the model binary!");
		qApp->quit();
	}
#endif
}

void PDIF::modelWrote()
{
	if (!procStarted) {
		QByteArray line = qProc->readLine();
		while (!line.isEmpty()) {
			if (QString(line).trimmed() == "(pdevs)") {
				procStarted = true;
				break;
			}
			line = qProc->readLine();
		}
	} else {		// Proc is runing
		QByteArray line = qProc->readLine();
		bool ok;
		while (!line.isEmpty()) {
			//qDebug(line.constData());
			while (line.startsWith("(pdevs)"))
				line = line.mid(7);	// Remove pdevs
			if (QString(line).trimmed() != "(pdevs)") {
				if (line.contains("ended") || line.contains("not running")) {	// Simulation endend found
          QRegExp r("ended (.+)");
					qProc->readAll();
					state = STOPPED;
					percentage = 0;
					justFinished = true;
          qDebug() << line;
          r.indexIn(line);
          QStringList ls = r.capturedTexts();
          qDebug() << ls;
          if (ls.length()>1)
          {
            exitStatus = ls.at(1).toInt();
          }
					//qProc->write(q.toLatin1().constData());
					stopMode();
					return;
				} else if (line.contains("Illegitimate") || line.contains("Abort")) {
					qProc->readAll();
					state = STOPPED;
					percentage = 0;
					//stopSimulation();
					justFinished = true;
					stopMode();
					msg("Simulation aborted. The model is illegitimate");
					return;
				} else if (line.contains("Init")) {
		      pg->setFormat(QString("Initializing..."));
				} else if (line.contains("Running")) {
	        pg->setFormat("%p%");
        }
				QString l = QString(line).trimmed();
				int perc = l.toInt(&ok);
				//qDebug((QString(" >> '") + l+ QString("'")).toLatin1().constData() );
				if (state == RUNNING && ok) {
					QString q = QString("s %1\np\n").arg(STEPTIMES);
					qProc->write(q.toLatin1().constData());
					pg->setValue(perc);
				} else if (state == STEPPING && ok) {
					pg->setValue(perc);
				} else {
					//qDebug(">>"); qDebug(l.toLatin1().constData());
				}
			}
			line = qProc->readLine();
		}
	}
}

void PDIF::stepSimulation()
{
	bool ok;
	QString qs = nmbsteps->displayText();
	int s = qs.toInt(&ok);
	if (!ok) {
		msg("The number of steps is incorrect.");
		return;
	}

	if (state != STEPPING) {
		state = STEPPING;
		times = runTimesEdit->displayText().toInt(&ok);
		if (!ok) {
			times = 1;
			QMessageBox msgBox;
			msgBox.
			    setText
			    ("The number of simulations to run is incorrect. Using 1");
			msgBox.setWindowTitle("PowerDEVS");
			msgBox.exec();
			runTimesEdit->setText(QString("1"));
		}
		if (!procStarted) {
			msg("Error. The model binary is not started.");
			runMode();
			state = STOPPED;
		} else {
			QString q =
			    QString("x %1\nt %2\ns %3\np\n").arg(times).
			    arg(getFinalTime()).arg(s);
			qProc->write(q.toLatin1().constData());
		}
		percentage = 0;
		stepMode();
		saveStmFile();
	} else {
		QString q = QString("s %1\np\n").arg(s);
		qProc->write(q.toLatin1().constData());
	}

}

void PDIF::runSimulation()
{
	state = RUNNING;
	percentage = 0;
	bool ok;
	runMode();
#ifndef RTAIOS
	times = runTimesEdit->displayText().toInt(&ok);
	if (!ok) {
		times = 1;
		QMessageBox msgBox;
		msgBox.
		    setText
		    ("The number of simulations to run is incorrect. Using 1");
		msgBox.setWindowTitle("PowerDEVS");
		msgBox.exec();
		runTimesEdit->setText(QString("1"));
	}
	if (!procStarted) {
		msg("Error. The model binary is not started.");
		runMode();
		state = STOPPED;
	} else {
		QString q = QString("x %1\nt %2\nb %4\ny 0\ns %3\np\n").arg(times).arg(getFinalTime()).arg(STEPTIMES).arg(getBreakSteps());
		////qDebug(q.toLatin1().constData());
		qProc->write(q.toLatin1().constData());
	}
#else
	// RTAI
	qDebug("Starting %g ..", getFinalTime());
	//qProc->start("./run", QStringList());
	qDebug() << QProcessEnvironment::systemEnvironment().value("DESKTOP_SESSION");
	QStringList args;
	args << "-k" << "-m" << "PowerDEVS requires root privileges to run the simulation under real time. Please enter your password" << QString("./model -tf %1").arg(getFinalTime());
	qDebug() << args;
	if (QProcessEnvironment::systemEnvironment().value("DESKTOP_SESSION")=="gnome")
	{
		qProc->start("/usr/bin/gksudo", args);
	} else {
		qProc->start("/usr/bin/kdsudo", args);
	}
	if (!qProc->waitForStarted()) {
		stopMode();
		msg("The process could not be started");
		return;
	}
#endif
  running.restart();
	saveStmFile();
}

int PDIF::getBreakSteps()
{
	bool ok;
	int i = ilegitimalCondition->displayText().toInt(&ok);
	if (ok)
		return i;
	return 10000;
}

double PDIF::getFinalTime()
{
	bool ok;
	double dTf;
	QString qs = tf->displayText();
	dTf = qs.toDouble(&ok);
	if (ok)
		return dTf;
	QMessageBox msgBox;
	msgBox.setText("The final time is incorrect. Using 10 sec.");
	msgBox.setWindowTitle("PowerDEVS");
	msgBox.exec();
	tf->setText("10");
	return 10.0;
}

void PDIF::stopSimulation()
{
#ifndef RTAIOS
	QString q("o\n");
	//qDebug(q.toLatin1().constData());
	qProc->write(q.toLatin1().constData());
#else

	if (qProc->state() == QProcess::Running) {
		qProc->kill();
	}
#endif
	stopMode();
}

void PDIF::runTimed()
{
	state = RUNNING;	// Run timed
	bool ok;
	runMode();
	times = runTimesEdit->displayText().toInt(&ok);
	if (!ok) {
		times = 1;
		QMessageBox msgBox;
		msgBox.
		    setText
		    ("The number of simulations to run is incorrect. Using 1");
		msgBox.setWindowTitle("PowerDEVS");
		msgBox.exec();
		runTimesEdit->setText(QString("1"));
	}
	if (!procStarted) {
		msg("Error. The model binary is not started.");
		runMode();
		state = STOPPED;
	} else {
		QString q =
		    QString("x %1\nt %2\ny 1\ns %3\np\n").arg(times).
		    arg(getFinalTime()).arg(STEPTIMES);
		//qDebug(q.toLatin1().constData());
		qProc->write(q.toLatin1().constData());
	}
	percentage = 0;
	saveStmFile();
  running.restart();
}

void PDIF::saveStmFile()
{
	bool ok;
	double dTf = tf->displayText().toDouble(&ok);
	if (QString(stmFile).trimmed().isEmpty())
		return;
	if (ok) {
		QFile *qf = new QFile(stmFile);
		if (qf->open(QIODevice::WriteOnly)) {
			QString st = QString("0\n%1").arg(dTf);
			qf->write(st.toLatin1());
			qf->close();
		}
	}
}

void PDIF::loadStmFile(char *f)
{
	bool ok;
	strcpy(stmFile, f);
	QFile *qf = new QFile(stmFile);
	tf->setText("10");
	if (qf->open(QIODevice::ReadOnly)) {
		QString qs = (QString) qf->readAll();
		qs.split('\n').at(1).toDouble(&ok);
		if (ok)
			tf->setText(qs.split('\n').at(1));
		qf->close();
	}
}

void PDIF::runMode()
{
	runButton->setDisabled(true);
	timedButton->setDisabled(true);
	exitButton->setDisabled(true);
	stopButton->setDisabled(false);
        viewLogButton->setDisabled(true);
	stepButton->setDisabled(true);
	runTimesEdit->setDisabled(true);
	tf->setDisabled(true);
	nmbsteps->setDisabled(true);
	ilegitimalCondition->setDisabled(true);
	pg->setFormat("%p%");
	justFinished = false;
	pg->setValue(0);
}

void PDIF::stepMode()
{
	runMode();
	nmbsteps->setDisabled(false);
	stepButton->setDisabled(false);
	runTimesEdit->setDisabled(false);
}

void PDIF::stopMode()
{
	state = STOPPED;
	runButton->setDisabled(false);
	timedButton->setDisabled(false);
	exitButton->setDisabled(false);
	stopButton->setDisabled(true);
	stepButton->setDisabled(false);
	viewLogButton->setDisabled(false);
	runTimesEdit->setDisabled(false);
	tf->setDisabled(false);
	ilegitimalCondition->setDisabled(false);
	nmbsteps->setDisabled(false);
  pg->setFormat("%p%");
	pg->setValue(0);
#ifdef RTAIOS
	timedButton->setDisabled(true);
	stepButton->setDisabled(true);
	runTimesEdit->setDisabled(true);
	ilegitimalCondition->setDisabled(true);
	nmbsteps->setDisabled(true);
#endif

	if (justFinished && exitStatus==0) {
		pg->setFormat(QString("Simulation Completed (%1 ms)").arg(running.elapsed()));
	}
	if (justFinished && exitStatus!=0) {
		pg->setFormat(QString("Simulation Completed (check log)"));
	}
}

void PDIF::exitModel()
{
	if (qProc->state() == QProcess::Running) {
		qProc->write("q\n");
	}
	if (!qProc->waitForFinished(1)) {
		qProc->kill();
	}
}
void PDIF::viewLog()
{
#ifndef Q_OS_WIN32
  QProcess::startDetached("/usr/bin/xdg-open", QStringList() << "../output/pdevs.log");
#else
    QProcess::startDetached("notepad", QStringList() << "../output/pdevs.log");
#endif

}
