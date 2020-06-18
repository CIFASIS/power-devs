//CPP:continuous/xpow_n.cpp
#if !defined xpow_n_h
#define xpow_n_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class xpow_n: public Simulator { 
double S[3];
double Sigma;
double y[10];
double n;
int nm;


public:
	xpow_n(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
