#include "rintegrator.h"
void rintegrator::init(double t, ...){
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
Method=va_arg(parameters,char*);
dQ=va_arg(parameters,double);
X=va_arg(parameters,double);
dQmin=dQ;
inf=1e20;
pidiv3=1.047197551;
u=0;
mu=0;
pu=0;
if (strcmp(Method,"QSS")==0) {
    q=floor(X/dQ)*dQ; 
};
if(strcmp(Method,"QSS2")==0) {
        q=X;
};
if (strcmp(Method,"QSS3")==0){
        q=X;
};
if((strcmp(Method,"BQSS")==0)||(strcmp(Method,"CQSS")==0)||(strcmp(Method,"TQSSG")==0)){
        ep=dQ/100;
        //ep=0;
        qs=floor(X/dQ)*dQ+dQ; 
        qi=floor(X/dQ-ep)*dQ; 
        q=qi;
        eps=1e-20;
        band=1;
};
mq=0;
pq=0;
sigma=0;
for (int i=0;i<10;i++) {
    y[i]=0;
};
}

double rintegrator::ta(double t){
//This function return a double.
return sigma;
}

void rintegrator::dint(double t){
if (strcmp(Method,"QSS")==0) {
  X=X+sigma*u;
  q=X;
  if (u==0) {
   sigma=inf;
  } 
  else {
    sigma=dQ/fabs(u);
  };
};
if (strcmp(Method,"QSS2")==0) {
  X=X+u*sigma+mu/2*sigma*sigma;
  q=X;
  u=u+mu*sigma; 
  mq=u;
  if (mu==0){
    sigma=inf;
  }
  else {
    sigma=sqrt(2*dQ/fabs(mu));
  }; 
};
if(strcmp(Method,"QSS3")==0){
    X=X+u*sigma+(mu*pow(sigma,2))/2 + (pu*pow(sigma,3))/3;
    q=X;
    u=u + mu * sigma + pu * pow(sigma,2);
    mq=u;
    mu=mu+2*pu*sigma;
    pq=mu/2;
    //pu=2*pu;
    if (pu==0){
        sigma=inf;
    }
    else{
       sigma=pow(fabs(3*dQ/pu), 1.0/3);
    };
};
if((strcmp(Method,"BQSS")==0)||(strcmp(Method,"CQSS")==0)||(strcmp(Method,"TQSSG")==0)){
    X=X+sigma*u;
   // dQ=X/100;      //Agregado borrar
   // if(dQ<dQmin){dQ=dQmin;};  //Agregado borrar
    if (sigma>eps) { // X arrives to a new level (q)
        if(u>0) {
           qs=qs+dQ;
           q=qs;
           qi=q-2*dQ;
        }else{
           qi=qi-dQ;
           q=qi;
           qs=q+2*dQ;
        }; 
    } else { // slope change
        if(u>0) {
           q=qs;
        }else{
           q=qi;
        };
    };
//    if(fabs(q/dQ)<1){q=0;};  //Corrige error numérico en torno a cero
    if (u!=0) {
        sigma=(q-X)/u+2*eps;
    } else {
        sigma=inf;
    };
    band=0;
};
}

