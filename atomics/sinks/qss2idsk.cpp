#include "qss2idsk.h"
void qss2idsk::init(double t,...) {
char* FName;
va_list parameters;
va_start(parameters, t);
FName=va_arg(parameters, char*);
x0=va_arg(parameters, double);
ref=va_arg(parameters, double);
inf=4e10;
Sigma=inf;
FOutput = fopen(FName, "w");
fclose(FOutput);
FOutput = fopen(FName, "a");
u=0;
mu=0;
X=x0;
tlast=t;
}
double qss2idsk::ta(double t) {
return Sigma;
}
void qss2idsk::dint(double t) {
Sigma=inf;
}
void qss2idsk::dext(Event x, double t) {
double *xv;
double dt;
xv=(double*)(x.value);
if (e>0) {
  for (int i=1;i<ref;i++) {
     dt=i*e/ref;
     fprintf(FOutput, "%g , %g\n",tlast+dt,X+u*dt+mu*dt*dt/2);
  };
  tlast=t;
  X=X+u*e+mu*e*e/2;
  fprintf(FOutput, "%g , %g\n",tlast,X);
};
u=xv[0];
mu=xv[1];
}
Event qss2idsk::lambda(double t) {

}
void qss2idsk::exit() {

}
