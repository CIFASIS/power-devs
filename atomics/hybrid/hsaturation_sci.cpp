#include "hsaturation_sci.h"
void hsaturation_sci::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
char *fvar= va_arg(parameters,char*);
xl= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
xu= getScilabVar(fvar );
inf=1e20;
for(int i=0;i<10;i++){y[i]=0;};
sigma=inf;
u1=(xu+xl)/2;
u=0;
mu=0;
pu=0;
}
double hsaturation_sci::ta(double t) {
//This function returns a double.
return sigma;
}
void hsaturation_sci::dint(double t) {
if(sigma!=0){
	u=u+mu*sigma+pu*sigma*sigma;
	mu=mu+2*pu*sigma;
	if( u<((xu+xl)/2) ){u=xl;}else{u=xu;};
};
double a, b, c;
double s[2];
double sol[2];
a=pu;
b=mu;
s[0]=s[1]=inf;
sol[0]=sol[1]=inf;
for(int i=0;i<2;i++){
	if(i==0){c=u-xl;}else{c=u-xu;};
	if(a==0){
		if(b!=0){s[0]=-c/b;};
	}else{
		if(b*b>=4*a*c){
			s[0]=(-b+sqrt(b*b-4*a*c))/(2*a);
			s[1]=(-b-sqrt(b*b-4*a*c))/(2*a);
		};
	};
	if(s[0]<=0){s[0]=inf;};
	if(s[1]<=0){s[1]=inf;};
	if(s[0]<s[1]){sol[i]=s[0];}else{sol[i]=s[1];};
};
if(sol[0]<sol[1]){sigma=sol[0];}else{sigma=sol[1];};
}
void hsaturation_sci::dext(Event x, double t) {
u1=u+mu*e+pu*e*e;
double *xv;
xv=(double*)(x.value);
u=xv[0];
mu=xv[1];
pu=xv[2];
level[0]=level[1]=1;
if(u1<xl){level[0]=0;}else{if(u1>xu){level[0]=2;};};
if(u<xl){level[1]=0;}else{if(u>xu){level[1]=2;};};
if( ((u<=xu)&&(u>=xl)) || (level[0]!=level[1])){
	sigma=0;
}else{
	double sol[2];
	double s[2];
	double a=pu;
	double b=mu;
	double c;	
	s[0]=s[1]=inf;
	sol[0]=sol[1]=inf;
	for (int i=0;i<2;i++){
		if(i==0){c=u-xu;}else{c=u-xl;};
		if(a==0){
			if(b!=0){s[0]=-c/b;};
		}else{
			if(b*b>=4*a*c){
				s[0]=(-b+sqrt(b*b-4*a*c))/(2*a);
				s[1]=(-b-sqrt(b*b-4*a*c))/(2*a);
			};
		};
	
		if(s[0]<=0){s[0]=inf;};
		if(s[1]<=0){s[1]=inf;};
		if(s[0]<s[1]){sol[i]=s[0];}else{sol[i]=s[1];};	
	}; 
	if(sol[0]<sol[1]){sigma=sol[0];}else{sigma=sol[1];};
};
}
Event hsaturation_sci::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
if( (sigma!=0)||(level[1]!=1) ){
	if( (u+mu*sigma+pu*sigma*sigma)>((xl+xu)/2) ){y[0]=xu;}else{y[0]=xl;};
}else{
	y[0]=u;
};
y[1]=mu+2*pu*sigma;
y[2]=pu;
if( (y[0]==xu) && ( (y[1]>0) || ( (y[1]==0)&&(y[2]>0) ) ) ){
	y[1]=0;
	y[2]=0;
} else{
	if( (y[0]==xl) && ( (y[1]<0) || ( (y[1]==0)&&(y[2]<0) ) ) ){
		y[1]=0;
		y[2]=0;
	};
};
//revisar casp limite con b=0 y a><0
return Event(&y[0],0);
}
void hsaturation_sci::exit() {

}
