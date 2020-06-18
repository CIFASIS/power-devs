//CPP:hybrid/hcomparator_sci.cpp
#if !defined hcomparator_sci_h
#define hcomparator_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class hcomparator_sci: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma,u[2],mu[2],pu[2];
double tcross;
int sw;
double Vu, Vl;
double inf;

double y[10];


public:
	hcomparator_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
