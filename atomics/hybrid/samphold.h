//CPP:hybrid/samphold.cpp
#if !defined samphold_h
#define samphold_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class samphold: public Simulator { 
double T,phi;
double Sigma;
double S[10],y[10];


public:
	samphold(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
