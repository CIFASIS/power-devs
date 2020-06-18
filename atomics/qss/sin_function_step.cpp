#include "sin_function_step.h"
void sin_function_step::init(double t,...) {
va_list parameters;
va_start(parameters, t);
char *advance_method_str=va_arg(parameters,char*);
purely_static=strcmp(advance_method_str,"Purely static") == 0;
char *dQrel_str=va_arg(parameters,char*);
char *dQmin_str=va_arg(parameters,char*);
dQrel = getScilabVar(dQrel_str);
dQmin = getScilabVar(dQmin_str);

sigma=INF; 
for (int i=0;i<10;i++){
y[i]=0;
u[i]=0;
};
order=1;

eps=1e-20;
max_iter=100;
}
double sin_function_step::ta(double t) {
return sigma;
}
void sin_function_step::dint(double t) {

if (purely_static) {
  sigma=INF;
  return;
}

double y_n1,y_n2;
switch (order)
{
	case 1:
		sigma=INF;
		return;
	break;
	case 2:
		if (fabs(u[1])<eps) { // No dynamic input
			sigma=INF;
			return;
		}
		y_n1=-sin(u[0])*u[1]*u[1]/2+cos(u[0])*u[2];
		y_n2=-cos(u[0])*pow(u[1],3)/6-sin(u[0])*u[1]*u[2]+cos(u[0])*u[3];
	break;
	case 3:
		if (fabs(u[1])<eps && fabs(u[2])<eps) { // No dynamic input
			sigma=INF;
			return;
		}
		y_n1=-cos(u[0])*pow(u[1],3)/6-sin(u[0])*u[1]*u[2]+cos(u[0])*u[3];
		y_n2=sin(u[0])*pow(u[1],4)/24 - cos(u[0])*u[1]*u[1]*u[2]/2 - sin(u[0])*u[1]*u[3] - sin(u[0])*u[2]*u[2]/2;
	break;
	case 4:
		if (fabs(u[1])<eps && fabs(u[2])<eps && fabs(u[3])<eps) { // No dynamic input
			sigma=INF;
			return;
		}
		y_n1=sin(u[0])*pow(u[1],4)/24 - cos(u[0])*u[1]*u[1]*u[2]/2 - sin(u[0])*u[1]*u[3] - sin(u[0])*u[2]*u[2]/2; 
		y_n2= -sin(u[0])*u[2]*u[3] - cos(u[0])*u[1]*u[1]*u[3]/2 - cos(u[0])*u[1]*u[2]*u[2]/2 + sin(u[0])*pow(u[1],3)*u[2]/6 + cos(u[0])*pow(u[1],5)/120;
	break;
}

tol=fabs(y[0])*dQrel;
if (tol<dQmin) 
  tol=dQmin;
sigma = estimate_sigma(y_n1,y_n2,order,tol,u,y,sin);

}
void sin_function_step::dext(Event x, double t) {
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
Event sin_function_step::lambda(double t) {
if (sigma!=0) {
	advance_time(u,sigma,order);
	if (fabs(sin(u[0])-evaluate_poly(y,sigma,order))>tol)
		printLog("[t=%g] Diff = %g y_n+1=%g y_n2=%g u={%g %g %g}\n",t,fabs(sin(u[0])-evaluate_poly(y,sigma,order)),y_n1,y_n2, u[0],u[1],u[2] );
}
switch(order){ 
case 1: 
	y[0]=sin(u[0]);
break;
case 2: 
	y[0]=sin(u[0]);
	y[1]=cos(u[0])*u[1];
break;
case 3:
	y[0]=sin(u[0]);
	y[1]=cos(u[0])*u[1];
	y[2]=-sin(u[0])*u[1]*u[1]/2+cos(u[0])*u[2];
break;
case 4:
	y[0]=sin(u[0]);
	y[1]=cos(u[0])*u[1];
	y[2]=-sin(u[0])*u[1]*u[1]/2+cos(u[0])*u[2];
	y[3]=-cos(u[0])*pow(u[1],3)/6-sin(u[0])*u[1]*u[2]+cos(u[0])*u[3];
} 
return 
Event(y,0);
}
void sin_function_step::exit() {

}
