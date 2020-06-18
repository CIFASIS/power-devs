//CPP:qss/qsstools.cpp

//CPP:vector/vector_sum.cpp
#if !defined vector_sum_h
#define vector_sum_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"
#include "vector.h"


class vector_sum: public Simulator { 
// Declare the state, output
// variables and parameters

//states
double Sigma;
double X[10000][10];
int order;

//parameters
double K;
int n;

//outputs
double y[10];


#define INF 1e20







public:
	vector_sum(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
