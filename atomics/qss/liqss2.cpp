#include "liqss2.h"
void liqss2::init(double t,...) {
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
band2=false; //this flag becomes true when a future situation ddx=0 is detected.
band3=false;//this flag becomes true after sending an event trying to provoke ddx=0.
band4=false;
sigma=0;

}
double liqss2::ta(double t) {
//This function return a double.
return sigma;
}
void liqss2::dint(double t) {
if (X[2]==0){
	sigma=INF;
} else {
	sigma=sqrt(fabs(dQ/X[2]));
};
}
void liqss2::dext(Event x, double t) {
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
		//linear model estimation
	if(e>0||t==0){
		band=false;
		band3=false;
	}
	if(band&&fabs(q_old+dq_old-q[0]-dq)>1e-12){ //self feedback
			aold=a;
			a=(dx_old-derx[0])/(q_old+dq_old-q[0]-dq);
	      if(a<-1e30||a>0){
			//printLog("With dx_old=%g, derx[0]=%g, q_old=%g, dq_old=%g, q[0]=%g and dq=%g we computed a=%g\n",dx_old,derx[0],q_old,dq_old,q[0],dq,a);

			a=0;
		}

	} else {
		   advance_time(q,e,1);
	}; 
	//printLog("en t=%g estimamos a=%g\n",t,a);
	u[0]=derx[0]-a*(q[0]+dq);
	u[1]=derx[1]-a*q[1];

	X[0]=X[0]+X[1]*e+X[2]*e*e;
	X[1]=derx[0];
	X[2]=derx[1]/2;
	if (band4) {
		sigma=0;
	  //	printLog("Ext: t=%g: sigma=%g, X[0]=%g, X[1]=%g, X[2]=%g, q[0]=%g, q[1]=%g,dQ=%g. We found a problem \n",t,sigma,X[0],X[1],X[2],q[0],q[1],dQ);
	}
	//printLog("Ext: t=%g: e=%g, X=[%g, %g, %g], q= [%g, %g], u=[%g ,%g], a=%g \n",t,e,X[0],X[1],X[2],q[0],q[1],u[0],u[1],a);

	if (sigma>0){
	   	diffxq[1]=q[1]-X[1];
		diffxq[2]=-X[2];
	   	diffxq[0]=q[0]-X[0]-dQ;
	   	sigma=minposroot(diffxq,2);
	   	diffxq[0]=q[0]-X[0]+dQ;
	   	dt1=minposroot(diffxq,2);
	   	if (dt1<sigma) sigma=dt1;
			if(dt1!=sigma){diffxq[0]=q[0]-X[0]-dQ;}

		
			if (a!=0&&(fabs(X[2])>1e-10)&&!band3&&!band2){
				double diff1[10];
				diff1[0]=a*a*(q[0]+dq)+a*u[0]+u[1];
				diff1[1]=a*a*q[1]+a*u[1];
				dt1=minposroot(diff1,1);
				if (dt1<sigma) {
					//printLog("t=%g: Changing sigma from %g to %g\n",t,sigma,dt1);
					band2=true;
					sigma=dt1;
				}
			} else {
				band2=false;
			}
			if (sigma>getFinalTime())sigma=getFinalTime();
			advance_time(diffxq,sigma/2,2);
			if (fabs(diffxq[0])>3*(dQmin+dQrel*fabs(q[0]))){
			  //printLog("t=%g: sigma=%g, X[0]=%g, X[1]=%g, X[2]=%g, q[0]=%g, q[1]=%g,dQ=%g. We found a problem \n",t,sigma,X[0],X[1],X[2],q[0],q[1],dQ);
			 sigma=1e-12;
			}
	   	//if (fabs(X[0]-q[0])>3*dQ) sigma=1e-12;

	}
} else {
	advance_time(X,e,2);
	X[0]=derx[0];
	sigma=0;
	band2=false;
}
}
Event liqss2::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)

double ddx_est;
band3=false;
advance_time(q,sigma,1);
q_old=q[0];
dq_old=dq;
advance_time(u,sigma,1);
advance_time(X,sigma,2);
dx_old=X[1];
//printLog("Lambda: t=%g: X=[%g, %g, %g], q= [%g, %g], u=[%g ,%g], a=%g \n",t,X[0],X[1],X[2],q[0],q[1],u[0],u[1],a);
if (!band2){
	q[0]=X[0];
	dQ=dQrel*fabs(X[0]);
	if (dQ<dQmin)dQ=dQmin;
} else {
  //printLog("Expr=%g\n",a*(q[0]+dq)+u[0]+u[1]/a);	
	band2=false;
}

	if (a<-1e-30){
	if (X[2]<0) {        
	  //if (a*X[0]+u[0]+u[1]/a>0) {
		//printLog("t=%g: Checking up \n",t);
		//we try the upper value
		ddx_est=a*a*(q[0]+dQ)+a*u[0]+u[1];
		if (ddx_est<=0) {
			//printLog("OK, going up \n");
			dq=dQ; //OK
		} else {
			dq=-u[1]/a/a-u[0]/a-q[0];
			band3=true;
			//printLog("Setting dq=%g\n",dq);
			if(fabs(dq)>dQ){
					//printLog("t=%g: Cannot set dq= %g\n",t,dq);
					dq=dQ;
			}
	   };
		} else {
		//we try the lower value
		//printLog("t=%g: Checking down \n",t);
		ddx_est=a*a*(q[0]-dQ)+a*u[0]+u[1];
		
		if (ddx_est>=0) {
			dq=-dQ; //OK
			//printLog("OK, going down \n");
		} else {
			band3=true;
			dq=-u[1]/a/a-u[0]/a-q[0];
			//printLog("Setting dq=%g\n",dq);
			if(fabs(dq)>dQ){
					//printLog("t=%g: Cannot set dq= %g\n",t,dq);
					dq=-dQ;
			}
		};
	   }; 
	  //if (band4) printLog("band4=true\n");
    if (q[1]*X[1]<0&&!band4&&!band2&&!band3) {
  		if (q[1]<0) {
			dq=q_old-q[0]+dq_old-fabs(dq_old)*0.1;
  		} else {
			dq=q_old-q[0]+dq_old+fabs(dq_old)*0.1;
  		}
  		band4=true; //do it only once
		 // printLog("Out: t=%g: sigma=%g, X[0]=%g, X[1]=%g, X[2]=%g, q[0]=%g, q[1]=%g,dQ=%g. We found a problem \n",t,sigma,X[0],X[1],X[2],q[0],q[1],dQ);

	  } else if (band4) {
  		band4=false;
  		if (fabs(-u[1]/a/a-u[0]/a-q[0])<3*dQ){ 
			dq=-u[1]/a/a-u[0]/a-q[0];
			band3=true;
			//       printLog("We try provoke X[2]=0\n");

			} 
	  } else {
			band4=false;
		}
  
	} else {
         	//not enough self feedback
		band4=false;
		if (X[2]<0) {dq=-dQ;} else {dq=dQ;}
	}

if (!band3) {
	q[1]=X[1];
} else {
  q[1]=a*(q[0]+dq)+u[0];	
}
y[0]=q[0]+dq;  
y[1]=q[1];
//printLog("Lambda: dq=%g: X=[%g, %g, %g], q= [%g, %g], u=[%g ,%g], a=%g \n",dq,X[0],X[1],X[2],q[0],q[1],u[0],u[1],a);
band=true;

return Event(&y,0);
}
void liqss2::exit() {

}
