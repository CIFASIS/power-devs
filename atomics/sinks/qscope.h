//HeadersDir:C:\PowerDEVS\GCC\INCLUDE

//CPP:sink/qscope.cpp
#if !defined qscope_h
#define qscope_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "stdio.h"
#include "windows.h"
#include "string.h"


class qscope: public Simulator { 
// Declare the state, output
// variables and parameters
double sigma;
HANDLE hChildStdinRd, hChildStdinWr;
SECURITY_ATTRIBUTES saAttr;
PROCESS_INFORMATION piProcInfo;
STARTUPINFO siStartInfo;
BOOL bFuncRetn ;
DWORD dwWritten;


public:
	qscope(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
