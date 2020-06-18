#include "constant.h"
void constant::init(double t,...) {
va_list parameters;
va_start(parameters, t);
S[0]=va_arg( parameters, double);
Sigma=0;
for(int i=1;i<10;i++){S[i]=0;};
}
double constant::ta(double t) {
return Sigma;
}
void constant::dint(double t) {
Sigma=4e10;
}
void constant::dext(Event x, double t) {

}
Event constant::lambda(double t) {
return Event(&S[0],0);
}
void constant::exit() {

}
