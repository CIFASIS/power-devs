//CPP:continuous/sinus2.cpp
#if !defined sinus2_h
#define sinus2_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class sinus2: public Simulator { 
double Y[10];
double Sigma;
double S, Ms, Ps;
double c;


public:
	sinus2(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
