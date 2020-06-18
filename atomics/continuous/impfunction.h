//CPP:Continuous/mathexpr.cpp

//CPP:continuous/impfunction.cpp
#if !defined impfunction_h
#define impfunction_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "mathexpr.h"


class impfunction: public Simulator { 
// Declare here the state, output
// variables and parameters
double sigma;
double u[11];
double mu[11];
double pu[11];
double c[11];
double z;
double mz;
double cz;
double h[11];
double g[11];
double f[4];
double y[10];
char* expre;
int n,ini,nm;
double tol;
PROperation pop;

#define INF 1e20






public:
	impfunction(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
