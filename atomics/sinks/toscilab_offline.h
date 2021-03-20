//CPP:sinks/toscilab_offline.cpp
#if !defined toscilab_offline_h
#define toscilab_offline_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "string.h"
#include "unistd.h"
#include "fcntl.h"
#include "stdlib.h"
#include "stdio.h"


class toscilab_offline: public Simulator { 
double Sigma;
long int FOutput;
char *time_var;
char *signal_var;
unsigned count;









public:
	toscilab_offline(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
