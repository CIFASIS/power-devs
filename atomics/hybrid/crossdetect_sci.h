//CPP:hybrid/crossdetect_sci.cpp
#if !defined crossdetect_sci_h
#define crossdetect_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class crossdetect_sci: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma,u,mu,pu;
double tcross;
int sw,ch;

double level,outval,inf;

double y[10];


public:
	crossdetect_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
