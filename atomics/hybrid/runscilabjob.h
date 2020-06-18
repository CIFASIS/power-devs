//CPP:hybrid/runscilabjob.cpp
#if !defined runscilabjob_h
#define runscilabjob_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "string.h"


class runscilabjob: public Simulator { 
// Declare the state,
// output variables
// and parameters
double sigma;
char *atInit;
char *atDExt;
char *atExit;












public:
	runscilabjob(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
