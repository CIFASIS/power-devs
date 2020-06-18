//CPP:qss/qsstools.cpp

//CPP:Qss/qss_wsum.cpp
#if !defined qss_wsum_h
#define qss_wsum_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class qss_wsum: public Simulator { 
// Declare the state, output
// variables and parameters

//states
double Sigma;
double X[8][10];
int order;

//parameters
double K[8];
int n;

//outputs
double y[10];


#define INF 1e20




public:
	qss_wsum(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
