//CPP:qss/qsstools.cpp

//CPP:sin_function_step.cpp
#if !defined sin_function_step_h
#define sin_function_step_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "qss/qsstools.h"


class sin_function_step: public Simulator { 
//states 
double u[10];
double sigma;
int order;

//parameters
double tol;
double dQrel,dQmin;
double eps;
double y_n1,y_n2;
bool purely_static;
//output
double y[10];
unsigned int max_iter;
#define INF 1e20




































public:
	sin_function_step(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
