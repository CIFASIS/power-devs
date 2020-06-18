#include "to_disk.h"
void to_disk::init(double t,...) {
char* FName;
va_list parameters;
va_start(parameters, t);
FName=va_arg(parameters, char*);
Sigma=10e10;
FOutput = PDFileOpen(FName, 'w');
}
double to_disk::ta(double t) {
return Sigma;
}
void to_disk::dint(double t) {
Sigma=10e10;
}
void to_disk::dext(Event x, double t) {
char buf[1024];
  double *Aux;
	if (x.port>=0) {
		Aux=(double*)(x.value);
		sprintf(buf,"%g, %g\n",t,*Aux);
		PDFileWrite(FOutput,buf,strlen(buf));
}
}
Event to_disk::lambda(double t) {
  return Event(0,0);
}
void to_disk::exit() {
PDFileClose(FOutput);
}
