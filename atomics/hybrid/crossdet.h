//CPP:hybrid/crossdet.cpp
#if !defined crossdet_h
#define crossdet_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class crossdet: public Simulator { 
// Declare here the state, output
// variables and parameters
double X;
double u;
double mu;
double sigma;
double level;
double y[2];
double inf;


public:
	crossdet(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
