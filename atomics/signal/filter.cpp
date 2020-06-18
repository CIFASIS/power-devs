#include "filter.h"
void filter::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type
for (int i=1;i<10;i++){
	y[i]=0;
	u[i]=0;
};
sigma=INF;
c=0;
}
double filter::ta(double t) {
//This function returns a double.
return sigma;
}
void filter::dint(double t) {
sigma=INF;
}
void filter::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition

if ((fabs(x.getDouble(0)-u[0])<1e-10)&&(fabs(x.getDouble(1)-u[1])<1e-10)) {
		sigma=INF;
		c=0;
} else {
		if (e==0) {
			printLog("[t=%g, model %d]: We received u0=%g,u1=%g\n",t,this,x.getDouble(0),x.getDouble(1));
			c++;
		} else {
			c=0;
		}
		
		u[0]=x.getDouble(0);
		u[1]=x.getDouble(1);
		if (c>3) {
			sigma=INF;
		} else {
			sigma=0;
		};
		
} 
}
Event filter::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
y[0]=u[0];
y[1]=u[1];

return Event(&y,0);
}
void filter::exit() {
//Code executed at the end of the simulation.

}
