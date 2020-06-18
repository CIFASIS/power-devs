//CPP:qss/qsstools.cpp

//CPP:vector/matrix_gain.cpp
#if !defined matrix_gain_h
#define matrix_gain_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "qss/qsstools.h"
#include "vector.h"


class matrix_gain: public Simulator { 
// Declare the state, output
// variables and parameters

//states
double Sigma;
double **X;
int order;
int outs,nextout;
int *outinds;

//parameters
double **A;
int n,rows,cols;
int *nnrows;
int *nncols;
int **Arows;
int **Acols;

//outputs
double y[10];

vector yvec;
#define INF 1e20












public:
	matrix_gain(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
