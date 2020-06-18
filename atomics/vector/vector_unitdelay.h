//CPP:discrete/qoperator_sci.cpp
//CPP:vector/vector_unitdelay.cpp
#if !defined vector_unitdelay_h
#define vector_unitdelay_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "engine.h"
#include "vector.h"
#include "discrete/qoperator_sci.h"


class vector_unitdelay: public Simulator { 
// Declare the state,
// output variables
// and parameters

Coupling *D0;
//Simulator **D1[10000];
//Connection **EIC1[1]; 
//Connection **EOC1[1];
//Connection **IC1[1];
vector vec;
Event y;
int N,Ni;














public:
	vector_unitdelay(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
