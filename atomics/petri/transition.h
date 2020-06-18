//CPP:petri/database.cpp

//CPP:petri/transition.cpp
#if !defined transition_h
#define transition_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "database.h"
#include "place.h"


class transition: public Simulator { 
// Declare the state, output
// variables and parameters
double Sigma;
double time, inf;
bool enabled, source, working, portsinit;
int nin;
DataBase *data;
Menssaje *menssaje;
place *predecessor;










































































































public:
	transition(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
