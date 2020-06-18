//CPP:adquisicion/adquiutil.cpp
//CPP:adquisicion/adquider.cpp
#if !defined adquider_h
#define adquider_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "sys/io.h"
#include "comandos.h"
#include "adquiutil.h"
#include "math.h"

class adquider: public Simulator { 
// Declare the state,
// output variables
// and parameters
	int inti;

	int aux;
	 
	ushort Cuanto1;
	ushort Cuanto2;
	ushort Cuanto3;
	uchar  tipomaq;
	uchar ModoADC;
	uchar Interrupts;
	uchar desinc;
	uchar ERROR;

	double	sigma;
	 
	uchar cambio1;
	uchar cambio2;
	uchar cambio3;
	uchar cambio2d;
	uchar cambio3d;
	
	ushort ADC1; 
	short ADC2;
	short ADC3;
	short refpos;


	double dADC1[10];
	double dADC2[10];
	double dADC3[10];
	double dDADC2[10];
	double dDADC3[10];

	double d2sup;
	double d2inf;
	double d3sup;
	double d3inf;
	double TMAX;
	
	double *evT2;
	double *evA2;
	int evI2ant;
	int evI2;
	int evN2;
	
	double *evT3;
	double *evA3;
	int evI3;
	int evI3ant;
	int evN3;
	
	double gainPWM;
	double gainADC1;
	double gainADC2;
	double gainADC3;

public:
	adquider(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
