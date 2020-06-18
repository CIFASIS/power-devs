//CPP:petri/linkedlist.cpp

//CPP:petri/place.cpp
#if !defined place_h
#define place_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "linkedlist.h"


class place: public Simulator { 

// Declare the state, output
// variables and parameters
double Sigma;
int nin, m, tokens;
double time, inf;
Menssaje *menssaje;
LinkedList *list;
int TimeFlag;
Simulator *transition;


























public:
	place(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
	int GetTokens(){return tokens;};
	void SetTokens(int m) {tokens = m;};
};
#endif
