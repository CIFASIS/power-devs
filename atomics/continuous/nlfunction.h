//CPP:continuous/mathexpr.cpp

//CPP:continuous/nlfunction.cpp
#if !defined nlfunction_h
#define nlfunction_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "mathexpr.h"


class nlfunction: public Simulator { 
// Declare here the state, output
// variables and parameters
double sigma;
double u[10];
double mu[10];
double pu[10];
double uaux[10];
double y[10];
double f,mf,pf;
char* expre;
int n,maxord;
double inf,dt;
PROperation pop;








public:
	nlfunction(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
