//CPP:hybrid/trajcross.cpp
#if !defined trajcross_h
#define trajcross_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class trajcross: public Simulator { 
// Declare here the state, output
// variables and parameters
double X;
double u;
double mu;
double v;
double mv;
double sigma;
double y[2];
double inf;


public:
	trajcross(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
