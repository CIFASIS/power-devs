#include "cross_detect.h"
void cross_detect::init(double t,...) {
va_list parameters;
va_start(parameters,t);

char *fvar= va_arg(parameters,char*); 
level= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
outval= getScilabVar(fvar );

for(int i=0;i<10;i++){u[i]=0;};  
for(int i=0;i<10;i++){y[i]=0;};  

sigma=INF;
sw=0;
}
double cross_detect::ta(double t) {
return sigma;
}
void cross_detect::dint(double t) {
advance_time(u,sigma,4);
sw=-sw;
u[0]=1e-20*sw;//to correct numerical errors
sigma=minposroot(u,4);

}
void cross_detect::dext(Event x, double t) {
double *xv;
xv=(double*)(x.value);

u[0]=xv[0]-level;
u[1]=xv[1];
u[2]=xv[2];
u[3]=xv[3];
u[4]=xv[4];


if (((sw==1)&&(u[0]<0))||((sw==-1)&&(u[0]>0))) {
	sigma=0;
} else {
  sigma=minposroot(u,4);
  if (sw==0) {
		if (u[0]>0) {
			sw=1;
		} else {
			sw=-1;
		};
	};
};
			
		




}
Event cross_detect::lambda(double t) {
y[0]=outval;
return Event(y,0);
}
void cross_detect::exit() {

}
