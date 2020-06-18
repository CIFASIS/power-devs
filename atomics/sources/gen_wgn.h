//CPP:source/gen_wgn.cpp
#if !defined gen_wgn_h
#define gen_wgn_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "stdlib.h"
#include "time.h"


class gen_wgn: public Simulator { 
// Declare the state, output
// variables and parameters

double sigma; //estado
double u;
double y[10]; //salida

//parametros

double variance; //variance
double mean;
double sampleperiod; //period of the samples



public:
	gen_wgn(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
