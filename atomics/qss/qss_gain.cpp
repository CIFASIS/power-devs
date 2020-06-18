#include "qss_gain.h"
void qss_gain::init(double t,...) {
va_list parameters;
va_start(parameters,t);
char *fvar= va_arg(parameters,char*);
K=getScilabVar(fvar );
for(int i=0;i<10;i++){u[i]=0;};  
for(int i=0;i<10;i++){y[i]=0;};  
order=1;
sigma=INF;
}
double qss_gain::ta(double t) {
return sigma;
}
void qss_gain::dint(double t) {
sigma=INF;
}
void qss_gain::dext(Event x, double t) {
double *xv;
xv=(double*)(x.value);
switch(order){
  case 1:   
     u[0]=xv[0];
     if (xv[1]!=0){order=2;u[1]=xv[1];}
     if (xv[2]!=0){order=3;u[2]=xv[2];}  
     if (xv[3]!=0){order=4;u[3]=xv[3];}  

   break;
  case 2:  
     u[0]=xv[0];
     u[1]=xv[1];
      if (xv[2]!=0){order=3;u[2]=xv[2];} 
      if (xv[3]!=0){order=4;u[3]=xv[3];}  
 
   break;
  case 3:
        u[0]=xv[0];
        u[1]=xv[1];
        u[2]=xv[2];
        if (xv[3]!=0){order=4;u[3]=xv[3];}  
   break;
  case 4:
        u[0]=xv[0];
        u[1]=xv[1];
        u[2]=xv[2];
		  u[3]=xv[3];
 }
 sigma=0;
}
Event qss_gain::lambda(double t) {
switch(order){
  case 1:   
    y[0]=K*u[0];
   break;
  case 2:  
    y[0]=K*u[0];
    y[1]=K*u[1];
   break;
  case 3:
      y[0]=K*u[0];
      y[1]=K*u[1];
      y[2]=K*u[2];
  break;
  case 4:
      y[0]=K*u[0];
      y[1]=K*u[1];
      y[2]=K*u[2];
      y[3]=K*u[3];

 }  
  return Event(&y[0],0);
}
void qss_gain::exit() {

}
