//CPP:continuous/sinus.cpp
#if !defined sinus_h
#define sinus_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class sinus: public Simulator { 
double Y[10];
double Sigma;
double S, Ms, Ps;
double c;
int nm;


public:
	sinus(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
