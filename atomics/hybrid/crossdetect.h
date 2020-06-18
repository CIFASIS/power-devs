//CPP:hybrid/crossdetect.cpp
#if !defined crossdetect_h
#define crossdetect_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class crossdetect: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma,u,mu,pu;
double tcross;
int sw,ch;

double level,outval,inf;

double y[10];


public:
	crossdetect(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