void rintegrator::dext(Event x, double t){
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double *Aux;
Aux=(double*)x.value;
if (x.port==0) {
if(strcmp(Method,"QSS")==0){
  X=X+e*u;
  u=Aux[0];  
  if (sigma!=0){
    if (u==0) {
      sigma=inf;
    }
    else {
      if (u>0) {
        sigma=(q+dQ-X)/u;
      }
      else {
         sigma=(q-dQ-X)/u;
      }
    };      
  };
};
if (strcmp(Method,"QSS2")==0) {
  double a,b,c,s;
  X=X+u*e+mu/2*e*e;
  u=Aux[0];  //input value
  mu=Aux[1];  //input slope
  if (sigma!=0){
    q=q+mq*e;
    a=mu/2;
    b=u-mq;
    c=X-q+dQ;
    sigma=inf;
    if (a==0) {
      if (b!=0) {
         s=-c/b;
         if (s>0) {sigma=s;};
         c=X-q-dQ;
         s=-c/b;
         if ((s>0)&&(s<sigma)) {sigma=s;};
      };
    }
    else {
        s=(-b+sqrt(b*b-4*a*c))/2/a;
        if (s>0) {sigma=s;};
        s=(-b-sqrt(b*b-4*a*c))/2/a;
        if ((s>0)&&(s<sigma)) {sigma=s;};
        c=X-q-dQ;
        s=(-b+sqrt(b*b-4*a*c))/2/a;
        if ((s>0)&&(s<sigma)) {sigma=s;};
        s=(-b-sqrt(b*b-4*a*c))/2/a;
        if ((s>0)&&(s<sigma)) {sigma=s;};
    };
    if (((X-q)>dQ) || ((q-X)>dQ)){sigma=0;};
  }; 
};
if(strcmp(Method,"QSS3")==0){
    double a,b,c,s;
    X=X +u*e+(mu*e*e)/2+(pu*e*e*e)/3;
    u=Aux[0];    //input value
    mu=Aux[1]; //input slope
    pu=Aux[2]; //input derivative
    if(sigma!=0)
    {
        double a,b,c,v,w,A,B, i1,i2, s;
        q=q+mq*e+pq*e*e;
        mq=mq+2*pq*e;
        a=mu/2-pq;
        b=u-mq;
        c=X-q-dQ;
        if(pu!=0)
        {
            a=3*a/pu;
            b=3*b/pu;
            c=3*c/pu;
            v=b-a*a/3;
            w=c-b*a/3+2*a*a*a/27;
            i1=-w/2;
            i2=i1*i1+v*v*v/27;
            if(i2>0)
            {
                i2=sqrt(i2);
                A=i1+i2;
                B=i1-i2;
                if(A>0) A=pow(A,1.0/3);
                else A=-pow(fabs(A),1.0/3);
                if(B>0) B=pow(B,1.0/3);
                else B=-pow(fabs(B),1.0/3);
                s=A+B-a/3;
                //esta raiz es la unica real pero puede ser negativa
                if(s<0){ s=inf;}
             }
             else if (i2==0)
             {
                 double x1, x2;
                 A=i1;
                 if(A>0) A=pow(A, 1.0/3);
                 else A=-pow(fabs(A),1.0/3);
                 x1=2*A-a/3;
                 x2=-(A+a/3);
                 if (x1<0)
                 {
                       if(x2<0){s=inf;}
                       else {s=x2;}
                 }    
                 else if(x2<0){ s=x1;} 
                     else if (x1<x2) {s=x1;}
                           else {s=x2;}    
             }
             else 
             {
                 double y1, y2, y3, arg;
                 arg=w*sqrt(27/(-v))/(2*v);
                 arg=acos(arg)/3;
                 y1=2*sqrt(-v/3);                    
                 y2=-y1*cos(pidiv3-arg)-a/3;
                 y3=-y1*cos(pidiv3+arg)-a/3;
                 y1=y1*cos(arg)-a/3;
                 if( y1<0){ s=inf;}
                 else if (y3<0){ s=y1;}
                        else if(y2<0) {s=y3;}
                               else { s=y2;}
             }
             c=c+6*dQ/pu;
             w=c-b*a/3+2*a*a*a/27;
             i1=-w/2;
             i2=i1*i1+v*v*v/27;
             if(i2>0)
             {
                  i2=sqrt(i2);
                  A=i1+i2;
                  B=i1-i2;
                  if(A>0) A=pow(A,1.0/3);
                  else A=-pow(fabs(A),1.0/3);
                  if(B>0) B=pow(B,1.0/3);
                  else B=-pow(fabs(B),1.0/3);
                  sigma=A+B-a/3;
                  //esta raiz es la unica real pero puede ser negativa
                  if(s<sigma || sigma<0){ sigma=s;}
              }
              else if (i2==0)
              {
                   double x1, x2;
                   A=i1;
                   if(A>0) A=pow(A, 1.0/3);
                   else A=-pow(fabs(A),1.0/3);
                   x1=2*A-a/3;
                   x2=-(A+a/3);
                   if (x1<0)
                   {
                        if(x2<0){sigma=inf;}
                        else {sigma=x2;}
                   }    
                   else if(x2<0){ sigma=x1;} 
                          else if (x1<x2) {sigma=x1;}
                                 else {sigma=x2;}    
                   if(s<sigma){ sigma=s;}                   
             }
             else 
             {
                   double y1, y2, y3, arg;
                   arg=w*sqrt(27/(-v))/(2*v);
                   arg=acos(arg)/3;
                   y1=2*sqrt(-v/3);                    
                   y2=-y1*cos(pidiv3-arg)-a/3;
                   y3=-y1*cos(pidiv3+arg)-a/3;
                   y1=y1*cos(arg)-a/3;
                   if( y1<0){ sigma=inf;}
                   else if (y3<0){ sigma=y1;}
                          else if(y2<0) {sigma=y3;}
                                 else { sigma=y2;}
                   if(s<sigma){ sigma=s;}
              }
          }
          else
          { 
            double x1, x2;
            if(a!=0)
            {
                 x1=b*b-4*a*c;
                 if (x1<0){ s=inf;}
                 else
                 { 
                      x1=sqrt(x1);
                      x2=(-b-x1)/2/a;
                      x1=(-b+x1)/2/a;
                      if(x1<0)
                      {
                           if(x2<0){s=inf;}
                           else {s=x2;}
                      }           
                      else if(x2<0){s=x1;}
                             else if(x1<x2){ s=x1;}
                                    else {s=x2;}
                 }
                 c=c+2*dQ;
                 x1=b*b-4*a*c;
                 if (x1<0){ sigma=inf;}
                 else
                 { 
                      x1=sqrt(x1);
                      x2=(-b-x1)/2/a;
                      x1=(-b+x1)/2/a;
                      if(x1<0)
                      {
                           if(x2<0){sigma=inf;}
                           else {sigma=x2;}
                      }           
                      else if(x2<0){sigma=x1;}
                             else if(x1<x2){ sigma=x1;}
                                    else {sigma=x2;}
                      }
                      if(s<sigma) sigma=s;
                 }
                 else
                  {
                       if(b!=0)
                       { 
                            x1=-c/b;
                            x2=x1-2*dQ/b;
                            if(x1<0) x1=inf;
                            if(x2<0) x2=inf;
                            if(x1<x2){sigma=x1;}else{sigma=x2;}
                       }
                   }
            }     
        if ((fabs(X-q))>dQ){sigma=0;};                        
    };
};
if((strcmp(Method,"BQSS")==0)||(strcmp(Method,"CQSS")==0)||(strcmp(Method,"TQSSG")==0)){
    if (e*u!=0) {
    X=X+e*u;
         if (u>0) { // we check the correct value of qi;
             if (X-qi>=dQ+ep) qi=qi+dQ;
         } else { //we check the correct value of qs;  
             if (qs-X>=dQ+ep) qs=qs-dQ;
         };
    };
    u=Aux[0];
    if (t==0) {// initialization
        if (band==1) { //we need to send the output
            sigma=0;
        } else {  //the output was already sent and there was a change in u
            if (u*(q-X)>=0) {  //q is still ok
                if (u!=0) {
                    sigma=(q-X)/u+eps;
                } else {
                    sigma=inf;
                };
             } else {   // q is wrong, but we wait an infinitesimal until changing it    
                sigma=eps;
             };
        };
    } else {
      if((sigma-e)<eps){
            sigma=2*eps;
      }else{ 
        if (e>0) {   //derivative change  
           if (u*(q-X)>=0) {  // q was ok
               if (u!=0) {
                   sigma=(q-X)/u+2*eps;
               }else{
                   sigma=inf;
               };
           } else { //we need to change q
               if((strcmp(Method,"BQSS")==0)||(strcmp(Method,"TQSSG")==0)){
                   sigma=0;
               }else{
                   if(u>0){
                         sigma=(qs-X)/u+2*eps;
                         q=qs;
                   }else{
                         sigma=(qi-X)/u+2*eps;
                         q=qi;
                   };
               };
           };
        }else { //we had already sent the value q 
           if (u*(q-X)>=0) { // q is still ok    
               if (u!=0) {
                   sigma=(q-X)/u+eps;
               } else {
                   sigma=inf;
               };      
           } else { // q is wrong, but we are close to u=0, so we set u=0.
               u=0;
               sigma=inf;
           };
        };
      };
  };
};
} else {
  X=Aux[0];
  q=X;
  sigma=0;
if((strcmp(Method,"BQSS")==0)||(strcmp(Method,"CQSS")==0)||(strcmp(Method,"TQSSG")==0)){
        ep=dQ/100;
        //ep=0;
        qs=floor(X/dQ)*dQ+dQ; 
        qi=floor(X/dQ-ep)*dQ; 
        q=qi;
        eps=1e-20;
       band=1;
  };
};
}

