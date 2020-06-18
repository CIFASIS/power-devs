//CPP:realtime/rtview_lcd.cpp
#if !defined rtview_lcd_h
#define rtview_lcd_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"



class rtview_lcd: public Simulator { 
// Declare the state,
// output variables
// and parameters

char filename[128];














public:
	rtview_lcd(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
