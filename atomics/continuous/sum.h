//CPP:continuous/sum.cpp
#if !defined sum_h
#define sum_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class sum: public Simulator { 
double S[20];
double Ms[20];
double Sigma;
double Output[2];
int n;


public:
	sum(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
