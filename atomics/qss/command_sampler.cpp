#include "command_sampler.h"
void command_sampler::init(double t,...) {
va_list parameters;
va_start(parameters,t);

for(int i=0;i<10;i++){
            u[i]=0;
            y[i]=0;
}
sigma=INF;
order=1;
}
double command_sampler::ta(double t) {
return sigma;
}
void command_sampler::dint(double t) {
sigma=INF;
}
void command_sampler::dext(Event x, double t) {
double *xv; 
xv=(double*)(x.value);

if (x.port==0) {
	switch(order){
	case 1: 
		u[0]=xv[0];
		if (xv[1]!=0){order=2;u[1]=xv[1];}
		if (xv[2]!=0){order=3;u[2]=xv[2];} 
		if (xv[3]!=0){order=4;u[3]=xv[3];} 
		if (xv[4]!=0){order=5;u[4]=xv[4];} 
	break;

	case 2: 
		u[0]=xv[0];
		u[1]=xv[1];
		if (xv[2]!=0){order=3;u[2]=xv[2];} 
		if (xv[3]!=0){order=4;u[3]=xv[3];} 
		if (xv[4]!=0){order=5;u[4]=xv[4];} 
	break;

	case 3:
		u[0]=xv[0];
		u[1]=xv[1];
		u[2]=xv[2];
		if (xv[3]!=0){order=4;u[3]=xv[3];} 
		if (xv[4]!=0){order=5;u[4]=xv[4];} 
	break;

	case 4:
		u[0]=xv[0];
		u[1]=xv[1];
		u[2]=xv[2];
		u[3]=xv[3];
		if (xv[4]!=0){order=5;u[4]=xv[4];} 
	break;

	case 5:
		u[0]=xv[0];
		u[1]=xv[1];
		u[2]=xv[2];
		u[3]=xv[3];
		u[4]=xv[4];
	}
	sigma=sigma-e;
} else {
	advance_time(u,e,order-1);
	sigma=0;
};	
}
Event command_sampler::lambda(double t) {
y[0]=u[0];
return Event(y,0);
}
void command_sampler::exit() {

}
