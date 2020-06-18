#include "hswitchtraj.h"
void hswitchtraj::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
inf=1e10;
for (int i=0;i<4;i++) {
	u[i]=0;
	mu[i]=0;
	pu[i]=0;
};
sigma=inf;
tcross=inf;
sw=0;
for(int i=0;i<10;i++){y[i]=0;};
ch=0;
}
double hswitchtraj::ta(double t) {
//This function returns a double.
return sigma;
}
void hswitchtraj::dint(double t) {
if (sigma==0) {
	sigma=tcross;
} else {
	for (int i=0;i<4;i++) {
		u[i]=u[i]+mu[i]*sigma+pu[i]*sigma*sigma;
		mu[i]=mu[i]+2*pu[i]*sigma;
	};
	if ((mu[2]-mu[3])*(pu[2]-pu[3])<0) {
		sigma=(mu[3]-mu[2])/(pu[2]-pu[3]);
	} else {
		sigma=inf;
	};
	tcross=sigma;
	sw=1-sw;
};
ch=0;
}
void hswitchtraj::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double *xv;
xv=(double*)x.value;
for (int i=0;i<4;i++) {
	if (i!=x.port) {
		u[i]=u[i]+mu[i]*e+pu[i]*e*e;
		mu[i]=mu[i]+2*pu[i]*e;
	};
};
u[x.port]=xv[0];
mu[x.port]=xv[1];
pu[x.port]=xv[2];
if ((x.port==2)||(x.port==3)){
   //calculate time to next crossing
	double a,b,c,s1,s2;
	a=pu[2]-pu[3];
	b=mu[2]-mu[3];
	c=u[2]-u[3];
	if (a==0) {
		if (b==0){
			s1=inf;
			s2=inf;
		} else {
			s1=-c/b;
			s2=inf;
		};
	} else {
		s1=(-b+sqrt(b*b-4*a*c))/2/a;
		s2=(-b-sqrt(b*b-4*a*c))/2/a;
	};
	if ((s1>0)&&((s1<s2)||(s2<0))) {
		tcross=s1;
	} else {
		if (s2>0) {
			tcross=s2;
		} else {
			tcross=inf;
		};
	};  
	if ((sigma==e)&&(((u[2]>u[3])&&(sw==0))||((u[3]<u[2])&&(sw==1)))) {
		sigma=0;
	} else {
		if (((u[2]-u[3]>0)&&(sw==1))||((u[2]-u[3]<0)&&(sw==0))){
			sw=1-sw;
			ch=1;
			sigma=0;
		} else {
			sigma=tcross; 
		};
	};
} else {
	tcross=tcross-e;
             if(sigma-e==0){
		if((sw==x.port)||(ch==1)){
			sigma=0;
		}else{
			sigma=tcross;
		};
	}else{
		if ((u[2]-u[3]>0)&&(x.port==0)) {
			sigma=0;
			sw=0;
  		}else{
			if ((u[2]-u[3]<0)&&(x.port==1)) {
				sigma=0;
				sw=1;
			} else {
    				sigma=tcross;
  			};
		};
	}
};
}
Event hswitchtraj::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
if (sigma==0) {
	y[0]=u[sw];
	y[1]=mu[sw];
	y[2]=pu[sw];
} else {
	y[0]=u[1-sw]+mu[1-sw]*sigma+pu[1-sw]*sigma*sigma;
	y[1]=mu[1-sw]+2*pu[1-sw]*sigma;
	y[2]=pu[1-sw];
};
return Event(&y[0],0);
}
void hswitchtraj::exit() {

}
