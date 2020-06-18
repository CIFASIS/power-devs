//CPP:hybrid/levelcheck.cpp
#if !defined levelcheck_h
#define levelcheck_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class levelcheck: public Simulator { 
double sigma;
double u,mu,pu,y[10];
double level, outvalue,inf;


public:
	levelcheck(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
