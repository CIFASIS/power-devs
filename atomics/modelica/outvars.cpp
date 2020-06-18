#include "outvars.h"
#include "simulation_result.h"
#include "simulation_runtime.h"
void outvars::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
sigma=getFinalTime()/2000;
}
double outvars::ta(double t) {
//This function returns a double.
return sigma;
}
void outvars::dint(double t) {
  globalData->timeValue=t;
  sim_result->emit();
}
void outvars::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

}
Event outvars::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
return Event();
}
void outvars::exit() {
//Code executed at the end of the simulation.
delete sim_result;
}
