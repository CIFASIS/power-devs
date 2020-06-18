//CPP:continuous/nhintegrator.cpp
#if !defined nhintegrator_h
#define nhintegrator_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class nhintegrator: public Simulator { 
// Declare the state, output
// variables and parameters
double X,dx,q,sigma;
double y[10];
double inf;
double dq;




public:
	nhintegrator(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
