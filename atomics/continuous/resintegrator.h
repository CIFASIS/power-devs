//CPP:continuous/resintegrator.cpp
#if !defined resintegrator_h
#define resintegrator_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"


class resintegrator: public Simulator { 
// Declare here the state, output
// variables and parameters
double pidiv3;
double u,mu,pu,X,q,mq,pq;
double inf,dQ;
double sigma;
char* Method;
double y[10]; //output




public:
	resintegrator(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
