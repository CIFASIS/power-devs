//CPP:source/pwmgen.cpp
#if !defined pwmgen_h
#define pwmgen_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class pwmgen: public Simulator { 
// Declare here the state, output
// variables and parameters

double a;
double tm;
double dt[64];
double sigma;
int j;
double sig;
double y[10];


public:
	pwmgen(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
