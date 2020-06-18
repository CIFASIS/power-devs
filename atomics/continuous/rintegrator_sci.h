//CPP:continuous/rintegrator_sci.cpp
#if !defined rintegrator_sci_h
#define rintegrator_sci_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"


class rintegrator_sci: public Simulator { 
// Declare here the state, output
// variables and parameters
double pidiv3;
double u,mu,pu,X,q,mq,pq;
double qi,qs;
double eps,ep;
double inf,dQ;
double sigma;
char* Method;
int band;
double y[10]; //output
double dQmin;



public:
	rintegrator_sci(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
