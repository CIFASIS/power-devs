#include "realtimeclock.h"
void realtimeclock::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
sigma = 10e10;
for (int i=0;i<10;i++) {
  y[i]=0;
}
}
double realtimeclock::ta(double t) {
//This function returns a double.
return sigma;
}
void realtimeclock::dint(double t) {
sigma=10e10;
}
void realtimeclock::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
dblRealTime = getRealSimulationTime();
sigma=0;
}
Event realtimeclock::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0] = dblRealTime;
return Event(& y,0);
}
void realtimeclock::exit() {

}
