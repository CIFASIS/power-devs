#include "qss_quantizer.h"
void qss_quantizer::init(double t,...) {
va_list parameters;
va_start(parameters,t);

char *fvar= va_arg(parameters,char*); 
dQ= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
eps=dQ*1e-10;
if (strcmp(fvar,"Yes")==0)eps=dQ;

for(int i=0;i<10;i++){
	u[i]=0;
	y[i]=0;
};  
q=0;

sigma=0;
}
double qss_quantizer::ta(double t) {
return sigma;
}
void qss_quantizer::dint(double t) {
//printLog("Switch internal transition \n");
if (sigma>0){
	if (u[0]>q+0.5*dQ){
		q=q+dQ;
	} else {
		q=q-dQ;
	};	
	
} else {
	q=dQ*floor(u[0]/dQ);
};
if ((u[0]>=q+dQ)||(u[0]<q-eps)) {
  //we are at the wrong level
	//printLog("Internal: We have q=%g and u=%g \n",q,u[0]);
  sigma=0;
} else {
  //we compute next crossing time
	u[0]=u[0]-q-dQ;
	double sigma1=minposroot(u,4);
	u[0]=u[0]+dQ+eps;
	sigma=minposroot(u,4);
	u[0]=u[0]+q-eps;
	if (sigma1<sigma)sigma=sigma1;
};


//printLog("Internal: We set sigma=%g \n",sigma);
}
void qss_quantizer::dext(Event x, double t) {
double *xv;
xv=(double*)(x.value);

//printLog("t=%g : We entered delta_ext \n",t);

u[0]=xv[0];
u[1]=xv[1];
u[2]=xv[2];
u[3]=xv[3];
	
if ((u[0]>=q+dQ)||(u[0]<q-eps)) {
  //we are at the wrong level
  sigma=0;
} else {
  //we compute next crossing time
	u[0]=u[0]-q-dQ;
	double sigma1=minposroot(u,4);
	u[0]=u[0]+dQ+eps;
	sigma=minposroot(u,4);
	u[0]=u[0]+q-eps;
	if (sigma1<sigma)sigma=sigma1;
};
//printLog("t=%g: We have u[0]=%g , sw=%i and we set sigma=%g \n",t,u[0],sw,sigma);
}
Event qss_quantizer::lambda(double t) {
if (sigma>0){
	advance_time(u,sigma+1e-20,-1);
	if (u[0]>q+0.5*dQ){
		y[0]=q+dQ;
	} else {
		y[0]=q-dQ;
	};	
} else {
	y[0]=dQ*floor(u[0]/dQ);
};
return Event(y,0);
}
void qss_quantizer::exit() {

}
