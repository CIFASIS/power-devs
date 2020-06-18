#include "samplehold.h"
void samplehold::init(double t,...) {
va_list parameters;
va_start(parameters,t);
char *fvar= va_arg(parameters,char*);
T= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
phi= getScilabVar(fvar );
for(int i=0;i<10;i++){
            u[i]=0;
            y[i]=0;
}
sigma=phi;
order=1;
}
double samplehold::ta(double t) {
return sigma;
}
void samplehold::dint(double t) {
sigma=T;
}
void samplehold::dext(Event x, double t) {
double *xv; 
xv=(double*)(x.value);
switch(order){
case 1: 
u[0]=xv[0];
if (xv[1]!=0){order=2;u[1]=xv[1];}
if (xv[2]!=0){order=3;u[2]=xv[2];} 
if (xv[3]!=0){order=4;u[3]=xv[3];} 
if (xv[4]!=0){order=5;u[4]=xv[4];} 

break;
case 2: 
u[0]=xv[0];
u[1]=xv[1];
if (xv[2]!=0){order=3;u[2]=xv[2];} 
if (xv[3]!=0){order=4;u[3]=xv[3];} 
if (xv[4]!=0){order=5;u[4]=xv[4];} 
break;
case 3:
u[0]=xv[0];
u[1]=xv[1];
u[2]=xv[2];
if (xv[3]!=0){order=4;u[3]=xv[3];} 
if (xv[4]!=0){order=5;u[4]=xv[4];} 
break;
case 4:
u[0]=xv[0];
u[1]=xv[1];
u[2]=xv[2];
u[3]=xv[3];
if (xv[4]!=0){order=5;u[4]=xv[4];} 
break;

case 5:
u[0]=xv[0];
u[1]=xv[1];
u[2]=xv[2];
u[3]=xv[3];
u[4]=xv[4];
}
sigma=sigma-e;
}
Event samplehold::lambda(double t) {
if (order>0)advance_time(u,sigma,order-1);//this should be at the internal transition
y[0]=u[0];

return Event(y,0);
}
void samplehold::exit() {

}
