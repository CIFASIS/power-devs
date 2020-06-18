#include "to_disk3.h"
void to_disk3::init(double t,...) {
char* FName;
va_list parameters;
va_start(parameters, t);
FName=va_arg(parameters, char*);
Sigma=4e10;
FOutput = fopen(FName, "w");
fclose(FOutput);
FOutput = fopen(FName, "a");
}
double to_disk3::ta(double t) {
return Sigma;
}
void to_disk3::dint(double t) {
Sigma=4e10;
}
void to_disk3::dext(Event x, double t) {
double *Aux;
Aux=(double*)(x.value);
fprintf(FOutput, "%g , %g, %g, %g\n",t,Aux[0],Aux[1],Aux[2] );
}
Event to_disk3::lambda(double t) {

}
void to_disk3::exit() {

}
