#include "step13.h"
void step13::init(double t,...) {
va_list parameters;
va_start(parameters, t);
S[0]=va_arg( parameters, double);
Delay=va_arg( parameters, double);
Vfin=va_arg( parameters, double);
for(int i=1;i<10;i++){S[i]=0;};
Sigma=0;
Aux=false;
}
double step13::ta(double t) {
return Sigma;
}
void step13::dint(double t) {
if (Aux==false) {
  Sigma=Delay;	
  Aux=true;
  S[0]=Vfin;
} else {
  Sigma=4e10;
};
}
void step13::dext(Event x, double t) {

}
Event step13::lambda(double t) {
return Event(&S[0],0);
}
void step13::exit() {

}
