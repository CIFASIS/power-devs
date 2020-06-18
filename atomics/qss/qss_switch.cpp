#include "qss_switch.h"
void qss_switch::init(double t,...) {
va_list parameters;
va_start(parameters,t);

char *fvar= va_arg(parameters,char*); 
level= getScilabVar(fvar );

for(int i=0;i<10;i++){
	u[0][i]=0;
	u[1][i]=0;
	u[2][i]=0;
	y[i]=0;
};  

sigma=INF;
if (level>0){
	sw=2;
} else {
	sw=0;
};
change=0;
tcross=INF;
}
double qss_switch::ta(double t) {
return sigma;
}
void qss_switch::dint(double t) {
//printLog("Switch internal transition \n");
if (change==0) { 
	sigma=tcross;
} else {
	if (sigma==0) {
		//We arrived here because we skipped a switch
		sigma=tcross;
	} else {
		u[1][0]=0;
		advance_time(u[1],1e-20,-1);
        
		sigma=minposroot(u[1],4);
		tcross=sigma;
	};
	sw=2-sw;
}; 
change=1; //we plan to change
}
void qss_switch::dext(Event x, double t) {
double *xv;
xv=(double*)(x.value);

//printLog("t=%g : We entered delta_ext \n",t);

for (int i=0;i<3;i++) {
	if (i!=x.port) {
		advance_time(u[i],e,-1);
	};
}
u[x.port][0]=xv[0];
u[x.port][1]=xv[1];
u[x.port][2]=xv[2];
u[x.port][3]=xv[3];

if (x.port==1){
	u[1][0]=u[1][0]-level;
	if (u[1][0]==0){advance_time(u[1],1e-20,-1);} //We cross the discontinuity
	tcross=minposroot(u[1],4);
	//printLog("We computed tcross=%g from u[1]=[%g , %g , %g , %g, %g]\n",tcross,u[1][0],u[1][1],u[1][2],u[1][3],u[1][4]);
	if (((u[1][0]>0)&&(sw==2))||((u[1][0]<=0)&&(sw==0))){ //we migth be in the wrong position
		//printLog("t= %g : We are wrong, we have u[1]=[%g , %g , %g , %g, %g] and sigma= %g\n",t,u[1][0],u[1][1],u[1][2],u[1][3],u[1][4],sigma);
		sigma=0;
	} else {
		sigma=tcross;
	};
	change=1; //we plan to switch in the next event at tcross (or 0)
} else {
	tcross=tcross-e; 
	if ((sw==x.port)&&(e<sigma)) {
		sigma=0;
		change=0; //we do not change
	} else {
		if (e<sigma) {
			sigma=tcross;
		} else {
			sigma=0;
		}
		change=1; //we plan to switch in tcross (or 0)
	};
};

//printLog("We received event by port %i and computed sigma= %g \n",x.port,sigma);
//printLog("We have sw= %i and change= %i \n",sw,change);
}
Event qss_switch::lambda(double t) {
if (sigma>0){
for (int i=0;i<3;i++) {
			advance_time(u[i],sigma,-1);
		};
}
if (change==0) {
   if (((u[1][0]>1e-10)&&(sw==2))||((u[1][0]<-1e-10)&&(sw==0)))sw=2-sw;
	y[0]=u[sw][0];
	y[1]=u[sw][1];
	y[2]=u[sw][2];
	y[3]=u[sw][3];
} else {
   if (((u[1][0]>1e-10)&&(sw==0))||((u[1][0]<-1e-10)&&(sw==2)))sw=2-sw;
	y[0]=u[2-sw][0];
	y[1]=u[2-sw][1];
	y[2]=u[2-sw][2];
	y[3]=u[2-sw][3];
};

if (sigma>0)advance_time(y,sigma,-1);
 
return Event(y,0);
}
void qss_switch::exit() {

}
