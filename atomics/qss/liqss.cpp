#include "liqss.h"
void liqss::init(double t,...) {
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
for (int i=0;i<10;i++) {
	y[i]=0;
 	X[i]=0;
	q[i]=0;
	u[i]=0;
};

char *fvar= va_arg(parameters,char*);
dQmin=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
dQrel=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
X[0]=getScilabVar(fvar);
dQ=fabs(X[0])*dQrel;
dq=dQ; 
dq_old=dQ;
q_old=X[0];
if (dQ<dQmin){dQ=dQmin;};
q[0]=X[0];
a=0;	 
band=false;// //this flag becomes true after sending an event. It becomes false after e>0.
band3=false;
sigma=0;

}
double liqss::ta(double t) {
//This function return a double.
return sigma;
}
void liqss::dint(double t) {
if (X[1]==0){
	sigma=INF;
} else {
	sigma=fabs(dQ/X[1]);
};
}
void liqss::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *derx;
double diffxq[10];
double dt1;
double aold;

derx=(double*)x.value;
if (x.port==0) {
	if(e>0||t==0){band=false;}
	if (band&&fabs(q_old+dq_old-q[0]-dq)>1e-10){ 		//self feedback
			aold=a;
			a=(dx_old-derx[0])/(q_old+dq_old-q[0]-dq);
			if ((a<-1e30)||(a>0)){a=0;}
	};

	u[0]=derx[0]-a*(q[0]+dq);
	X[0]=X[0]+X[1]*e;
	X[1]=derx[0];
	if (band3) sigma=0;
	//printLog("Ext: t=%g: a=%g, X=[%g, %g], u=[%g], e=%g\n",t,a,X[0],X[1],u[0],e);  
	if (sigma>0){
	   	diffxq[1]=-X[1];
	   	diffxq[0]=q[0]-X[0]-dQ;
	   	sigma=minposroot(diffxq,1);
	   	diffxq[0]=q[0]-X[0]+dQ;
	   	dt1=minposroot(diffxq,1);
	   	if (dt1<sigma) sigma=dt1;
		  	if (fabs(X[0]-q[0])>dQ) sigma=1e-12;
	};
} else {
	X[0]=derx[0];
	sigma=0;
}
}
Event liqss::lambda(double t) {
//double ddx_est;
//this should be at the internal transition
//we put here to optimize code

q_old=q[0];
dq_old=dq;
advance_time(X,sigma,1);
dx_old=X[1];
q[0]=X[0];
dQ=dQrel*fabs(X[0]);
if (dQ<dQmin)dQ=dQmin;
if (a<-1e-30){ 
	if (X[1]>0) {
		//we try the upper value
		if (a*(q[0]+dQ)+u[0]>=0) {
			dq=dQ; //OK
		} else {
			 dq=-u[0]/a-q[0];
			if (fabs(dq)>dQ)dq=dQ;
		};
	} else {
		//we try the lower value
		if (a*(q[0]-dQ)+u[0]<=0) {
			dq=-dQ; //OK
		} else {
			dq=-u[0]/a-q[0];
			if (fabs(dq)>dQ)dq=-dQ;

		};
	} 
if (dq*dq_old<0&&!band3) {
  q[0]=q_old; //posible error
  dq=dq_old*1.1;	
  band3=true; //do it only once
  //printLog("We detected a change\n");
} else if (band3) {
  band3=false;
  if (fabs(-u[0]/a-q[0])<2*dQ) dq=-u[0]/a-q[0];
}

} else {
  if (X[1]>0) {dq=dQ;} else {dq=-dQ;}
}
//printLog("t=%g: a=%g, X=[%g, %g], u=[%g], dq=%g\n",t,a,X[0],X[1],u[0],dq);   
y[0]=q[0]+dq;
band=true;
return Event(&y,0);
}
void liqss::exit() {

}
