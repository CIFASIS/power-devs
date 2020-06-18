#include "qss/qsstools.h"

double minposroot(double *coeff, int order) {
  double mpr=INF;
  switch (order) {
  case 0:
	  mpr=INF;
	break;
  case 1:
    if (coeff[1]==0) { 
      mpr=INF;
    } else {
      mpr=-coeff[0]/coeff[1];
    };
    if (mpr<0) mpr=INF;
    break;
  case 2:
  if (coeff[2]==0 || (1000*fabs(coeff[2]))<fabs(coeff[1])){
	if (coeff[1]==0) { 
	  mpr=INF;
	} else {
	  mpr=-coeff[0]/coeff[1];
	};
	if (mpr<0) mpr=INF;
      } else {
	double disc;
	disc=coeff[1]*coeff[1]-4*coeff[2]*coeff[0];
	if (disc<0) {
	  //no real roots
	  mpr=INF;
	} else {
	  double sd,r1;
	  sd=sqrt(disc);
	  r1=(-coeff[1]+sd)/2/coeff[2];
	  if (r1>0) {
	    mpr=r1;
	  } else {
	    mpr=INF;
	  };
	  r1=(-coeff[1]-sd)/2/coeff[2];
	  if ((r1>0)&&(r1<mpr)) mpr=r1;
	};
      };
      break;

  case 3:
    if ((coeff[3]==0)||(1000*fabs(coeff[3])<fabs(coeff[2]))) {
      mpr=minposroot(coeff,2);
    } else {
      double q,r,disc;
      q=(3*coeff[3]*coeff[1]-coeff[2]*coeff[2])/9/coeff[3]/coeff[3];
      r=(9*coeff[3]*coeff[2]*coeff[1]-27*coeff[3]*coeff[3]*coeff[0]-2*coeff[2]*coeff[2]*coeff[2])/54/coeff[3]/coeff[3]/coeff[3];
      disc=q*q*q+r*r;
      mpr=INF;
      if (disc>=0) {
	//only one real root
	double sd,s,t,r1;
	sd=sqrt(disc);
	if (r+sd>0) {  
	  s=pow(r+sd,1.0/3);
	} else {
	  s=-pow(fabs(r+sd),1.0/3);
	};  
	if (r-sd>0) {  
	  t=pow(r-sd,1.0/3);
	} else {
	  t=-pow(fabs(r-sd),1.0/3);
	};  
	r1=s+t-coeff[2]/3/coeff[3];
	if (r1>0) mpr=r1;
      }  else {
	//three real roots
	double rho,th,rho13,costh3,sinth3,spt,smti32,r1;
        rho=sqrt(-q*q*q);
	th=acos(r/rho);
	rho13=pow(rho,1.0/3);
	costh3=cos(th/3);
	sinth3=sin(th/3);
	spt=rho13*2*costh3;
	smti32=-rho13*sinth3*sqrt(3);
	r1=spt-coeff[2]/3/coeff[3];
	if (r1>0) mpr=r1;
	r1=-spt/2-coeff[2]/3/coeff[3]+smti32;
	if ((r1>0)&&(r1<mpr)) mpr=r1;
	r1=r1-2*smti32;
	if ((r1>0)&&(r1<mpr)) mpr=r1;
      };
  
    };

    break;
  case 4:
    //Based on Ferrari's Method
    if ((coeff[4]==0)||(1000*fabs(coeff[4])<fabs(coeff[3]))) {
      mpr=minposroot(coeff,3);
    } else {
      double p,q,r,z0,b1,c1a,c1b,db1,dc1a,r1;
      p=-3*coeff[3]*coeff[3]/8/coeff[4]/coeff[4]+coeff[2]/coeff[4];
      q=coeff[3]*coeff[3]*coeff[3]/8/coeff[4]/coeff[4]/coeff[4]-coeff[3]*coeff[2]/2/coeff[4]/coeff[4]+coeff[1]/coeff[4];
      r=-3*pow(coeff[3],4)/256/pow(coeff[4],4)+coeff[2]*coeff[3]*coeff[3]/16/pow(coeff[4],3)-coeff[3]*coeff[1]/4/coeff[4]/coeff[4]+coeff[0]/coeff[4];
      double co[4];
      co[0]=-q*q;
      co[1]=p*p-4*r;
      co[2]=2*p;
      co[3]=1;
      z0=minposroot(&co[0],3);
      b1=-sqrt(z0);
      c1a=(p+z0)/2;
      c1b=-q/2/b1;
      db1=coeff[3]/2/coeff[4];
      dc1a=coeff[3]*coeff[3]/16/coeff[4]/coeff[4];
      mpr=INF;
      co[0]=c1a+c1b+b1/2*db1+dc1a;
      co[1]=b1+db1;
      co[2]=1;
      r1=minposroot(&co[0],2);
      if (r1>0) mpr=r1;
      co[0]=c1a-c1b-b1/2*db1+dc1a;
      co[1]=-b1+db1;
      r1=minposroot(&co[0],2);
      if ((r1>0)&&(r1<mpr))mpr=r1;
 
    };
    break;

  };
  return mpr;
}

