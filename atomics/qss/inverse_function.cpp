#include "inverse_function.h"
void inverse_function::init(double t,...) {
va_list parameters;
va_start(parameters, t);

sigma=INF; 
for (int i=0;i<10;i++){
y[i]=0;
u[i]=0;
};
order=1;
}
double inverse_function::ta(double t) {
return sigma;
}
void inverse_function::dint(double t) {
sigma=INF;
}
void inverse_function::dext(Event x, double t) {
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
Event inverse_function::lambda(double t) {
switch(order){ 
case 1: 
	y[0]=1/u[0];
break;
case 2: 
	y[0]=1/u[0];
	y[1]=-1/(u[0]*u[0])*u[1];
break;
case 3:
	y[0]=1/u[0];
	y[1]=-1/(u[0]*u[0])*u[1];
	y[2]=1/(u[0]*u[0]*u[0])*u[1]*u[1]-1/(u[0]*u[0])*u[2];
break;
case 4:
	y[0]=1/u[0];
	y[1]=-1/(u[0]*u[0])*u[1];
	y[2]=1/(u[0]*u[0]*u[0])*u[1]*u[1]-1/(u[0]*u[0])*u[2];
	y[3]=-1/(pow(u[0],4))*pow(u[1],3)+2/(u[0]*u[0]*u[0])*u[1]*u[2]-1/(u[0]*u[0])*u[3];
} 
return Event(y,0);
}
void inverse_function::exit() {

}
