#include "qss_saturation.h"
void qss_saturation::init(double t,...) {
va_list parameters;
va_start(parameters,t);

char *fvar= va_arg(parameters,char*); 
xl= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
xu= getScilabVar(fvar );

for(int i=0;i<10;i++){
	u[i]=0;
	y[i]=0;
};  

if (xl>0){
	sw=-1;
 } else {
	if (xu<0) {
		sw=1;
	} else {
		sw=0;
	};
};
sigma=0;
}
double qss_saturation::ta(double t) {
return sigma;
}
void qss_saturation::dint(double t) {
//printLog("Switch internal transition \n");
if (sigma>0) {
	//change in saturation state
	if (sw==0) {
		//we enter saturation
		if (u[0]>0.5*(xl+xu)){
			//we are in up saturation
			sw=1;
		} else {
			//we are in low saturation
			sw=-1;
		};			
	} else {
		//we enter linear zone
		sw=0;	
	};
} else {
	if (u[0]<=xl) {
		sw=-1;	
   } else if (u[0]>=xu) {
		sw=1;
	} else {
		sw=0;
	};
};

switch(sw) {
	case -1:
		if (u[0]==xl)advance_time(u,1e-20,-1); //cross the discontinuity
		if (u[0]>xl) {
		 //we are outside low saturation
			sigma=0;
		} else {
			u[0]=u[0]-xl;
			sigma=minposroot(u,4);
			u[0]=u[0]+xl;
		}
	break;

	case 0:
		if (u[0]==xl)advance_time(u,1e-20,-1); //cross the discontinuity
		if (u[0]==xu)advance_time(u,1e-20,-1); //cross the discontinuity
		if ((u[0]<=xl) ||(u[0]>=xu)){
		 //we are outside linear zone
			sigma=0;
		} else {
			u[0]=u[0]-xl;
			double sigma1=minposroot(u,4);
			u[0]=u[0]+xl-xu;
			sigma=minposroot(u,4);
			u[0]=u[0]+xu;
			if (sigma1<sigma)sigma=sigma1;
		}
		break;

	case 1:
		if (u[0]==xu)advance_time(u,1e-20,-1); //cross the discontinuity
		if (u[0]<xu) {
		 //we are outside up saturation
			sigma=0;
		} else {
			u[0]=u[0]-xu;
			sigma=minposroot(u,4);
			u[0]=u[0]+xu;
		}

}; 

		 
//printLog("Internal: We set sigma=%g \n",sigma);
}
void qss_saturation::dext(Event x, double t) {
double *xv;
xv=(double*)(x.value);

//printLog("t=%g : We entered delta_ext \n",t);

u[0]=xv[0];
u[1]=xv[1];
u[2]=xv[2];
u[3]=xv[3];

switch(sw) {
	case -1:
		if (u[0]==xl)advance_time(u,1e-20,-1); //cross the discontinuity
		if (u[0]>xl) {
		 //we are outside low saturation
			sigma=0;
		} else {
			u[0]=u[0]-xl;
			sigma=minposroot(u,4);
			u[0]=u[0]+xl;
		}
	break;

	case 0:

		sigma=0;
	break;

	case 1:
		if (u[0]==xu)advance_time(u,1e-20,-1); //cross the discontinuity
		if (u[0]<xu) {
		 //we are outside up saturation
			sigma=0;
		} else {
			u[0]=u[0]-xu;
			sigma=minposroot(u,4);
			u[0]=u[0]+xu;
		}

}; 
	


//printLog("t=%g: We have u[0]=%g , sw=%i and we set sigma=%g \n",t,u[0],sw,sigma);
}
Event qss_saturation::lambda(double t) {
if (sigma>0) {
	//change in saturation state
	advance_time(u,sigma+1e-20,-1);//this should be done at internal transition
	if (sw==0) {
		//we enter saturation
		if (u[0]>0.5*(xl+xu)){
			//we are in up saturation
			y[0]=xu;
			y[1]=0;	
			y[2]=0;	
			y[3]=0;	
		} else {
			//we are in low saturation
			y[0]=xl;
			y[1]=0;	
			y[2]=0;	
			y[3]=0;	
		};			
	} else {
		//we enter linear zone
		y[0]=u[0];
		y[1]=u[1];
		y[2]=u[2];
		y[3]=u[3];
	};
} else {
	if (u[0]<=xl) {
		y[0]=xl;
		y[1]=0;	
		y[2]=0;	
		y[3]=0;	
   } else if (u[0]>=xu) {
		y[0]=xu;
		y[1]=0;	
		y[2]=0;	
		y[3]=0;	
	} else {
		y[0]=u[0];
		y[1]=u[1];	
		y[2]=u[2];	
		y[3]=u[3];	
	};
};		 
return Event(y,0);
}
void qss_saturation::exit() {

}
