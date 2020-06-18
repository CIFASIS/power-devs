#include "compare.h"
void compare::init(double t,...) {
va_list parameters;
va_start(parameters,t);

char *fvar= va_arg(parameters,char*); 
low= getScilabVar(fvar );

fvar= va_arg(parameters,char*); 
up= getScilabVar(fvar );

for(int i=0;i<10;i++){
	u[0][i]=0;
	u[1][i]=0;
	y[i]=0;
	du[i]=0;
};  
sw=1;

sigma=0;
}
double compare::ta(double t) {
return sigma;
}
void compare::dint(double t) {
//printLog("Switch internal transition \n");
advance_time(u[0],sigma,-1);
advance_time(u[1],sigma,-1);
for (int i=0;i<4;i++) {
  du[i]=u[0][i]-u[1][i];		
};
if (sigma>0||du[0]==0){
	du[0]=0;
	advance_time(du,1e-20,-1);
};
	
sigma=minposroot(du,4);
sw=-sw;
}
void compare::dext(Event x, double t) {
double *xv;
xv=(double*)(x.value);

//printLog("t=%g : We entered delta_ext \n",t);

u[x.port][0]=xv[0];
u[x.port][1]=xv[1];
u[x.port][2]=xv[2];
u[x.port][3]=xv[3];

advance_time(u[1-x.port],e,-1);

for (int i=0;i<4;i++) {
  du[i]=u[0][i]-u[1][i];		
};

if (du[0]==0){advance_time(du,1e-20,-1);} //We cross the discontinuity

if (((du[0]>0)&&(sw==-1))||((du[0]<=0)&&(sw==1))){ //we migth be in the wrong position
		sigma=0;
} else {
		if ((sigma>0)||(t>0))sigma=minposroot(du,4);
};
}
Event compare::lambda(double t) {
if (sw==1) {
	y[0]=low;
} else {
	y[0]=up;
};
 
return Event(y,0);
}
void compare::exit() {

}
