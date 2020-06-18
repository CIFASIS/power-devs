//CPP:qss/qsstools.cpp

//CPP:qss/qss3.cpp
#if !defined qss3_h
#define qss3_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"
#include "qss/qsstools.h"


class qss3: public Simulator { 
// Declare here the state, output
// variables and parameters
double X[10],q[10];
double sigma;

double qs,qi;

//parameters
double dQ;
double dQmin,dQrel;


double eps,ep;
int band;

double y[10]; //output




















public:
	qss3(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
