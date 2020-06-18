#include "hysteretic.h"
void hysteretic::init(double t,...) {
va_list parameters;
va_start(parameters,t);

char *fvar= va_arg(parameters,char*); 
ud[0]= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
ud[1]= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
yd[0]= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
yd[1]= getScilabVar(fvar );

for(int i=0;i<10;i++){
	u[i]=0;
	y[i]=0;
};  

if (ud[0]<0){
	sw=0;
 } else {
	sw=1;
};
sigma=0;

}
double hysteretic::ta(double t) {
return sigma;
}
void hysteretic::dint(double t) {
//printLog("Switch internal transition \n");
if (sigma>0){
	advance_time(u,sigma,-1);
//	u[0]=0;
//	advance_time(u,1e-14,-1);
};
u[0]=u[0]+ud[1-sw]+ud[1-sw]; //We change the detection level
sigma=minposroot(u,4);
sw=1-sw;

//printLog("Internal: We set sigma=%g \n",sigma);
}
void hysteretic::dext(Event x, double t) {
double *xv;
xv=(double*)(x.value);

//printLog("t=%g : We entered delta_ext \n",t);

u[0]=xv[0]-ud[1-sw];
u[1]=xv[1];
u[2]=xv[2];
u[3]=xv[3];
	
if (u[0]==0){advance_time(u,1e-14,-1);} //We cross the discontinuity

if (((u[0]>0)&&(sw==0))||((u[0]<=0)&&(sw==1))){ //we migth be in the wrong position
			sigma=0;
} else {
		if ((sigma>0)||(t>0)){
			sigma=minposroot(u,4);
		}
};
//printLog("t=%g: We have u[0]=%g , sw=%i and we set sigma=%g \n",t,u[0],sw,sigma);
}
Event hysteretic::lambda(double t) {
y[0]=yd[1-sw]; 
return Event(y,0);
}
void hysteretic::exit() {

}
