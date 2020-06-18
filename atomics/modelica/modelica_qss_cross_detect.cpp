#include "modelica_qss_cross_detect.h"
void modelica_qss_cross_detect::init(double t,...) {
va_list parameters;
va_start(parameters,t);

//index=static_cast<int>(va_arg(parameters,double));
index=(int)va_arg(parameters,double);
level= 0;
outval= 1;
from_discont=false;

for(int i=0;i<10;i++){u[i]=0;};  
for(int i=0;i<10;i++){y[i]=0;};  

sigma=INF;
sw=0;
va_end(parameters);
}
double modelica_qss_cross_detect::ta(double t) {
return sigma;
}
void modelica_qss_cross_detect::dint(double t) {

//printLog("t=%g: DINT We schedule the next event for  %g secs. u=(%g %g %g) sw=%d\n",t,t+sigma,u[0],u[1],u[2],sw);
sw=-sw;
if (!from_discont)
  u[0]=1e-24*sw;//to correct numerical errors
from_discont=false;
sigma=minposroot(u,4);
//printLog("[t=%g] Crossing %d found (now is %d)\n",t,index,condition(index));
//sigma=INF;
}
void modelica_qss_cross_detect::dext(Event x, double t) {
double *xv;
xv=(double*)(x.value);

double old_u0=evaluate_poly(u,e,4);
u[0]=xv[0]-level;
u[1]=xv[1];
u[2]=xv[2];
u[3]=xv[3];
u[4]=xv[4];

if (old_u0*u[0]<0)
{
  //printLog("[t=%g] Old u=%g newu=%g\n",t,old_u0,u[0]);//sigma=0;
  from_discont=true;
  sigma=0;
  return;
}


if (((sw==1)&&(u[0]<=0))||((sw==-1)&&(u[0]>=0))) {
	if (u[0]*u[1]>=0){
		sigma=0;
		//printLog("t=%g: We almost skip an event... doing it now. sw=%d\n",t, sw);
	} else {
//		sigma=INF;
		sigma=minposroot(u,4);
		sw=(u[0]>0 ? 1 : -1);
		//printLog("t=%g: Short duration event skipped. We have u[0]=%g, u[1]=%g, sw=%d sigma=%g\n",t,u[0],u[1],sw,minposroot(u,4));
	};
} else {
  sigma=minposroot(u,4);
  //printLog("t=%g: We schedule the next event for  %g secs. u=(%g %g %g) sw=%d\n",t,t+sigma,u[0],u[1],u[2],sw);
  if (sw==0) {
	if (u[0]>0) {
			sw=1;
		} else {
			sw=-1;
		};
	};
		bool old=condition(index);
		if (u[0]>0) 
			set_condition_to(index,false);
		else
			set_condition_to(index,true);
		//if (condition(index)!=old)
		//	printLog("DEXT    [t=%g] Cond %d = %d\n",t,index,condition(index));
  function_updateDepend(t,0);
};
if (sigma<1e-12)sigma=1e-12;
}
Event modelica_qss_cross_detect::lambda(double t) {
y[0]=outval;
advance_time(u,sigma,4);
bool old=condition(index);
if (!from_discont) {
  if (u[1]>0) 
	  set_condition_to(index,false);
  else if (u[1]<0)
	  set_condition_to(index,true);
  else if (u[2]>0)
	  set_condition_to(index,false);
  else
	  set_condition_to(index,true);
} else {
  if (u[0]>0) 
    set_condition_to(index,false);
  else
	  set_condition_to(index,true);
}
//if (condition(index-1)!=old)
//	printLog("[t=%g] Changing Cond %d = %d\n",t,index-1,condition(index-1));
//printLog("Event detected at t=%g %g %g %g cond(%d)=%d\n",t,u[0],u[1],u[2],index-1,condition(index-1));
function_updateDepend(t,0);
return Event(y,0);
}
void modelica_qss_cross_detect::exit() {

}
