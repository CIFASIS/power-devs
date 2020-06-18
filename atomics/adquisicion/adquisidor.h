//CPP:adquisicion/adquiutil.cpp
//CPP:adquisicion/adquisidor.cpp
#if !defined adquisidor_h
#define adquisidor_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "sys/io.h"
#include "comandos.h"
#include "adquiutil.h"

#include <math.h>
#include <unistd.h>

class adquisidor: public Simulator { 
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
	
	ushort ADC1; 
	short ADC2;
	short ADC3; 
	short refpos;
	

	double dADC1[10];
	double dADC2[10];
	double dADC3[10];    

	double gainPWM;
	double gainADC1;
	double gainADC2;
	double gainADC3;

public:
	adquisidor(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
