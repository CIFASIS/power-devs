//CPP:hybrid/hsaturation.cpp
#if !defined hsaturation_h
#define hsaturation_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class hsaturation: public Simulator { 
// Declare the state, output
double y[10];
// variables and parameters
double xl, xu;
double inf;
double sigma;
double u,mu,pu;
double u1;
double level[2];



public:
	hsaturation(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
