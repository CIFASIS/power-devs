//CPP:control/conmlogica.cpp
#if !defined conmlogica_h
#define conmlogica_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"


class conmlogica: public Simulator { 
// Declare the state,
// output variables
// and parameters
double K[2];
double Sigma;

double S[10];
int n;
int order;
double inp[3];
bool band0;
bool band1;
double maxe;
double lastmax;
double outp;



public:
	conmlogica(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
