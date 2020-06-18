#include "sinegen_sci.h"
void sinegen_sci::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
char *fvar=va_arg( parameters, char*);
a=getScilabVar(fvar );
fvar=va_arg( parameters, char*);
f=getScilabVar(fvar );
fvar=va_arg( parameters, char*);
phi=getScilabVar(fvar );
fvar=va_arg( parameters, char*);
k=getScilabVar(fvar );

Method=va_arg(parameters,char*); 
dt=1/f/k;
w=3.14159*2*f;
sigma=0;
for(int i=0;i<10;i++){y[i]=0;};
}
double sinegen_sci::ta(double t) {
//This function return a double.
return sigma;
}
void sinegen_sci::dint(double t) {
sigma=dt;
}
void sinegen_sci::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
}
Event sinegen_sci::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=a*sin(w*t+phi);
if (strcmp(Method,"QSS2")==0){
  y[1]=a*w*cos(w*t+phi);
} else if (strcmp(Method,"QSS3")==0){
  y[1]=a*w*cos(w*t+phi);
  y[2]=-a*w*w*sin(w*t+phi)/2;
} else if (strcmp(Method,"QSS4")==0){
  y[1]=a*w*cos(w*t+phi);
  y[2]=-a*w*w*sin(w*t+phi)/2;
  y[3]=-a*w*w*w*cos(w*t+phi)/6;
};
return Event(&y[0],0);
}
void sinegen_sci::exit() {

}
