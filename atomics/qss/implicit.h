//CPP:qss/mathexpr.cpp

//CPP:qss/qsstools.cpp

//CPP:qss/secant_solve.cpp

//CPP:Qss/implicit.cpp
#if !defined implicit_h
#define implicit_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "mathexpr.h"
#include "qss/qsstools.h"
#include "secant_solve.h"


class implicit: public Simulator { 
// Declare here the state, output
// variables and parameters

//states
double sigma;
double u[10][10];
double uaux[10][10],f[10];
int order;
double dt;

//parameters
char* expre;
int n;
double iguess,tol;
PROperation pop;

//output
double y[10];

#define INF 1e20


















public:
	implicit(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
