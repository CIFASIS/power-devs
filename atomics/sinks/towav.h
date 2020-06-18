//CPP:sink/towav.cpp
#if !defined towav_h
#define towav_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "fcntl.h"
#include "unistd.h"
#include "string.h"


class towav: public Simulator { 
// Declare the state, output
// variables and parameters
double y[10],sigma;
char *fname;
char *sample;
unsigned long count;

double ts;
short buf[1024],val16;
FILE *fd;
struct header {
char riff[4];
int filesize;
char wave[4];
char fmt[4];
int chunksize;
short comp;
short channels;
int samplerate;
int byterate;
short blockalign;
short bitdepth;
char data[4];
int datasize;

} h;




public:
	towav(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
