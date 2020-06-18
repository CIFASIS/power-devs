#include "square.h"
void square::init(double t,...) {
va_list parameters;
char* InitVal;
va_start(parameters,t);
Vu=va_arg(parameters, double);
Vl=va_arg(parameters, double);
Th=fabs(va_arg(parameters, double));
Tl=fabs(va_arg(parameters, double));
InitVal=va_arg(parameters, char*);
if (strcmp(InitVal,"Upper")== 0) {
  Dir=true;  //Alto
  Xs=Vu;
  Sigma=Th;
} else {
  Dir=false;  //Bajo
  Xs=Vl;
  Sigma=Tl;
};
}
double square::ta(double t) {
return Sigma;
}
void square::dint(double t) {
if (Dir==true) { //Alto
  Xs=Vl;
  Sigma=Tl;
  Dir=false;
} else { //Bajo
  Xs=Vu;
  Sigma=Th;
  Dir=true;
};
}
void square::dext(Event x, double t) {

}
Event square::lambda(double t) {
return Event(&Xs,0);
}
void square::exit() {

}
