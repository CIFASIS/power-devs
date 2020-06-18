//CPP:Continuous\printegrator.cpp
#if !defined printegrator_h
#define printegrator_h

#include "math.h"
#include "string.h"
#include "simulator.h"
#include "Event.h"
#include "stdarg.h"

class printegrator:public simulator{
// Declare here the state, output
// variables and parameters
double u,mu,pu,X;
double inf;
double sigma;
double y[10]; //output



public:
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
};
#endif
