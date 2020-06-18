//CPP:source/Trapezoidal.cpp
#if !defined Trapezoidal_h
#define Trapezoidal_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"


class Trapezoidal: public Simulator { 
double Tl;
double Tup;
double Th;
double Tlw;
double Vu;
double Vl;
double S[2];
bool Dir;
double Q;
double Sigma;
char *Method;


public:
	Trapezoidal(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
