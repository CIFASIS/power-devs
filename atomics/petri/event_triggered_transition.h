//CPP:petri/event_triggered_transition.cpp
#if !defined event_triggered_transition_h
#define event_triggered_transition_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "structures.h"


class event_triggered_transition: public Simulator { 
// Declare the state, output
// variables and parameters
double Sigma;
double inf;
Menssaje *menssaje;
void* thing;



























public:
	event_triggered_transition(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
