#include "cross3switch.h"
void cross3switch::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
X = va_arg(parameters,double);
inf=1e10;
sigma=inf;
v[0]=0;
v[1]=0;
mv[0]=0;
mv[1]=0;
u=0;
u2=0;
mu=0;
flag=0;
}
double cross3switch::ta(double t) {
//This function return a double.
return sigma;
}
void cross3switch::dint(double t) {
double t1;
double t2;
double disc;
double uv;
X = X + sigma*u + sigma*sigma*mu / 2;
v[0] = v[0] + mv[0]*sigma;
v[1] = v[1] + mv[1]*sigma;
uv = u;
u = u + sigma*mu;
if (flag == 0) {
  if (u * uv >= 0) {
    sigma = inf;
  }
  else {
    disc = u * u - 2 * (X - u2) * mu;
    if (disc < 0) {
      t1 = inf;
      t2 = inf;
    }
    else {
      t1 = ( -u - sqrt(disc)) / mu;
      if (t1 <= 0) {
        t1 = inf;
      }
      t2 = ( -u + sqrt(disc)) / mu;
      if (t2 <= 0) {
        t2 = inf;
      }
    }
    if (t1 <= t2) {
      sigma = t1;
    }
    else {
      sigma = t2;
    }
  }
}
else {
  sigma = sigma2;
}
flag=0;
}
void cross3switch::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number
double*Input;
double t1;
double t2;
double disc;
Input = (double * ) (x.value);
X = X + u*e + mu*e*e / 2;
v[0] = v[0] + mv[0]*e;
v[1] = v[1] + mv[1]*e;
u = u + mu*e;

//u2=0;
if ((x.port == 2) || (x.port==3)) {
  if (x.port == 3) {
    if ((u2-X)*(Input[0]-X)<0) {  //check if there is a change!
      flag=1;
    }  
    u2 = Input[0];
  }
 else {
  u = Input[0];
  mu = Input[1];
 }
    if (mu == 0) {
      if (u == 0) {
        t1 = inf;
        t2 = inf;
      }
      else {
        t1 = (u2 - X) / u;
        if (t1 <= 0) {
          t1 = inf;
        }
        t2 = inf;
      }
    }
    else {
      disc = u * u - 2 * (X - u2) * mu;
      if (disc < 0) {
        t1 = inf;
        t2 = inf;
      }
      else {
        t1 = ( -u - sqrt(disc)) / mu;
        if (t1 <= 0) {
          t1 = inf;
        }
        t2 = ( -u + sqrt(disc)) / mu;
        if (t2 <= 0) {
          t2 = inf;
        }
      }
    }
    if (t1 <= t2) {
      sigma = t1;
    }
    else {
      sigma = t2;
    }
  if (flag==1) {
    sigma2=sigma; // If the model was going to show a new value, we let it do it.
    sigma=0;
  }
}
else {
  sigma2 = sigma - e;
  v[x.port] = Input[0];
  mv[x.port] = Input[1];
  if (X >= u2) {
    if (x.port == 0) {
      sigma = 0;
      flag = 1;
    }
    else {
      sigma = sigma2;
    }
  }
  else {
    if (x.port == 1) {
      sigma = 0;
      flag = 1;
    }
    else {
      sigma = sigma2;
    }
  }
}
}
Event cross3switch::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
if (flag == 1) {
  if (X+u*sigma +mu*sigma*sigma/2>= u2) {   //take into account the elapsed time before the last transition.
    y[0] = v[0];
    y[1] = mv[0];
  }
  else {
    y[0] = v[1];
    y[1] = mv[1];
  }
}
else {
  if (u + mu * sigma >= 0) {
    y[0] = v[0];
    y[1] = mv[0];
  }
  else {
    y[0] = v[1];
    y[1] = mv[1];
  }
}
return Event(&y[0],0);
}
void cross3switch::exit() {

}
