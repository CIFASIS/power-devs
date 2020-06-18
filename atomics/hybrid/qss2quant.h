//CPP:hybrid/qss2quant.cpp
#if !defined qss2quant_h
#define qss2quant_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class qss2quant: public Simulator { 
// Declare here the state, output
// variables and parameters
double X;
double u;
double mu;
double sigma;
double y[2];
double inf;
double dq;



public:
	qss2quant(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
