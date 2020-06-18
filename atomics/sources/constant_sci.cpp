#include "constant_sci.h"
void constant_sci::init(double t,...) {
va_list parameters;
va_start(parameters, t);
char *fvar= va_arg(parameters,char*);
S[0]= getScilabVar(fvar );
Sigma=0;
for(int i=1;i<10;i++){S[i]=0;};
}
double constant_sci::ta(double t) {
return Sigma;
}
void constant_sci::dint(double t) {
Sigma=4e20;
}
void constant_sci::dext(Event x, double t) {

}
Event constant_sci::lambda(double t) {
return Event(&S[0],0);
}
void constant_sci::exit() {

}
