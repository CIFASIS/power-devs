#include "hhysteresis.h"
void hhysteresis::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
xl=va_arg(parameters,double);
xu=va_arg(parameters,double);
yl=va_arg(parameters,double);
yu=va_arg(parameters,double);
inf=1e10;
sigma=0;
level=1;
y[1]=0;
y[2]=0;
}
double hhysteresis::ta(double t) {
//This function returns a double.
return sigma;
}
void hhysteresis::dint(double t) {
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
void hhysteresis::dext(Event x, double t) {
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
Event hhysteresis::lambda(double t) {
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
void hhysteresis::exit() {

}
