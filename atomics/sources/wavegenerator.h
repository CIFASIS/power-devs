//CPP:source/wavegenerator.cpp
#if !defined wavegenerator_h
#define wavegenerator_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"


class wavegenerator: public Simulator { 
double A; //Amplitude
double f; //Frecuency
double S[2]; //Output
double dQ; //delta Q
double Sigma;
bool Dir; 
char* Type;
char* Method;


public:
	wavegenerator(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
