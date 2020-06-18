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

#ifndef PDEVSLIB_H
#define PDEVSLIB_H
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


#ifdef RTAIOS
#include <rtai_sched.h>
#include <rtai_usi.h>
#include <rtai_lxrt.h>
#include <rtai_mbx.h>
#include <rtai_msg.h>
#include <rtai_fifos.h>
#endif

#ifdef __WINDOWS__
#include <process.h>
#include <conio.h>
#include <windows.h>
#include "winsock2.h"
#endif

#include "event.h"

/*! Prints a message to the log file output/pdevs.log. This acts as printf. */
void printLog(const char *fmt,...);
/*! Creates a new process, and executes 'path' with 'arg' as arguments */ 
void spawnProcess(const char *path,char *arg);
/*! Writes the 'value' to a hardware port */
void writeToPort(short value,int port);
/*! Reads from a hardware port */
short readFromPort(int port);

/*! Used internally by the simulation engine to synchronize events
*   Each distribution (Windows, Linux, RTAI-Linux) implements its own version */
int waitFor(Time t, RealTimeMode m) ;

/*! Opens a file (works in realtime) */
long int PDFileOpen(char* name,char mode);
/*! Writes into a file (works in realtime) */
long int PDFileWrite(long int file ,char* buf,int size);
/*! Reads from a file (not implemented in RTAI) */
long int PDFileRead(long int file ,char* buf ,int size);
/*! Closes a file */
void PDFileClose(long int file);

class Simulator;
extern bool debug;

enum DebugEvent { INIT, TA, DINT, DEXT, LAMBDA,PROP };
enum OS { LINUX, RTAI, WINDOWS};

OS getOs(); 

/*! Debugs an event into the lof file. Debug is activated by the -d option when calling model */
void DEBUG(DebugEvent, Simulator *, const char *,...);
void initLib();
void preinitLib();
void cleanLib();
void initScilab();
void cleanScilab();
/*! Evaluates the expression given by varname in Scilab's workspace and returns the results */
double getScilabVar(char *varname);
/*! Writes Scilab's variable given by varname with value 'v' */
void putScilabVar(char *varname,double v);
/*! Evaluates the expression given by varname in Scilab's workspace and returns the dimension and values of a matrix via file */
void getScilabMatrix(char *varname, int *rows, int *cols, double **data);
/*! Evaluates the expression given by varname in Scilab's workspace and returns the dimension and values of a vector via file */
void getScilabVector(char *varname, int *length, double *data);


/*! Returns the result of the last command evaluated in Scilab */
void getAns(double *ans, int rows, int cols);
/*! Executes command 'cmd' in Scilab's workspace. The 'blocking' flag tells if the function must wait for Scilabs response to return 
This function (unlike exectionVoidScilabJob) writes the result to the result variable
*/
double executeScilabJob(char *cmd,bool blocking);
/*! Executes command 'cmd' in Scilab's workspace. The 'blocking' flag tells if the function must wait for Scilabs response to return */
double executeVoidScilabJob(char *cmd,bool blocking);
/*! This function returns the actual processor time. Each distribution uses its own implementation */
double getTime();
double getRealSimulationTime();

/*! This function tells the simulation engine that the atomic 'a' needs to be notified when the hardware interrupt 'irq' ocurrs */
void RequestIRQ(unsigned irq, void  *a);



void enterRealTime ();
void leaveRealTime();
/*
double *getScilabMatrix(char *varname, int rows, int cols, double **m);
void   putScilabMatrix(char *varname, int rows, int cols, double m[128][128]);
*/
double getFinalTime();
#endif

