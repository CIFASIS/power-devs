//CPP:hybrid/rama_inv.cpp
#if !defined rama_inv_h
#define rama_inv_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class rama_inv: public Simulator { 
// Declare the state, output
// variables and parameters

double u[3];
double sal;
double sigma;
double inf;
double Vdwn;
double Vup;
double y[10];


public:
	rama_inv(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
