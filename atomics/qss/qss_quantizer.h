//CPP:qss/qsstools.cpp

//CPP:qss/qss_quantizer.cpp
#if !defined qss_quantizer_h
#define qss_quantizer_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class qss_quantizer: public Simulator { 
//states
double sigma,q;
double u[10];


//output
double y[10];

//parameters
double dQ,eps;

#define INF 1e20














public:
	qss_quantizer(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