void advance_time(double *coeff, double dt, int order) {
	if (order==-1) {
		if (coeff[4]!=0){
			order=4;
		} 
		else if (coeff[3]!=0){
			order=3;
		} 
		else if (coeff[2]!=0) {
			order=2;
		} 
		else {
			order=1;
		};
	};


switch (order) {
  case 0:
    break;	 
  case 1:
    coeff[0]=coeff[0]+dt*coeff[1];
    break;
  case 2:
    coeff[0]=coeff[0]+dt*coeff[1]+dt*dt*coeff[2];
    coeff[1]=coeff[1]+2*coeff[2]*dt;
    break;
  case 3:
    coeff[0]=coeff[0]+dt*coeff[1]+dt*dt*coeff[2]+dt*dt*dt*coeff[3];
    coeff[1]=coeff[1]+2*coeff[2]*dt+3*coeff[3]*dt*dt;
    coeff[2]=coeff[2]+3*coeff[3]*dt;
    break;
  case 4:
    coeff[0]=coeff[0]+dt*coeff[1]+dt*dt*coeff[2]+dt*dt*dt*coeff[3]+dt*dt*dt*dt*coeff[4];
    coeff[1]=coeff[1]+2*coeff[2]*dt+3*coeff[3]*dt*dt+4*coeff[4]*dt*dt*dt;
    coeff[2]=coeff[2]+3*coeff[3]*dt+6*coeff[4]*dt*dt;
    coeff[3]=coeff[3]+4*coeff[4]*dt;
  };
};

double evaluate_poly(double *coeff, double dt, int order) {
  switch (order) {
  case 0:
	  return coeff[0];
  case 1:
    return coeff[0]+dt*coeff[1];
    break;
  case 2:
    return coeff[0]+dt*coeff[1]+dt*dt*coeff[2];
    break;
  case 3:
    return coeff[0]+dt*coeff[1]+dt*dt*coeff[2]+dt*dt*dt*coeff[3];
    break;
  case 4:
    return coeff[0]+dt*coeff[1]+dt*dt*coeff[2]+dt*dt*dt*coeff[3]+dt*dt*dt*dt*coeff[4];
  };
  return 0.0;
};


void printLog(const char *,...);



double estimate_sigma_prod(double y_n1, double y_n2, int order, double tol, double input[2][10], double output[10])
{
  unsigned iter=0,max_iter=200;
  double sigma=0;
  double eps=1e-30;
  if (fabs(y_n1)>eps) {
  	sigma=.8* pow(fabs(tol/y_n1),1.0/order);
  } 
  if (fabs(y_n1)<eps && fabs(y_n2)>eps) { 
		sigma=.8*pow(fabs(tol/y_n2),1.0/(order+1));
  }

  if (fabs(sigma)>eps && fabs(evaluate_poly(input[0],sigma,order)*evaluate_poly(input[1],sigma,order)-evaluate_poly(output,sigma,order))>tol) { 
	  double err=fabs(evaluate_poly(input[0],sigma,order)*evaluate_poly(input[1],sigma,order)-evaluate_poly(output,sigma,order));
	  while (err>tol && iter<max_iter) { 
		  sigma=sigma*.8*pow(tol/err,1/order);
	    err=fabs(evaluate_poly(input[0],sigma,order)*evaluate_poly(input[1],sigma,order)-evaluate_poly(output,sigma,order));
      iter++;
	  }
  }
  if (fabs(sigma)<eps || fabs(evaluate_poly(input[0],sigma,order)*evaluate_poly(input[1],sigma,order)-evaluate_poly(output,sigma,order))>tol) { 
	  sigma = 1e-10;
    iter=0;
    while (fabs(evaluate_poly(input[0],2*sigma,order)*evaluate_poly(input[1],2*sigma,order)-evaluate_poly(output,2*sigma,order))<tol && iter<max_iter) {
      sigma*=2;
      iter++;
    }
  }
  return sigma;
}

double estimate_sigma(double y_n1, double y_n2, int order, double tol, double *input, double *output,
                      double (*f)(double))
{
  unsigned iter=0,max_iter=200;
  double sigma=0;
  double eps=1e-20;
  if (fabs(y_n1)>eps) {
  	sigma=.8* pow(fabs(tol/y_n1),1.0/order);
	  printLog("With first coefficient u={%g %g %g} sigma=%g y_n1=%g\n",input[0],input[1],input[2],sigma,y_n1);
  } 
  if (fabs(y_n1)<eps && fabs(y_n2)>eps &&
     fabs(sigma)>eps && fabs(f(evaluate_poly(input,sigma,order))-evaluate_poly(output,sigma,order))>tol) { 
		sigma=.8*pow(fabs(tol/y_n2),1.0/(order+1));
		printLog("With second coefficient u={%g %g %g} sigma=%g y_n2=%g \n",input[0],input[1],input[2],sigma,y_n2); 
  }

  if (fabs(sigma)>eps && fabs(f(evaluate_poly(input,sigma,order))-evaluate_poly(output,sigma,order))>tol) { 
	  double err=fabs(f(evaluate_poly(input,sigma,order))-evaluate_poly(output,sigma,order));
	  while (err>tol && iter<max_iter) { 
		  sigma=sigma*.8*pow(tol/err,1/order);
	    err=fabs(f(evaluate_poly(input,sigma,order))-evaluate_poly(output,sigma,order));
		  printLog("With iteration coefficient u={%g %g %g} sigma=%g  y_n1=%g\n",input[0],input[1],input[2],sigma,y_n1); 		
      iter++;
	  }
  }
  if (fabs(sigma)<eps || fabs(f(evaluate_poly(input,sigma,order))-evaluate_poly(output,sigma,order))>tol) { 
	  sigma = 1e-10;
    iter=0;
    while (fabs(f(evaluate_poly(input,sigma*2,order))-evaluate_poly(output,sigma*2,order))<tol && iter<max_iter) {
      sigma*=2;
      iter++;
    }
	  printLog("Using a bisection step u={%g %g %g} sigma=%g \n",input[0],input[1],input[2],sigma); 	
  }
  return sigma;
}
