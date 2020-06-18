//CPP:integrator.cpp
#if !defined INTEGRATOR_H
#define INTEGRATOR_H

#include "simulator.h"
#include "event.h"
#include "stdarg.h"
#include "math.h"

class integrator: public Simulator { 
// Declare here the state, output
// variables and parameters
double u,mu,pu,X,q,mq,pq;
double u_ant,mu_ant;
double qi,qs;
int band;

//parameters
double pidiv3;
double eps,ep;
double sigma;
double dQ;
char* Method;
double y[10]; //output

#define INF 1e20;

double gain,q_ant;
double count;
int met;




public:
	integrator(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
