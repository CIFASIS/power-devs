//CPP:sink/wxscope.cpp
#if !defined wxscope_h
#define wxscope_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "wx/frame.h"
#include "wx/pen.h"
#include "wx/list.h"
#include "wx/dcclient.h"
#include "wx/dc.h"
#include "wx/dynarray.h"
#include "wx/dialog.h"
#include "wx/textctrl.h"
#include "wx/button.h"
#include "wx/listbox.h"
#include "wx/fontdlg.h"
#include "wx/radiobox.h"


class wxscope: public Simulator { 

public:
	wxscope(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
