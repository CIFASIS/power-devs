#include "qss_saturation1.h"
void qss_saturation1::init(double t,...) {
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

sigma=INF;
}
double qss_saturation1::ta(double t) {
return sigma;
}
void qss_saturation1::dint(double t) {
//printLog("Switch internal transition \n");
double uvalue;

uvalue=u[0];
switch(sw) {
	case -1:
			u[0]=u[0]-xl;
			sigma=minposroot(u,4);
			if(sigma==0)sigma=INF;
			//uvalue=u[0];
			u[0]=uvalue;
			//printLog("At t=%g we set sigma=%g in A\n",t,sigma);
	break;

	case 0:
			double s1;
			u[0]=u[0]-xu;
			s1=minposroot(u,4);
         if(s1==0)s1=INF;
			u[0]=uvalue;
			u[0]=u[0]-xl;
			sigma=minposroot(u,4);
			if(sigma==0)sigma=INF;
			if(s1<sigma){sigma=s1;}; 
			u[0]=uvalue;
			//printLog("At t=%g we set sigma=%g in B\n",t,sigma);
	break;

	case 1:
			u[0]=u[0]-xu;
			sigma=minposroot(u,4);
			if(sigma==0)sigma=INF;
			//uvalue=u[0];
			u[0]=uvalue;
			//printLog("At t=%g we set sigma=%g in C\n",t,sigma);
}; 
u[0]=uvalue;
//printLog("Interna \n");		 
//printLog("Internal: We set sigma=%g \n",sigma);
}
void qss_saturation1::dext(Event x, double t) {
double *xv;
xv=(double*)(x.value);

//printLog("t=%g : We entered delta_ext \n",t);

u[0]=xv[0];
u[1]=xv[1];
u[2]=xv[2];
u[3]=xv[3];

double uvalue,uvalue1,uvalue2,uvalue3;

uvalue=u[0];
uvalue1=u[1];
uvalue2=u[2];
uvalue3=u[3];

switch(sw) {
	case -1: //state was lower saturated
   	if(u[0]<xl){ //state remains lower saturated 
			u[0]=u[0]-xl;
			sigma=minposroot(u,4);
			if(sigma==0)sigma=INF;
			u[0]=uvalue;
		}else{
				if(u[0]>=xu){ //output state change and became upper saturated
						sigma=0;
				}else{
                 if(u[0]>xl){ //output state change and is in the linear zone
							sigma=0;
						}else{ //u[0]==xl
							if(u[1]>0||(u[1]==0&&u[2]>0)){ //output state change and is in the linear zone
								sigma=0;	
							}else{ //state remains lower saturated
								u[0]=u[0]-xl;
								sigma=minposroot(u,4);
								if(sigma==0)sigma=INF;
								//uvalue=u[0];
                      u[0]=uvalue;
							}
							
						}
				}
		}
	break;

	case 0:{};
     if(u[0]<xu && u[0]>xl){ //state remains in the linear zone
			double s1;
			u[0]=u[0]-xu;
			s1=minposroot(u,4);
			if(s1==0)s1=INF;
       	u[0]=uvalue;         //AGREGADO
			u[0]=u[0]-xl;
			sigma=minposroot(u,4);
			if(sigma==0)sigma=INF;
			if(s1<sigma){sigma=s1;}; 
     		u[0]=uvalue;
         sigma=0;
     }else{
				if(u[0]>=xu){
					if(u[0]>xu){ //output state change and became upper saturated
						sigma=0;
              }else{  //u[0]==xu
						if(u[1]>0||(u[1]==0&&u[2]>0)){//output state change and became upper saturated
							sigma=0;						
						}else{ //state remains in the linear zone
										double s1;
										u[0]=u[0]-xu;
										s1=minposroot(u,4);
										if(s1==0)s1=INF;
										u[0]=uvalue;
										u[0]=u[0]-xl;
										sigma=minposroot(u,4);
										if(sigma==0)sigma=INF;
										if(s1<sigma){sigma=s1;}; 
     									u[0]=uvalue;
						}
					}
				}else{ //u[0]<=xl
					if(u[0]<xl){ //output state change and became lower saturated
						sigma=0;
              }else{//u[0]==xl
						if(u[1]<0||(u[1]==0&&u[2]<0)){//output state change and became lower saturated
							sigma=0;						
						}else{ //state remains in the linear zone
										double s1;
										u[0]=u[0]-xu;
										s1=minposroot(u,4);
										if(s1==0)s1=INF;
										u[0]=uvalue;
										u[0]=u[0]-xl;
										sigma=minposroot(u,4);
										if(sigma==0)sigma=INF;
										if(s1<sigma){sigma=s1;}; 
     									u[0]=uvalue;
						}
					}

				}
	  }

	break;

	case 1:
   	if(u[0]>xu){ //state remains upper saturated
			u[0]=u[0]-xu;
			sigma=minposroot(u,4);
			if(sigma==0)sigma=INF;
			u[0]=uvalue;
		}else{
				if(u[0]<xl){ //output state change and became lower saturated
						sigma=0;
				}else{
                 if(u[0]<xu){ //output state change and is in the linear zone
							sigma=0;
						}else{ //u[0]==xl
							if(u[1]<0||(u[1]==0&&u[2]<0)){ //output state change and is in the linear zone
								sigma=0;	
							}else{ //state remains upper saturated
								u[0]=u[0]-xu;
								sigma=minposroot(u,4);
								if(sigma==0)sigma=INF;
								u[0]=uvalue;	
							}
							
						}
			
				}
		}
}; 
	
u[0]=uvalue;
u[1]=uvalue1;
u[2]=uvalue2;
u[3]=uvalue3;

//printLog("Externa \n");
//printLog("t=%g: We have u[0]=%g , sw=%i and we set sigma=%g \n",t,u[0],sw,sigma);
}
Event qss_saturation1::lambda(double t) {
advance_time(u,sigma+1e-20,-1);//this should be done at internal transition
double band;
band=0;
if(u[0]<xu && u[0]>xl){ //state is in the linear zone
		y[0]=u[0];
		y[1]=u[1];
		y[2]=u[2];
		y[3]=u[3];
		sw=0;
		band=1;
}else{
		if(u[0]>=xu){
			if(u[0]>xu){ //output state change and became upper saturated
					y[0]=xu;
					y[1]=0;
					y[2]=0;
					y[3]=0;
					sw=1;
					band=2;
        }else{  //u[0]==xu
						if(u[1]>0||(u[1]==0&&u[2]>0)){//output state change and became upper saturated
								y[0]=xu;
								y[1]=0;
								y[2]=0;
								y[3]=0;
								sw=1;
								band=3;
						}else{ //state remains in the linear zone
								y[0]=u[0];
								y[1]=u[1];
								y[2]=u[2];
								y[3]=u[3];
								sw=0;	
								band=4;							
						}
			}
		}else{ //u[0]<=xl
				if(u[0]<xl){ //output state change and became lower saturated
						y[0]=xl;
						y[1]=0;
						y[2]=0;
						y[3]=0;
						sw=-1;
						band=5;
           }else{//u[0]==xl
						if(u[1]<0||(u[1]==0&&u[2]<0)){//output state change and became lower saturated
								y[0]=xl;
								y[1]=0;
								y[2]=0;
								y[3]=0;
								sw=-1;
								band=6;
						}else{ //state remains in the linear zone
								y[0]=u[0];
								y[1]=u[1];
								y[2]=u[2];
								y[3]=u[3];
								sw=0;	
								band=7;									
						}
				}
		}
}
//printLog("t=%g: We have u[0]=%g, u[1]=%g, u[2]=%g, u[3]=%g, sw=%i y lo calculo %g \n",t,u[0],u[1],u[2],u[3],sw,band);
//printLog("t=%g: We have y[0]=%g, y[1]=%g, y[2]=%g, y[3]=%g sw=%i y lo calculo %g \n\n",t,y[0],y[1],y[2],y[3]);
return Event(y,0);
}
void qss_saturation1::exit() {

}
