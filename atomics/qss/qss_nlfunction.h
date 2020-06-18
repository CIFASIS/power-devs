//CPP:qss/mathexpr.cpp

//CPP:qss/qsstools.cpp

//CPP:qss/qss_nlfunction.cpp
#if !defined qss_nlfunction_h
#define qss_nlfunction_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "mathexpr.h"
#include "qss/qsstools.h"


class qss_nlfunction: public Simulator { 
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
PROperation pop;

//output
double y[10];

#define INF 1e20












public:
	qss_nlfunction(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
