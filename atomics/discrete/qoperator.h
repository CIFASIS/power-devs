//CPP:discrete/qoperator.cpp
#if !defined qoperator_h
#define qoperator_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

class qoperator: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma;
double state;
double next;
double ts;
double y[10];
public:
	qoperator(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
