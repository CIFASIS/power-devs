//CPP:qss/qsstools.cpp
//CPP:Qss/liqss_integrator.cpp
#if !defined liqss_integrator_h
#define liqss_integrator_h

#include "simulator.h"
#include "Event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"
#include "qss/qsstools.h"


class liqss_integrator: public simulator { 
// Declare here the state, output
// variables and parameters
double X[10],q[10],dq,q_old,dq_old;
double a,u[10],sigma;

//parameters
double dQ;
char* Method;
double dQmin,dQrel;
int met;

double y[10]; //output
















public:
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
