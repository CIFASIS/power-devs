//CPP:hybrid/switch3_sci.cpp
#if !defined switch3_sci_h
#define switch3_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class switch3_sci: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma,u[3],mu[3],pu[3];
double tcross;
int sw,ch;

double level,inf;

double y[10];


public:
	switch3_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
