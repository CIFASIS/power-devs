#include "to_disk_var.h"
void to_disk_var::init(double t,...) {
char* FName;
va_list parameters;
va_start(parameters, t);
FName=va_arg(parameters, char*);
Sigma=4e10;
FOutput = fopen(FName, "w");
fclose(FOutput);
FOutput = fopen(FName, "a");
}
double to_disk_var::ta(double t) {
return Sigma;
}
void to_disk_var::dint(double t) {
Sigma=4e10;
}
void to_disk_var::dext(Event x, double t) {
double *Aux;
Aux=(double*)(x.value);
fprintf(FOutput, "%.12g, %.12g, %.12g, %.12g, %g, %g, %g, %g, %g, %g, %g\n",t,Aux[0],Aux[1],Aux[2],Aux[3],Aux[4],Aux[5],Aux[6],Aux[7],Aux[8],Aux[9] );
}
Event to_disk_var::lambda(double t) {

}
void to_disk_var::exit() {

}
