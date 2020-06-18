//CPP:hybrid/switching2.cpp
#if !defined switching2_h
#define switching2_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class switching2: public Simulator { 
// Declare here the state, output
// variables and parameters
double sigma;
double u[2];
double mu[2];
int k;
double y[2];
double inf;


public:
	switching2(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
