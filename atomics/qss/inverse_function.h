//CPP:qss/inverse_function.cpp
#if !defined inverse_function_h
#define inverse_function_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class inverse_function: public Simulator { 
//states 
double u[10];
double sigma;
int order;

//parameters


//output
double y[10];

#define INF 1e20






public:
	inverse_function(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
