#include "liqss_integrator.h"
void liqss_integrator::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
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
Method=va_arg(parameters,char*);
char *fvar= va_arg(parameters,char*);
dQmin=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
dQrel=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
X[0]=getScilabVar(fvar);
dQ=fabs(X[0])*dQrel;
if (dQ<dQmin){dQ=dQmin;};

if (strcmp(Method,"LIQSS1")==0) {
          met=1;
          q[0]=X[0];
   		  a=0;	 
};

if (strcmp(Method,"LIQSS2")==0) {
      met=2;       
      q[0]=X[0];
};
if (strcmp(Method,"LIQSS3")==0){
        q[0]=X[0];
        met=3;  
};
if (strcmp(Method,"LIQSS4")==0){
        q[0]=X[0];
        met=4;  
};

sigma=0;
}
double liqss_integrator::ta(double t) {
//This function return a double.
return sigma;
}
void liqss_integrator::dint(double t) {
switch(met) {
case 1: // LIQSS

	dQ=dQrel*fabs(X[0]);
	if (dQ<dQmin)dQ=dQmin;
	if (X[1]==0){
		sigma=INF;
	} else {
		sigma=fabs(dQ/X[1]);
	};
break;
case 2: // LIQSS2

	dQ=dQrel*fabs(X[0]);
	if (dQ<dQmin)dQ=dQmin;
 	if (X[2]==0){
		sigma=INF;
 	} else {
 		sigma=sqrt(fabs(dQ/X[2]));
	};  		
break;
case 3:  // LIQSS3
 
	dQ=dQrel*fabs(X[0]);
	if (dQ<dQmin)dQ=dQmin;
   if (X[3]==0) {
		sigma=INF;
	} else {
		sigma=pow(fabs(dQ/X[3]),1.0/3);
	};  		
break;
case 4:  // LIQSS4
  	advance_time(X,sigma,4);
 	q[0]=X[0];
 	q[1]=X[1];
 	q[2]=X[2];
 	q[3]=X[3];
	dQ=dQrel*fabs(X[0]);
	if (dQ<dQmin)dQ=dQmin;
   if (X[4]==0) {
		sigma=INF;
	} else {
		sigma=pow(fabs(dQ/X[4]),1.0/4);
	};  		
break;

}
}
void liqss_integrator::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *derx;
double diffxq[10];
double dt1;

derx=(double*)x.value;
 
switch(met) {
case 1: //LIQSS
	if ((e==0)&&(t>0)){
		//self feedback
		a=(X[1]-derx[0])/(q_old+dq_old-q[0]-dq);
	}; 
	u[0]=derx[0]-a*(q[0]+dq);
	//printLog("We estimate a=%g y u=%g \n",a,u);

   X[0]=X[0]+X[1]*e;
   X[1]=derx[0];
	if (sigma>0){
   	diffxq[1]=-X[1];
   	diffxq[0]=q[0]-X[0]-dQ;
   	sigma=minposroot(diffxq,1);
   	diffxq[0]=q[0]-X[0]+dQ;
   	dt1=minposroot(diffxq,1);
   	if (dt1<sigma) sigma=dt1;
   	if (fabs(X[0]-q[0])>dQ) sigma=0;
	};
	break;  
break;
case 2: // LIQSS2

	//linear model estimation
	if ((e==0)&&(t>0)){
		//self feedback
		a=(X[1]-derx[0])/(q_old+dq_old-q[0]-dq);
   } else {
	   advance_time(q,e,1);
	}; 
	u[0]=derx[0]-a*(q[0]+dq);
	u[1]=derx[1]-a*q[1];

   X[0]=X[0]+X[1]*e+X[2]*e*e;
   X[1]=derx[0];
   X[2]=derx[1]/2;
	if (sigma>0){
   	diffxq[1]=q[1]-X[1];
		diffxq[2]=-X[2];
   	diffxq[0]=q[0]-X[0]-dQ;
   	sigma=minposroot(diffxq,2);
   	diffxq[0]=q[0]-X[0]+dQ;
   	dt1=minposroot(diffxq,2);
   	if (dt1<sigma) sigma=dt1;
   	if (fabs(X[0]-q[0])>dQ) sigma=0;
	};
	break;
case 3: // LIQSS3
 
	//linear model estimation
	if (((e==0)&&(sigma>0))&&(t>0)){
		//self feedback
		a=(X[1]-derx[0])/(q_old+dq_old-q[0]-dq);
   } else {
	   advance_time(q,e,2);
	}; 
	u[0]=derx[0]-a*(q[0]+dq);
	u[1]=derx[1]-a*q[1];
	u[2]=derx[2]-a*q[2];

   X[0]=X[0]+X[1]*e+X[2]*e*e+X[3]*e*e*e;
   X[1]=derx[0];
   X[2]=derx[1]/2;
   X[3]=derx[2]/3;

	printLog("**************** t=%g  external ********************\n",t);
   printLog("We have X[0]=%g  X[1]=%g  X[2]=%g   X[3]=%g \n",X[0],X[1],X[2],X[3]);
	printLog("We estimate a=%g, u[0]=%g  u[1]=%g  u[2]=%g  \n",a,u[0],u[1],u[2]);


	if (sigma>0){
	  	diffxq[1]=q[1]-X[1];
		diffxq[2]=q[2]-X[2];
		diffxq[3]=-X[3];
   	diffxq[0]=q[0]-X[0]-dQ;
   	sigma=minposroot(diffxq,3);
   	diffxq[0]=q[0]-X[0]+dQ;
   	dt1=minposroot(diffxq,3);
   	if (dt1<sigma) sigma=dt1;
	  	if (fabs(X[0]-q[0])>dQ) sigma=0;       
	};
break;
case 4: // LIQSS4
   X[0]=X[0]+X[1]*e+X[2]*e*e+X[3]*e*e*e+X[4]*e*e*e*e;
   X[1]=derx[0];
   X[2]=derx[1]/2;
   X[3]=derx[2]/3;
   X[4]=derx[3]/4;
	if (sigma>0){
	   advance_time(q,e,3);
   	diffxq[1]=q[1]-X[1];
		diffxq[2]=q[2]-X[2];
		diffxq[3]=q[3]-X[3];
		diffxq[4]=-X[4];
   	diffxq[0]=q[0]-X[0]-dQ;
   	sigma=minposroot(diffxq,4);
   	diffxq[0]=q[0]-X[0]+dQ;
   	dt1=minposroot(diffxq,4);
   	if (dt1<sigma) sigma=dt1;
   	if (fabs(X[0]-q[0])>dQ) sigma=0;       
	};
break;
};
}
Event liqss_integrator::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)