Event rintegrator::lambda(double t){
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
y[3]=0;
if (strcmp(Method,"QSS")==0) {
  if (u==0) {
    y[0]=q;
  } else {
    y[0]=q+dQ*u/fabs(u);
  };
};
if (strcmp(Method,"QSS2")==0) {
  y[0]=X+u*sigma+mu*sigma*sigma/2;
  y[1]=u+mu*sigma;
};
if(strcmp(Method,"QSS3")==0){
    y[0]=X+u*sigma+(mu*pow(sigma,2))/2 + (pu*pow(sigma,3))/3;
    y[1]=u + mu * sigma + pu * pow(sigma,2);
    y[2]=mu/2.0+ pu*sigma;
};
if((strcmp(Method,"BQSS")==0)||(strcmp(Method,"CQSS")==0)||(strcmp(Method,"TQSSG")==0)){
    if (sigma<=eps) {  //derivative change
         if(u>0){
             y[0]=qs;y[3]=1;
         }else{
             y[0]=qi;y[3]=2;
         };
    } else {   // X arrives to a new level (q)
         if(u>0){
            y[0]=q+dQ;y[3]=3;
         }else{
            y[0]=q-dQ;y[3]=4;
         };
    };
   // if(fabs(y[0]/dQ)<1){y[0]=0;};  //Corrige error numérico en torno a cero
    if((strcmp(Method,"CQSS")==0)||(strcmp(Method,"TQSSG")==0)){y[0]=(y[0]+X+sigma*u)/2;};
};
y[4]=X+sigma*u; //also output X
return Event(&y,0);
}

