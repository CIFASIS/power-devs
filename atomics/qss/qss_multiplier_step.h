//CPP:qss/qsstools.cpp

//CPP:qss/qss_multiplier_step.cpp
#if !defined qss_multiplier_step_h
#define qss_multiplier_step_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"


class qss_multiplier_step: public Simulator { 
//states
double sigma,u[2][10];
int order;

//output
double y[10];
double dQrel,dQmin,tol;
bool purely_static;
double y_n1,y_n2;
#define INF 1e20









public:
	qss_multiplier_step(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
