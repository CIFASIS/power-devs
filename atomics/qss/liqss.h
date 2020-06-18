//CPP:qss/qsstools.cpp

//CPP:qss/liqss.cpp
#if !defined liqss_h
#define liqss_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"
#include "qss/qsstools.h"


class liqss: public Simulator { 
// Declare here the state, output
// variables and parameters
double X[10],q[10],dq,q_old,dq_old,dx_old;
double a,u[10],sigma;

//parameters
double dQ;
double dQmin,dQrel;
double y[10]; //output
bool band,band3;































































public:
	liqss(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
