#include "liqss3.h"
void liqss3::init(double t,...) {
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
//   qold_new2[i]=0;
};
char *fvar= va_arg(parameters,char*);
dQmin=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
dQrel=getScilabVar(fvar );
fvar= va_arg(parameters,char*);
X[0]=getScilabVar(fvar);
dQ=fabs(X[0])*dQrel;
if (dQ<dQmin){dQ=dQmin;};
dq=dQ; 
dq_old=dQ;
q_old=X[0];
q[0]=X[0];
a=0;
band=false; //this flag becomes true after sending an event. It becomes false after e>0.
band2=false; //this flag becomes true when a future situation dddx=0 is detected.
band3=false;//this flag becomes true after sending an event trying to provoke dddx=0.
band4=false;
sigma=0;
}
double liqss3::ta(double t) {
//This function return a double.
return sigma;
}
void liqss3::dint(double t) {
if (X[3]==0) {
	sigma=INF;
} else {
	sigma=pow(fabs(dQ/X[3]),1.0/3);
}; 

}
void liqss3::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *derx;
double diffxq[10];
double dt1,dt2,dt3;

derx=(double*)x.value;

if (x.port==0) { 
	//linear model estimation
		if((e>0)||(t==0)){
			band=false;
			band3=false;
		}
		if (band&&fabs(q_old+dq_old-q[0]-dq)>1e-12){
			a=(dx_old-derx[0])/(q_old+dq_old-q[0]-dq);
		       	if ((a<-1e30)||(a>0)){a=0;}
	  	} else {
			advance_time(q,e,2);
		};
		//printLog("en t=%g estimamos a=%g\n",t,a);
		u[0]=derx[0]-a*(q[0]+dq);
		u[1]=derx[1]-a*q[1];
		u[2]=derx[2]-a*q[2];

		X[0]=X[0]+X[1]*e+X[2]*e*e+X[3]*e*e*e;
	  X[1]=derx[0];
		X[2]=derx[1]/2;
		X[3]=derx[2]/3;
		//printLog("t=%g: e=%g, X=[%g, %g, %g, %g], q= [%g, %g, %g], u=[%g ,%g, %g], a=%g \n",t,e,X[0],X[1],X[2],X[3],q[0],q[1],q[2],u[0],u[1],u[2],a);
		if (band4){
		 //printLog("t=%g: e=%g, X=[%g, %g, %g, %g], q= [%g, %g, %g], u=[%g ,%g, %g], a=%g \n",t,e,X[0],X[1],X[2],X[3],q[0],q[1],q[2],u[0],u[1],u[2],a);
		 sigma=0;
		}
		if (sigma>0){
		  	diffxq[1]=q[1]-X[1];
			  diffxq[2]=q[2]-X[2];
			  diffxq[3]=-X[3];
	   		diffxq[0]=q[0]-X[0]-dQ;
		   	sigma=minposroot(diffxq,3);
	   		diffxq[0]=q[0]-X[0]+dQ;
		   	dt1=minposroot(diffxq,3);
		   	if (dt1<sigma) sigma=dt1;
				if (dt1!=sigma) {diffxq[0]=q[0]-X[0]-dQ;}
		 	  if (a!=0&&(fabs(X[3])>1e-10)&&!band3&&!band2){
					double diff1[10];
					diff1[0]=a*a*a*(q[0]+dq)+a*a*u[0]+a*u[1]+2*u[2];
					diff1[1]=a*a*a*q[1]+a*a*u[1]+a*2*u[2];
					diff1[2]=a*a*a*q[2]+a*a*u[2];
					dt3=minposroot(diff1,2);
					if (dt3<sigma){
						band2=true;				
						sigma=dt3;
					} else {
						band2=false;
					}
				}
  		//if (sigma==0)printLog("t=%g: e=%g, X=[%g, %g, %g, %g], q= [%g, %g, %g], u=[%g ,%g, %g], a=%g \n",t,e,X[0],X[1],X[2],X[3],q[0],q[1],q[2],u[0],u[1],u[2],a);
				if (sigma>getFinalTime())sigma=getFinalTime();
		  	advance_time(diffxq,sigma/2,3);
		  	if (fabs(diffxq[0])>3*dQ) {
					sigma=1e-12;	
		 	}
	
		};
} else {
	advance_time(X,e,3);
	X[0]=derx[0];
	sigma=0;
	band2=false;
  band=false;
  band3=false;
  band4=false;
  a=0;
}
}
Event liqss3::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
double ddx_est;
	band3=false;
 	advance_time(q,sigma,2);
	q_old=q[0];
	dq_old=dq;
	advance_time(u,sigma,2);
	advance_time(X,sigma,3);
	dx_old=X[1];
	//printLog("\n\n Lambda: t=%g: e=%g, X=[%g, %g, %g, %g], q= [%g, %g, %g], u=[%g ,%g, %g], a=%g \n",t,e,X[0],X[1],X[2],X[3],q[0],q[1],q[2],u[0],u[1],u[2],a);
	if (!band2) {
	  	q[0]=X[0];
		dQ=dQrel*fabs(X[0]);
		if (dQ<dQmin)dQ=dQmin;
	} else {
		band2=false;
	}
	if (a<-1e-30){        
	  if (X[3]>0) {
		//printLog("t=%g: Checking up \n",t);
 		//we try the upper value
		ddx_est=a*a*a*(q[0]+dQ)+a*a*u[0]+a*u[1]+2*u[2];
		if (ddx_est>=0) {
			dq=dQ; //OK
		} else {
			 dq=-2*u[2]/a/a/a-u[1]/a/a-u[0]/a-q[0];
			//printLog("Setting dq=%g\n",dq);
			band3=true;
		};
     		 if (fabs(dq)>dQ){dq=dQ;}
	   } else  {
		//printLog("t=%g: Checking down \n",t);
		//we try the lower value
		ddx_est=a*a*a*(q[0]-dQ)+a*a*u[0]+a*u[1]+2*u[2];
		if (ddx_est<=0) {
			dq=-dQ; //OK
		} else {
			dq=-2*u[2]/a/a/a-u[1]/a/a-u[0]/a-q[0];
			//printLog("Setting dq=%g\n",dq);
			band3=true;
		};
     		 if (fabs(dq)>dQ){dq=-dQ;}
	  };
	  //if (band4) printLog("band4=true\n");
    	  if (q[1]*X[1]<0&&!band4&&!band2&&!band3) {
  		if (q[1]<0) {
			dq=q_old-q[0]+dq_old-fabs(dq_old)*0.1;
  		} else {
			dq=q_old-q[0]+dq_old+fabs(dq_old)*0.1;
  		}
  		band4=true; //do it only once
		//  printLog("We detected a change\n");
	  } else if (band4) {
  		band4=false;
  		if (fabs(-2*u[2]/a/a/a-u[1]/a/a-u[0]/a-q[0])<3*dQ){ 
			dq=-2*u[2]/a/a/a-u[1]/a/a-u[0]/a-q[0];
			band3=true;
			//       printLog("We try provoke X[3]=0\n");

		}
	  }
 
	} else {
         	//not enough self feedback
		band4=false;
		if (X[3]>0) {dq=-dQ;} else {dq=dQ;}
	}
 	if (!band3) {
		q[1]=X[1];
		q[2]=X[2];
	} else {
		q[1]=a*(q[0]+dq)+u[0];
		q[2]=a*q[1]/2+u[1]/2;
	};

	y[0]=q[0]+dq;  
	y[1]=q[1];
	y[2]=q[2];
	band=true;
return Event(&y,0);
}
void liqss3::exit() {

}
