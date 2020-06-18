//CPP:source/fromwav.cpp
#if !defined fromwav_h
#define fromwav_h

#include "simulator.h"
#include "event.h"
#include "stdarg.h"

#include "fcntl.h"
#include "unistd.h"
#include "malloc.h"


class fromwav: public Simulator { 
// Declare the state, output
// variables and parameters
double y[10],sigma;
char *fname;
char *buf;
char *val8; short *val16;
int fd;
long int count;
struct header {
char riff[4];
int filesize;
char wave[4];
char fmt[4];
int chunksize;
short comp;
short channels;
int samplerate;
int test;
char extra[2];
short bitdepth;
int extra2;
int datasize;

} h;




















public:
	fromwav(const char *n): Simulator(n) {};
	void init(double, ...);
	double ta(double t);
	void dint(double);
	void dext(Event , double );
	Event lambda(double);
	void exit();
};
#endif
