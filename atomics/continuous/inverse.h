//CPP:continuous/inverse.cpp
#if !defined inverse_h
#define inverse_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class inverse: public Simulator { 
double y[10];
double u,mu,pu;
double Sigma;
int nm;


public:
	inverse(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
