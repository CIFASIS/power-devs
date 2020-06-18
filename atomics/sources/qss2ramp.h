//CPP:source/qss2ramp.cpp
#if !defined qss2ramp_h
#define qss2ramp_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class qss2ramp: public Simulator { 
// Declare here the state 
// variables and parameters
double sigma;
int j; //states
double y[2]; //output
double t0,tr,u;
double T[4],v[4],mv[4],inf;
//parameters


public:
	qss2ramp(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
