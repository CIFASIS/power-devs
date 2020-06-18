//CPP:vector/index_mapper.cpp
#if !defined index_mapper_h
#define index_mapper_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "engine.h"
#include "qss/qsstools.h"
#include "vector.h"
#include "qss/qss_integrator.h"
#include "list"


class index_mapper: public Simulator { 
// Declare the state,
// output variables
// and parameters

double sigma;
int N;
vector vec;
Event y;
std::list<int> *map;
std::list<int>::iterator index;

int state,from;
#define INF 1e20



















public:
	index_mapper(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
