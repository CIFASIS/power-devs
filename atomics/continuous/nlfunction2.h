//CPP:Continuous/mathexpr.cpp

//CPP:continuous/nlfunction2.cpp
#if !defined nlfunction2_h
#define nlfunction2_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "mathexpr.h"


class nlfunction2: public Simulator { 
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
	nlfunction2(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
