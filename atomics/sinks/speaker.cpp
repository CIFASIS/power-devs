#include "speaker.h"
void speaker::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
sigma = 1e20;
}
double speaker::ta(double t) {
//This function returns a double.
return sigma;
}
void speaker::dint(double t) {
sigma=1e20;
}
void speaker::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double *Aux  = (double*)x.value;
int value = Aux[0] > 0 ? 0xff : 0x00;
writeToPort(value , 0x61);
}
Event speaker::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
}
void speaker::exit() {
writeToPort(0 , 0x61);
}
