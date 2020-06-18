//CPP:source/sinegen.cpp
#if !defined sinegen_h
#define sinegen_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class sinegen: public Simulator { 
// Declare here the state,
// variables and parameters
double sigma;
double y[10];
double a,f,phi,k,dt,w;
char* Method;




public:
	sinegen(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
