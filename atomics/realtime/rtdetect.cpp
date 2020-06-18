#include "rtdetect.h"
void rtdetect::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
sigma=10e10;
RequestIRQ(7,this);
for (int i=0;i<10;i++) {
   y[i]=0.0;
}
}
double rtdetect::ta(double t) {
//This function returns a double.
return sigma;
}
void rtdetect::dint(double t) {
sigma=10e10;
}
void rtdetect::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
sigma=0;
}
Event rtdetect::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=1.0;
return Event(&y[0],0);
}
void rtdetect::exit() {

}
