#include "printegrator.h"
void printegrator::init(double t, ...){
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
X=va_arg(parameters,double);
inf=1e20;
u=0;
mu=0;
pu=0;
sigma=0;
for (int i=0;i<10;i++) {
    y[i]=0;
};
}

double printegrator::ta(double t){
//This function return a double.
return sigma;
}

void printegrator::dint(double t){
sigma=inf;
}

void printegrator::dext(Event x, double t){
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *Aux;
Aux=(double*)x.value;
if (x.port==0){
X=X +u*e+(mu*e*e)/2+(pu*e*e*e)/3;
u=Aux[0];    //input value
mu=Aux[1]; //input slope
pu=Aux[2]; //input derivative
} else {
  X=Aux[0];
};
sigma=0;
}

Event printegrator::lambda(double t){
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
 y[0]=X;
 y[1]=u; 
 y[2]=mu/2.0;
 y[3]=pu/3.0;
return Event(&y,0);
}

