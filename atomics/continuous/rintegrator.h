//CPP:Continuous\rintegrator.cpp
#if !defined rintegrator_h
#define rintegrator_h

#include "math.h"
#include "string.h"
#include "simulator.h"
#include "Event.h"
#include "stdarg.h"

class rintegrator:public simulator{
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
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
};
#endif
