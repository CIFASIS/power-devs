#include "pulse.h"
void pulse::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
vbase= va_arg(parameters,double);
ampl= va_arg(parameters,double);
itim= va_arg(parameters,double);
etim= va_arg(parameters,double);
k=0;
sigma=0;
v[0]=vbase;
v[1]=vbase+ampl;
v[2]=vbase;
tim[0]=0;
tim[1]=itim;
tim[2]=etim;
tim[3]=1e10;
for(int i=0;i<10;i++){y[i]=0;};
}
double pulse::ta(double t) {
//This function return a double.
return sigma;
}
void pulse::dint(double t) {
k=k+1;
sigma=tim[k]-tim[k-1];
}
void pulse::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
}
Event pulse::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=v[k];
return Event(&y,0);
}
void pulse::exit() {

}
