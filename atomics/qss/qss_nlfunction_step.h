//CPP:qss/mathexpr.cpp

//CPP:qss/qsstools.cpp

//CPP:qss/qss_nlfunction_step.cpp
#if !defined qss_nlfunction_step_h
#define qss_nlfunction_step_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "mathexpr.h"
#include "qss/qsstools.h"


class qss_nlfunction_step: public Simulator { 
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
double tol,dQmin,dQrel,y_n1,y_n2;
bool purely_static;
//output
double y[10];
double f3dt,f2dt,fdt,f0,f_dt,f_2dt,f_3dt;
#define INF 1e20


























public:
	qss_nlfunction_step(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
