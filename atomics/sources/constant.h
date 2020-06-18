//CPP:source/constant.cpp
#if !defined constant_h
#define constant_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class constant: public Simulator { 
double S[10];
double Sigma;


public:
	constant(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
