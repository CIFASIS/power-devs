#include "hysteresis_sci.h"
void hysteresis_sci::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
char *fvar= va_arg(parameters,char*);
xl= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
xu= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
yl= getScilabVar(fvar );
fvar= va_arg(parameters,char*);
yu= getScilabVar(fvar );
inf=1e20;
sigma=0;
level=1;
y[1]=0;
y[2]=0;
}
double hysteresis_sci::ta(double t) {
//This function returns a double.
return sigma;
}
void hysteresis_sci::dint(double t) {
level=1-level;
u=u+mu*sigma+pu*sigma*sigma;
mu=mu+2*pu*sigma;
if (((u>xu)&&(level==0))||((u<xl)&&(level==1))) {
  sigma=0;
} else {
  double a,b,c,s1,s2;
  a=pu; 
  b=mu;
  if (level==0) {
    c=u-xu;
  } else {
    c=u-xl;
  };
  if (a==0) {
     if (b==0){
       s1=inf;
       s2=inf;
     } else {
       s1=-c/b;
       s2=inf;
     };
  } else {
    s1=(-b+sqrt(b*b-4*a*c))/2/a;
    s2=(-b-sqrt(b*b-4*a*c))/2/a;
  };
   if ((s1>0)&&((s1<s2)||(s2<0))) {
    sigma=s1;
  } else {
    if (s2>0) {
       sigma=s2;
    } else {
       sigma=inf;
    };
  };
};
}
void hysteresis_sci::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
double *xv;
xv=(double*)(x.value);
u=xv[0];
mu=xv[1];
pu=xv[2];
if (((u>xu)&&(level==0))||((u<xl)&&(level==1))) {
  sigma=0;
} else {
  double a,b,c,s1,s2;
  a=pu; 
  b=mu;
  if (level==0) {
    c=u-xu;
  } else {
    c=u-xl;
  };
  if (a==0) {
     if (b==0){
       s1=inf;
       s2=inf;
     } else {
       s1=-c/b;
       s2=inf;
     };
  } else {
    s1=(-b+sqrt(b*b-4*a*c))/2/a;
    s2=(-b-sqrt(b*b-4*a*c))/2/a;
  };
  if ((s1>0)&&((s1<s2)||(s2<0))) {
    sigma=s1;
  } else {
    if (s2>0) {
       sigma=s2;
    } else {
       sigma=inf;
    };
  };
};
}
Event hysteresis_sci::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
if (level==0) {
  y[0]=yu;
} else {
  y[0]=yl;
};
return Event(&y[0],0);
}
void hysteresis_sci::exit() {

}
