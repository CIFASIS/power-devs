//CPP:source/pulse.cpp
#if !defined pulse_h
#define pulse_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class pulse: public Simulator { 
// Declare here the state, output
// variables and parameters
double sigma;
int k;
double v[3];
double y[10];
double tim[3];
double vbase;
double ampl;
double itim;
double etim;



public:
	pulse(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
