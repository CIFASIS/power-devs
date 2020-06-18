//CPP:hybrid/cross3switch.cpp
#if !defined cross3switch_h
#define cross3switch_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class cross3switch: public Simulator { 
// Declare here the state, output
// variables and parameters
double X;
double u;
double u2;
double mu;
double v[2];
double mv[2];
double sigma;
double sigma2;
int flag;
double y[2];
double inf;



public:
	cross3switch(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
