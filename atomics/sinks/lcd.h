//CPP:sinks/lcd.cpp
#if !defined lcd_h
#define lcd_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class lcd: public Simulator { 
// Declare the state,
// output variables
// and parameters


char filename[128];	



public:
	lcd(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
