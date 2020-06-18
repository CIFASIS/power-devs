#include "step.h"
void step::init(double t,...) {
va_list parameters;
va_start(parameters, t);
S[0]=va_arg( parameters, double);
Delay=va_arg( parameters, double);
Vfin=va_arg( parameters, double);
S[1]=0;
Sigma=0;
Aux=false;
}
double step::ta(double t) {
return Sigma;
}
void step::dint(double t) {
if (Aux==false) {
  Sigma=Delay;	
  Aux=true;
} else {
  S[0]=Vfin;
  Sigma=4e10;
};
}
void step::dext(Event x, double t) {

}
Event step::lambda(double t) {
return Event(&S[0],0);
}
void step::exit() {

}
