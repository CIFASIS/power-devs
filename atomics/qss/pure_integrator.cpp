#include "pure_integrator.h"
void pure_integrator::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type

for (int i=0;i<10;i++) {
   X[i]=0; 
	y[i]=0;
};
char *fvar=va_arg(parameters,char*);
X[0]=getScilabVar(fvar);
sigma=0;
}
double pure_integrator::ta(double t) {
//This function return a double.
return sigma;
}
void pure_integrator::dint(double t) {
sigma=INF;
}
void pure_integrator::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *derx;
derx=(double*)x.value;
X[0]=evaluate_poly(X,e,4);
X[1]=derx[0];    
X[2]=derx[1]/2;
X[3]=derx[2]/3;
X[4]=derx[3]/4;
sigma=0;
}
Event pure_integrator::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
 y[0]=X[0];
 y[1]=X[1]; 
 y[2]=X[2];
 y[3]=X[3];
 y[4]=X[4];

return Event(&y,0);
}
void pure_integrator::exit() {

}
