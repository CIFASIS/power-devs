//CPP:continuous/multiplier.cpp
#if !defined multiplier_h
#define multiplier_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class multiplier: public Simulator { 
double y[10];
double sigma;
double u[2],mu[2],pu[2];
double inf;
int nm;



public:
	multiplier(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
