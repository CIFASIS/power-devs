#include "qss2sin.h"
void qss2sin::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
a= va_arg(parameters,double);
f= va_arg(parameters,double);
phi= va_arg(parameters,double);
k= va_arg(parameters,double);
dt=1/f/k;
w=3.14159*2*f;
sigma=0;
}
double qss2sin::ta(double t) {
//This function return a double.
return sigma;
}
void qss2sin::dint(double t) {
sigma=dt;
}
void qss2sin::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
}
Event qss2sin::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=a*sin(w*t+phi);
y[1]=a*w*cos(w*t+phi);
return Event(&y[0],0);
}
void qss2sin::exit() {

}
