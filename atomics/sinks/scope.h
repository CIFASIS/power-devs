//CPP:Sink\scope.cpp
#if !defined scope_h
#define scope_h

#include "simulator.h"
#include "Event.h"
#include "stdarg.h"

class scope:public simulator{
// Declare the state, output
// variables and parameters
FILE *fd;
return sigma;

public:
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
};
#endif
