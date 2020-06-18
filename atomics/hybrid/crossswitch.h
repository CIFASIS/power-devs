//CPP:hybrid/crossswitch.cpp
#if !defined crossswitch_h
#define crossswitch_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class crossswitch: public Simulator { 
// Declare here the state, output
// variables and parameters
double X;
double u;
double mu;
double v[2];
double mv[2];
double sigma;
double sigma2;
int flag;
double level;
double y[2];
double inf;



public:
	crossswitch(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
