//CPP:sources/fromworkspace.cpp
#if !defined fromworkspace_h
#define fromworkspace_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class fromworkspace: public Simulator { 
// Declare the state,
// output variables
// and parameters
double Sigma;
double nextT;
double actualT;
double nextV;
double actualV;
char *time_var;
char *signal_var;
int count;
int max;
double y[10];
char buf[1024];
int method;
double *u;
double *tu;





































public:
	fromworkspace(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
