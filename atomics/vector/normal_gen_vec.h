//CPP:/random/normal_gen.cpp
//CPP:/random/lib/stocc.cpp
//CPP:/random/lib/randomc.cpp
//CPP:vector/normal_gen_vec.cpp
#if !defined normal_gen_vec_h
#define normal_gen_vec_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "engine.h"
#include "random/lib/randomc.h"
#include "random/lib/stocc.h"
#include "random/normal_gen.h"
#include "vector.h"


class normal_gen_vec: public Simulator { 
// Declare the state,
// output variables
// and parameters

Coupling *D0;
//Simulator **D1[1];
//Connection **EIC1[1]; 
//Connection **EOC1[1];
//Connection **IC1[1];
int N;
vector vec;
Event y;













public:
	normal_gen_vec(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
