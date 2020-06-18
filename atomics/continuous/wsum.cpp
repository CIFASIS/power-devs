#include "wsum.h"
void wsum::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
K[0]=va_arg(parameters, double);
K[1]=va_arg(parameters, double);
K[2]=va_arg(parameters, double);
K[3]=va_arg(parameters, double);
K[4]=va_arg(parameters, double);
K[5]=va_arg(parameters, double);
K[6]=va_arg(parameters, double);
K[7]=va_arg(parameters, double);
n=(int) va_arg(parameters, double);
// Modification Algoritm (Mario)
order=1;
// End modification
for (int i=0;i<8;i++) {
  Xs[i]=0;
  Mxs[i]=0;
  Pxs[i]=0;
  S[i]=0;
};
S[8]=0;
S[9]=0;
Sigma=4e10;
}
double wsum::ta(double t) {
//This function returns a double.
return Sigma;
}
void wsum::dint(double t) {
Sigma=4e10;
}
void wsum::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double *Xv;
Xv=(double*)x.value;
switch(order) {
case 1: Xs[x.port]=Xv[0];
     if(Xv[1]!=0){ order=2;Mxs[x.port]=Xv[1];}
     if(Xv[2]!=0){ order=3;Pxs[x.port]=Xv[2];}  
break;
case 2: Xs[x.port]=Xv[0];
      for (int i=0;i<n;i++) {
       if (i!=x.port) {
        Xs[i]=Xs[i]+Mxs[i]*e; };
};
            Mxs[x.port]=Xv[1];
     if(Xv[2]!=0){ order=3;Pxs[x.port]=Xv[2];}  
break;
case 3: Xs[x.port]=Xv[0];
           Mxs[x.port]=Xv[1];
            Pxs[x.port]=Xv[2];  
               for (int i=0;i<n;i++) {
                 if (i!=x.port) {
                   Xs[i]=Xs[i]+Mxs[i]*e+Pxs[i]*e*e;
               Mxs[i]=Mxs[i]+2*Pxs[i]*e;};
};
}
Sigma=0;
}
Event wsum::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
S[0]=0;
S[1]=0;
S[2]=0;
switch(order) {
case 1: for(int i=0;i<n;i++) {
       S[0]=S[0]+K[i]*Xs[i];
}
break;
case 2: for(int i=0;i<n;i++) {
         S[0]=S[0]+K[i]*Xs[i];
            S[1]=S[1]+K[i]*Mxs[i];
}
break;
case 3: for(int i=0;i<n;i++) {
  S[0]=S[0]+K[i]*Xs[i];
  S[1]=S[1]+K[i]*Mxs[i];
  S[2]=S[2]+K[i]*Pxs[i];
}
}
return Event(&S[0],0);
}
void wsum::exit() {

}
