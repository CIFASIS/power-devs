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

#include "model.h"
#include "pdevslib.h"
#include <string.h>
#include <stdio.h>
#include <signal.h>

void sig_hdlr(int sig)
{
  exit(sig);
}

void printf_flush(const char *fmt,...) 
{

	va_list ap;
	va_start(ap, fmt); 
	vprintf(fmt, ap); 
	va_end(ap); 
	fflush(stdout);
}
const char *msg = 

"PowerDEVS 2.0\n\
Copyright (C) 2009 Facultad de Ciencias Exactas Agrimensura e Ingeniería\n\
                   Universidad Nacional de Rosario - Argentina\n\
Contact: PowerDEVS Information (kofman@fceia.unr.edu.ar, fbergero@fceia.unr.edu.ar)\n\
License GPL: GNU GPL <http://gnu.org/licenses/gpl.html>\n\
This is free software: you are free to change and redistribute it.\n";

const char *msgHelp = 
"PowerDEVS model\n\n\
  -i     Run intearctive shell\n\
  -tf t  Set final time to t (double format)\n\
  -ti t  Set initial time to t (double format)\n\
  -rt    Synchronize events with real time\n\
  -d     Debug simulation\n\
  -b n   Stop simulation after n steps in the same time (to detecet illegitimal models)\n";

double tf;

int main(int argc,char **argv)
{
	// Parse args
	Model *mod=NULL;

	debug=false;	
	tf=30;
	double ti=0;
	bool interactive=false,timed=false;
	int breakCount = 1000000;
	int i=1;

#ifdef RTAIOS
	preinitLib();
#endif 

  signal(SIGFPE,sig_hdlr);
  signal(SIGSEGV,sig_hdlr);
	while (i<argc) {
		if (strcmp("-tf",argv[i])==0) {
			if (i > argc-2) {	
				strcpy(argv[i],"-h");
			} else {
				tf=atof(argv[++i]);
			}
		}
		if (strcmp("-ti",argv[i])==0) {
			if (i > argc-2) {	
				strcpy(argv[i],"-h");
			} else {
				ti=atof(argv[++i]);
			}
		}
		if (strcmp("-rt",argv[i])==0) {
      timed=true;
    }
		if (strcmp(argv[i],"-d")==0) {
			debug=true;
		}
		if (strcmp(argv[i],"-i")==0) {
			interactive=true;
		}
		if (strcmp(argv[i],"-b")==0) {
			if (i+1 > argc) {	
				strcpy(argv[i],"-h");
			} else {
				breakCount=atoi(argv[++i]);
			}
		}
		if (strcmp(argv[i],"-h")==0) {
			printf_flush("%s",msgHelp);
			return 0;
		}
		i++;

	}
	char buff[1024];
	bool running=false;
	int runs=1;	
	int runCount = 0;

	if (interactive) {
		printf_flush("%s",msg);
		while (buff[0]!='q') {
			printf_flush("(pdevs)");
			char *res=fgets(buff,1024,stdin);
      if (res==NULL)
        break;
			if (buff[0]=='x') {
				if (!running) {
					runs=atoi(buff+1);
				}
			}
			if (buff[0]=='t') {
				if (!running) {
					tf=atof(buff+1);
				}
			}
			if (buff[0]=='y') {
				if (!running) {
					timed=atoi(buff+1);
				}
			}
			if (buff[0]=='r') {
				if (running) {
					mod->exit();
					delete mod;
				}
				mod=new Model(ti);
        mod->setBreakCount(breakCount);
				mod->setInitialTime(ti);
				mod->init();
				mod->setFinalTime(tf);
        if (timed) 
          mod->setRealTime();

				mod->run();
			  printf_flush("Simulation ended %d\n",mod->getExitStatus());
				delete mod;
				mod=NULL;
				running=false;
			}
			if (buff[0]=='p') {
				if (running)
					printf_flush("%d\n",(runCount*(100/runs))+mod->percentageDone()/runs);
				else
					printf_flush("Model not running\n");
			}
			if (buff[0]=='b') {
				breakCount=atoi(buff+2);
			}
			if (buff[0]=='o') {
						mod->exit();
						printf_flush("Simulation ended %d\n",mod->getExitStatus());
						delete mod;
						mod=NULL;
						running=false;
						runCount = 0;
			}
			if (buff[0]=='s') {
				int steps=atoi(buff+1);
				if (steps==0) steps=1;
				if (!running) {
					running=true;
				  printf_flush("Init\n");
					mod=new Model(ti);
					mod->setInitialTime(ti);
          mod->setBreakCount(breakCount);
					mod->init();
					if (timed) mod->setRealTime();
					mod->setFinalTime(tf);
				  printf_flush("Running\n");
				} 
				if (mod->step(steps)) { // A Simulation has ended
					runCount ++; 
					if (runCount<runs) {
						delete mod;
						mod=NULL;
						mod=new Model(ti);
            mod->setBreakCount(breakCount);
						mod->setInitialTime(ti);
						mod->init();
						if (timed) 
              mod->setRealTime();
						mod->setFinalTime(tf);
					} else {
						printf_flush("Simulation ended %d\n",mod->getExitStatus());
						delete mod;
						mod=NULL;
						running=false;
						runCount = 0;
					}
				}
			}
			if (buff[0]=='h') {
					printf_flush("\
  r: Run model\n\
  p: Show percentage done\n\
  y [0|1] sYnch events with real time\n\
  x n: Run n simulations\n\
  s [n]: Make n steps\n\
  o Stop simulation\n\
  t time: Set final Time\n\
  b n: Stop simulating after n steps in the same time (illegitimal)\n\
  q: quit\n");
			}

		}
	} else {
				mod=new Model(ti);
				if (timed) 
          mod->setRealTime();
				mod->setInitialTime(ti);
        mod->setBreakCount(breakCount);
				mod->init();
				mod->setFinalTime(tf);
				mod->run();
				int ret=mod->getExitStatus();
        delete mod;
        return ret;
	}
	return 0;
}

