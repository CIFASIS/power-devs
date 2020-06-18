#include "modelica_when_discrete.h"
void modelica_when_discrete::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
//We get the parameters here

Inputs = (int)va_arg(parameters,double);
Outputs = (int)va_arg(parameters,double);
Index = (int)va_arg(parameters,double);
inf=1e20;

//Initialization of states and outputs
for (int j=0;j<Inputs;j++) {
  u[j]=0;
  mu[j]=0;
  pu[j]=0;
 };
yv=0;
 
for (int i=0;i<10;i++) {
  y[i]=0;
};
lastport=-1;

//Start with a passive state
sigma=inf;
}
double modelica_when_discrete::ta(double t) {
//This function returns a double.
return sigma;
}
void modelica_when_discrete::dint(double t) {
lastport--;
if (lastport<0) 
  sigma=inf; // Finishing emiting events
else 
  sigma=0; // Continue to emit events
}
void modelica_when_discrete::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double *xv;
xv=(double*)(x.value); //here we take the input event value

for (int j=0;j<Inputs-1;j++) {
  if (j!=x.port) {
    u[j]=u[j]+mu[j]*e+pu[j]*e*e;
    mu[j]=mu[j]+2*pu[j]*e;
  };
};
if (x.port!=Inputs-1) { //not a dummy event 
  u[x.port]=xv[0];
  mu[x.port]=xv[1]; 
  pu[x.port]=xv[2];
  sigma=inf;
  if (lastport>=0) // Still got some events to emit
    sigma=0;
} else {  //dummy event--> discrete evolution
  if(functionQssWhen(Index,t,u,y)) {
    sigma=0;
    lastport=Outputs-1;
  }
};
}
Event modelica_when_discrete::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
//  y[0]=1
return Event(&y[lastport],lastport);
}
void modelica_when_discrete::exit() {

}
