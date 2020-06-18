#include "rama_inv.h"
void rama_inv::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
Vdwn = va_arg(parameters,double);
Vup = va_arg(parameters,double);
inf = 1e10;
sigma = inf;
for (int i=0;i<3;i++) {
  u[i]=0;
};
for (int i=0;i<10;i++) {
  y[i]=0;
};
sal=0;
}
double rama_inv::ta(double t) {
//This function returns a double.
return sigma;
}
void rama_inv::dint(double t) {
sigma = inf;
}
void rama_inv::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double xv;
xv=*(double*)x.value;
u[x.port]=xv;
if (fabs(u[0]+u[1]) < 0.001) { 
                if (u[2]<0) { 
                       sal = Vup;
                       } else {
                                  if (fabs(u[2])<0.001) {
                                         sal = 0;
                                          } else {
                                           sal = Vdwn;
                                   };
                 };
} else { 
         if (fabs(u[0]-1)<0.001) { 
                        sal = Vup;
          } else { 
                        sal = Vdwn;
          };
};  
sigma=0;
}
Event rama_inv::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0] = sal;
return Event(&y[0],0);
}
void rama_inv::exit() {

}
