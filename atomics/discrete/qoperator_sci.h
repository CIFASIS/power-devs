//CPP:discrete/qoperator_sci.cpp
#if !defined qoperator_sci_h
#define qoperator_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class qoperator_sci: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma;
double state;
double next;
double ts;
double y[10];













public:
	qoperator_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
