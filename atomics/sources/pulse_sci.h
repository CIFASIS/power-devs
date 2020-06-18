//CPP:source/pulse_sci.cpp
#if !defined pulse_sci_h
#define pulse_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class pulse_sci: public Simulator { 
// Declare here the state, output
// variables and parameters
double sigma;
int k;
double v[3];
double y[10];
double tim[4];
double vbase;
double ampl;
double itim;
double etim;



public:
	pulse_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
