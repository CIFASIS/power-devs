//CPP:continuous/xsquare.cpp
#if !defined xsquare_h
#define xsquare_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class xsquare: public Simulator { 
double S[3];
double Sigma;
double y[10];
int nm;



public:
	xsquare(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
