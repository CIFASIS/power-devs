#include "triangular_sci.h"
void triangular_sci::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
char *fvar=va_arg( parameters, char*);
a=getScilabVar(fvar );
fvar=va_arg( parameters, char*);
f=getScilabVar(fvar );
Method=va_arg(parameters,char*); 
fvar=va_arg( parameters, char*);
dQ=getScilabVar(fvar );

sgn=1;
pte=4*a*f;
sigma=0;
state=-a;
y[0]=state;
for(int i=0;i<10;i++){y[i]=0;};
}
double triangular_sci::ta(double t) {
//This function returns a double.
return sigma;
}
void triangular_sci::dint(double t) {
if(strcmp(Method,"QSS")==0){
    if(pte>0){
          if((a-y[0]-dQ)>0){
                 sigma=dQ/pte;
                 state=state+dQ;
          } else {
                 sigma=(a-y[0])/pte;
                 state=a;
                 pte=-pte;
          };
    } else{
          if((y[0]+a-dQ)>0){
                 sigma=-dQ/pte;
                 state=state-dQ;
          } else{
                 sigma=-(a+y[0])/pte;
                 pte=-pte;
                 state=-a;
          };
    }
} else{
    sgn=sgn*(-1);
    sigma=1/(2*f);
};
}
void triangular_sci::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
}
Event triangular_sci::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
if(strcmp(Method,"QSS")==0){
    y[0]=state;
    y[1]=0;   //pte
} else{
    y[0]=-a*sgn;
    y[1]=pte*sgn;
};
return Event(&y[0],0);
}
void triangular_sci::exit() {

}
