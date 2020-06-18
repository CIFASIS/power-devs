#include "ev_count2.h"
void ev_count2::init(double t,...) {
va_list parameters;
va_start(parameters, t);
FName=va_arg(parameters, char*);
n=(int)va_arg(parameters, double);
for (int i=0;i<n;i++){
  k[i]=0;
};
Sigma=1e20;
}
double ev_count2::ta(double t) {
return Sigma;
}
void ev_count2::dint(double t) {

}
void ev_count2::dext(Event x, double t) {
k[x.port]=k[x.port]+1;
}
Event ev_count2::lambda(double t) {

}
void ev_count2::exit() {
FILE* FOutput = fopen(FName, "w");
for (int i=0;i<n;i++){
  fprintf(FOutput, "%g\n",k[i]);
  };
fclose(FOutput);

//putScilabVar("ev(n)",k[0]);
}
