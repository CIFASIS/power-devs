#include "pulse_sci.h"
void pulse_sci::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
char *fvar=va_arg( parameters, char*);
vbase=getScilabVar(fvar );
fvar=va_arg( parameters, char*);
ampl=getScilabVar(fvar );
fvar=va_arg( parameters, char*);
itim=getScilabVar(fvar );
fvar=va_arg( parameters, char*);
etim=getScilabVar(fvar );


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
double pulse_sci::ta(double t) {
//This function return a double.
return sigma;
}
void pulse_sci::dint(double t) {
k=k+1;
sigma=tim[k]-tim[k-1];
}
void pulse_sci::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
}
Event pulse_sci::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=v[k];
return Event(&y,0);
}
void pulse_sci::exit() {

}
