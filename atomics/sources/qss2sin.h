//CPP:source/qss2sin.cpp
#if !defined qss2sin_h
#define qss2sin_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class qss2sin: public Simulator { 
// Declare here the state,
// variables and parameters
double sigma;
double y[2];
double a,f,phi,k,dt,w;




public:
	qss2sin(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
