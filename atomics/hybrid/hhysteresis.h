//CPP:hybrid/hhysteresis.cpp
#if !defined hhysteresis_h
#define hhysteresis_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class hhysteresis: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma,u,mu,pu;
int level;

double y[3];

double xl,xu,yl,yu,inf;


public:
	hhysteresis(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
