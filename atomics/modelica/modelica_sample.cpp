#include "modelica_sample.h"
void modelica_sample::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.

va_list parameters;
va_start(parameters,t);

//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type

Index = (int) va_arg(parameters,double);
ConditionIndex = (int) va_arg(parameters,double);
function_sample(Index, y);
itime=y[0];
period=y[1];
printLog("We set itime=%g and period=%g \n",itime,period);
for (int i=0;i<10;i++) {
  y[i]=0;
};

sigma=itime; //first sample
}
double modelica_sample::ta(double t) {
//This function returns a double.
return sigma;
}
void modelica_sample::dint(double t) {
sigma=period;
set_condition_to(ConditionIndex-1,false);
//printLog("[t=%g] Sample %d fired (crossing %d)\n",t,Index,ConditionIndex);
}
void modelica_sample::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
}
Event modelica_sample::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=1;
set_condition_to(ConditionIndex-1,true);
return  Event(&y[0], 0);
}
void modelica_sample::exit() {

}
