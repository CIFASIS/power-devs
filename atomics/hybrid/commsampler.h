//CPP:hybrid/commsampler.cpp
#if !defined commsampler_h
#define commsampler_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class commsampler: public Simulator { 
double sigma;
double u,mu,pu,y[10];
double inf;


public:
	commsampler(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
