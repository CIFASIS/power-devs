//CPP:sink/scilabcommands.cpp
#if !defined scilabcommands_h
#define scilabcommands_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "string.h"


class scilabcommands: public Simulator { 
// Declare the state,
// output variables
// and parameters
double sigma;
char *atInit;
char *atDExt;
char *atExit;













public:
	scilabcommands(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
