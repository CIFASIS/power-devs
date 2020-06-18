#include "to_diskrt.h"
void to_diskrt::init(double t,...) {
char* FName;
va_list parameters;
va_start(parameters, t);
FName=va_arg(parameters, char*);
Sigma=4e10;
FOutput = fopen(FName, "w");
fclose(FOutput);
FOutput = fopen(FName, "a");
itime=1.0*clock()/CLOCKS_PER_SEC;
}
double to_diskrt::ta(double t) {
return Sigma;
}
void to_diskrt::dint(double t) {
Sigma=4e10;
}
void to_diskrt::dext(Event x, double t) {
double *Aux;
double actime;
Aux=(double*)(x.value);
actime=1.0*clock()/CLOCKS_PER_SEC-itime;
fprintf(FOutput, "%g , %g, %g\n",t,actime, Aux[0] );
}
Event to_diskrt::lambda(double t) {

}
void to_diskrt::exit() {

}
