#include "qss2ramp.h"
void qss2ramp::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
inf=1e10;
t0=va_arg(parameters,double);
tr=va_arg(parameters,double);
u=va_arg(parameters,double);
T[0]=0;
T[1]= t0;
T[2]=t0+tr;
T[3]=inf;
v[0]= 0;
v[1]=0;
v[2]=u;
mv[0]=0;
if (tr>0){
  mv[1]=u/tr;
}
mv[2]=0;
sigma=0;
j=0;
}
double qss2ramp::ta(double t) {
//This function return a double.
return sigma;
}
void qss2ramp::dint(double t) {
sigma=T[j+1]-T[j];
j=j+1;
}
void qss2ramp::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
}
Event qss2ramp::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=v[j];
y[1]=mv[j];
return Event(&y,0);
}
void qss2ramp::exit() {

}
