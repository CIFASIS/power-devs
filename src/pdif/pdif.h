/***************************************************************************
**
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

#ifndef PDIF_H
#define PDIF_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QTimer>
#include <QMessageBox>
#include <QFile>
#include <QByteArray>
#include <QList>
#include <QProcess>
#include <QTime>

enum { STOPPED, RUNNING, RUNNING_TIMED, STEPPING };

class PDIF:public QDialog {
 Q_OBJECT public:
	PDIF(QWidget * parent = 0);

	void loadStmFile(char *);

	public slots:
  void runSimulation();
  void viewLog();
	void stopSimulation();
	void stepSimulation();
	void runTimed();
	void modelWrote();
	void exitModel();
	void binaryFinish(int, QProcess::ExitStatus);

 private:
  int exitStatus;
	int state;
	int times;
	bool readtimes;
	bool procStarted;
	bool justFinished;
	float percentage;
	void runMode();
	void stepMode();
	void stopMode();
	void saveStmFile();
	void startModel();
	double getFinalTime();
	int getBreakSteps();
	char stmFile[512];
	QProcess *qProc;
	QVBoxLayout *layout;
	QHBoxLayout *hLayout;
	QHBoxLayout *hLayoutSteps;
	QHBoxLayout *hLayoutTimes;
	QHBoxLayout *hLayoutIlegit;
	QWidget *hLayoutWidget;
	QWidget *hLayoutWidgetSteps;
	QWidget *hLayoutWidgetTimes;
	QWidget *hLayoutWidgetIleg;
	QPushButton *runButton;
	QPushButton *timedButton;
	QPushButton *exitButton;
	QPushButton *stopButton;
	QPushButton *viewLogButton;
	QPushButton *stepButton;
	QPushButton *closePlots;
	QLabel *label;
	QLabel *logo;
	QLabel *ilegitimal;
	QLabel *steps;
	QLabel *runTimes;
	QLineEdit *runTimesEdit;
	QLineEdit *nmbsteps;
	QLineEdit *tf;
	QLineEdit *ilegitimalCondition;
  QTime running;

	QProgressBar *pg;

};

#endif
