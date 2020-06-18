#include "ramp_sci.h"
void ramp_sci::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
inf=1e20;
char *fvar=va_arg( parameters, char*);
t0=getScilabVar(fvar );
fvar=va_arg( parameters, char*);
tr=getScilabVar(fvar );
fvar=va_arg( parameters, char*);
u=getScilabVar(fvar );
Method=va_arg(parameters,char*); 
fvar=va_arg( parameters, char*);
dq=getScilabVar(fvar );


for(int i=0;i<10;i++){y[i]=0;};
T[0]=0;
T[1]= t0;
T[2]=t0+tr;
T[3]=inf;
v[0]= 0;
v[1]=0;
v[2]=u;
v[3]=u;
mv[0]=0;
if (tr>0){
  mv[1]=u/tr;
}
mv[2]=0;
sigma=0;
j=0;
}
double ramp_sci::ta(double t) {
//This function return a double.
return sigma;
}
void ramp_sci::dint(double t) {
if( (strcmp(Method,"QSS")==0)&&(j==1) ){
		if( (mv[1]>0)&&((v[j]+dq)<u) ){
			sigma=tr*dq/u;
			v[1]=v[1]+dq;
		}else{
			if( (mv[1]<0)&&((v[j]-dq)>u) ){
				sigma=-tr*dq/u;
				v[1]=v[1]-dq;
			}else{
				j=j+1;
				sigma=-(u-v[j])/(u/tr);
				v[1]=u;
			};
		};
}else{
	sigma=T[j+1]-T[j];
	j=j+1;
if(j==3){sigma=1e10;};
};
}
void ramp_sci::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
}
Event ramp_sci::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
if(strcmp(Method,"QSS")==0){
   y[0]=(int) ((v[j]+dq/2)/dq);
   y[0]=y[0]*dq;
}else{
              y[0]=v[j];
	y[1]=mv[j];
};
return Event(&y,0);
}
void ramp_sci::exit() {

}
