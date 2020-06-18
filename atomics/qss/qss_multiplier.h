//CPP:qss/qsstools.cpp

//CPP:qss/qss_multiplier.cpp
#if !defined qss_multiplier_h
#define qss_multiplier_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class qss_multiplier: public Simulator { 
//states
double sigma,u[2][10];
int order;

//output
double y[10];

#define INF 1e20




public:
	qss_multiplier(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
