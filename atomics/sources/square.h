//CPP:source/square.cpp
#if !defined square_h
#define square_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "math.h"
#include "string.h"


class square: public Simulator { 
double   Tl;
double   Th;
double   Vu;
double   Vl;
double   Xs;
double   Sigma;
bool  Dir;


public:
	square(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
