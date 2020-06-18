//CPP:source/singen.cpp
#if !defined singen_h
#define singen_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"


class singen: public Simulator { 
// Declare here the state,
// variables and parameters
double sigma;
double y[10];
double a,f,phi,k,dt,w;



public:
	singen(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