double ddx_est;
switch(met) {
case 1: // LIQSS
//this should be at the internal transition
//we put here to optimize code

	q_old=q[0];
	dq_old=dq;
	advance_time(X,sigma,1);
 	q[0]=X[0];
   if (X[1]>0) {
		//we try the upper value
		if (a*(q[0]+dQ)+u[0]>=0) {
			dq=dQ; //OK
		} else {
			 dq=-u[0]/a-q[0];
		};
	} else {
		//we try the lower value
		if (a*(q[0]-dQ)+u[0]<=0) {
			dq=-dQ; //OK
		} else {
			dq=-u[0]/a-q[0];
		};
	};   
  y[0]=q[0]+dq;
break;

case 2: // LIQSS2


 	advance_time(q,sigma,1);
	q_old=q[0];
	dq_old=dq;
	advance_time(u,sigma,1);
	advance_time(X,sigma,2);
 	q[0]=X[0];

  
   if (X[1]*fabs(a)-u[1]>0) {
		//we try the upper value
		ddx_est=(a*(q[0]+dQ)+u[0])*fabs(a)-u[1];
		if (ddx_est>=0) {
			dq=dQ; //OK
		} else {
			 dq=-u[1]/a/a-u[0]/a-q[0];
		};
	} else {
		//we try the lower value
	   ddx_est=(a*(q[0]-dQ)+u[0])*fabs(a)-u[1];
		if (ddx_est<=0) {
			dq=-dQ; //OK
		} else {
			dq=-u[1]/a/a-u[0]/a-q[0];
		};
	};   
	q[1]=a*(q[0]+dq)+u[0];

	y[0]=q[0]+dq;  
	y[1]=q[1];



break;
case 3: // LIQSS3

 	advance_time(q,sigma,2);
	q_old=q[0];
	dq_old=dq;
	advance_time(u,sigma,2);
	advance_time(X,sigma,3);
 	q[0]=X[0];

   if (X[2]*fabs(a)-u[2]<0) {
		//we try the upper value
		ddx_est=(a* (a* (q[0]+dQ) +u[0] )+u[1])*fabs(a)/2-u[2];
		if (ddx_est<=0) {
			dq=dQ; //OK
		} else {
			 dq=-2*u[2]/a/a/a-u[1]/a/a-u[0]/a-q[0];
		};
	} else {
		//we try the lower value
	  ddx_est=(a*(a*(q[0]-dQ)+u[0])+u[1])*fabs(a)/2-u[2];
		//printLog("We are here \n");
		if (ddx_est>=0) {
			dq=-dQ; //OK
		} else {
			dq=-2*u[2]/a/a/a-u[1]/a/a-u[0]/a-q[0];
		};
	};   
	//if (fabs(dq)>fabs(dQ)) dq=dq*dQ/fabs(dq);
	q[1]=a*(q[0]+dq)+u[0];
	q[2]=a*q[1]/2+u[1]/2;
	
	y[0]=q[0]+dq;  
	y[1]=q[1];
	y[2]=q[2];
	printLog("**************** t=%g   ********************\n",t);
   printLog("We have X[0]=%g  X[1]=%g  X[2]=%g   X[3]=%g \n",X[0],X[1],X[2],X[3]);
	printLog("ddx_est= %g  Output: y[0]=%g  y[1]=%g  y[2]=%g  with dq=%g \n",ddx_est,y[0],y[1],y[2],dq);

break;
case 4: //LIQSS4
  y[0]=X[0];
  y[1]=X[1];
  y[2]=X[2];	 
  y[3]=X[3];	 
  y[4]=X[4];	 
  advance_time(y,sigma,4);
  y[4]=0;	 
  break;
};
return Event(&y,0);
}
void liqss_integrator::exit() {

}
