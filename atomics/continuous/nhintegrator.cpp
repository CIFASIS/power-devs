#include "nhintegrator.h"
void nhintegrator::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
     X=va_arg(parameters,double);
    dx=0;
    q=2*floor((X-1)/2)+1;
    inf=1e10;
    sigma=0;
    int i;
    for (i=0;i<10;i++) y[i]=0;
    dq=2;
}
double nhintegrator::ta(double t) {
//This function returns a double.
  return sigma;
}
void nhintegrator::dint(double t) {
X=X+sigma*dx;
   if (dx>0) {
      q=q+dq;
      sigma=dq/dx;
   }else {
     if (dx<0){
         sigma=-dq/dx;
          q=q-dq;
      }else {
         sigma=inf;
       }
     }
//printLog("DINT [t=%g] q=%g y dx=%g \n",t,q,dx);
}
void nhintegrator::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double xv;
xv = *(double*)(x.value);
X = X + dx* e;
  if (xv > 0) 
    sigma = (q + dq - X)/xv;
   else 
     if (xv < 0) 
       sigma = (q -  X)/xv;
       else 
       sigma = inf;
dx = xv;
printLog("DEXT [t=%g] q=%g, X=%g, dx=%g, sigma=%g\n",t,q , X,dx,sigma);
}
Event nhintegrator::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
if (dx==0) { 
  y[0] = q; 
} 
else {
y[0]=q+dq*dx/fabs(dx);
}
printLog("LAMBDA Emito el evento %g en t=%g (q=%g, dx=%g)\n",y[0],t,q,dx);
return Event(&y,0);
}
void nhintegrator::exit() {
//Code executed at the end of the simulation.
}
