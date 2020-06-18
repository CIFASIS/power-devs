#include "step_sci.h"
void step_sci::init(double t,...) {
va_list parameters;
va_start(parameters, t);
char *fvar=va_arg( parameters, char*);

S[0]=getScilabVar(fvar );
fvar=va_arg( parameters, char*);
Delay=getScilabVar(fvar );
fvar=va_arg( parameters, char*);
Vfin=getScilabVar(fvar );

for(int i=1;i<10;i++){S[i]=0;};
Sigma=0;
Aux=false;
}
double step_sci::ta(double t) {
return Sigma;
}
void step_sci::dint(double t) {
if (Aux==false) {
  Sigma=Delay;	
  Aux=true;
  S[0]=Vfin;
} else {
  Sigma=4e10;
};
}
void step_sci::dext(Event x, double t) {

}
Event step_sci::lambda(double t) {
return Event(&S[0],0);
}
void step_sci::exit() {

}
