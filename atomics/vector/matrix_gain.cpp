#include "matrix_gain.h"
void matrix_gain::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type

char *fvar= va_arg(parameters,char*);
char buff[128];
sprintf(buff,"tempvar=%s",fvar);
executeVoidScilabJob(buff,true);
sprintf(buff,"size(tempvar,1)");
rows=(int)getScilabVar(buff);
sprintf(buff,"size(tempvar,2)");
cols=(int)getScilabVar(buff);


//allocate input vector
X = (double **)calloc(cols,sizeof(double*));
	for (int i = 0; i < cols; i++)
		X[i] = (double*)calloc(10,sizeof(double));

//allocate matrix gain
A = (double **)calloc(rows,sizeof(double*));
	for (int i = 0; i < rows; i++)
		A[i] = (double*)calloc(cols,sizeof(double));

//allocate sparse vectors and matrices
nnrows = (int*)calloc(rows,sizeof(int));
nncols = (int*)calloc(cols,sizeof(int));
outinds = (int*)calloc(rows,sizeof(int));


for (int i = 0; i < rows; i++) nnrows[i]=0;

for (int i = 0; i < cols; i++) nncols[i]=0;

Arows = (int **)calloc(rows,sizeof(int*));
	for (int i = 0; i < rows; i++)
		Arows[i] = (int*)calloc(cols,sizeof(int));

Acols = (int **)calloc(cols,sizeof(int*));
	for (int i = 0; i < cols; i++)
		Acols[i] = (int*)calloc(rows,sizeof(int));


getScilabMatrix(fvar, &rows, &cols, A);


for (int i = 0; i < rows; i++) {
	for (int j = 0; j < cols; j++) {
		if (A[i][j]!=0) {
			Arows[i][nnrows[i]]=j;
			nnrows[i]++;
			Acols[j][nncols[j]]=i;
			nncols[j]++;
		}
	}
}



n=cols;

order=1;

for (int i=0;i<10;i++) {
  for (int j=0;j<n;j++) {
  		X[j][i]=0;
  };
  y[i]=0;
};

Sigma=INF;
outs=0;
nextout=0;
}
double matrix_gain::ta(double t) {
//This function returns a double.
return Sigma;
}
void matrix_gain::dint(double t) {
nextout++;
if (outs==nextout){
	Sigma=INF;
	outs=0;
} 
  else Sigma=0;

}
void matrix_gain::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'vec.index' is the port number
vector vec;
double *Xv;
vec=*(vector*)x.value;
Xv=(double*)x.value;

if (vec.index==-1) {
	for (int i=0;i<n;i++) {
		X[i][0]=Xv[0];
		X[i][1]=Xv[1];
		X[i][2]=Xv[2];
		X[i][3]=Xv[3];
      	}
	nextout=0;
	outs=rows;
	for (int i=0;i<rows;i++) {
		outinds[i]=i;
			}
  
} else {
  switch(order) {
  case 1: 
		X[vec.index][0]=Xv[0];
     if (Xv[1]!=0){order=2;X[vec.index][1]=Xv[1];}
     if (Xv[2]!=0){order=3;X[vec.index][2]=Xv[2];}  
     if (Xv[3]!=0){order=4;X[vec.index][3]=Xv[3];}  

  break;
  case 2: 
		X[vec.index][0]=Xv[0];
		X[vec.index][1]=Xv[1];

      for (int i=0;i<n;i++) {
      	 if (i!=vec.index) {
				advance_time(X[i],e,1);
			};
		};
     if (Xv[2]!=0){order=3;X[vec.index][2]=Xv[2];}  
     if (Xv[3]!=0){order=4;X[vec.index][3]=Xv[3];}  
  break;
  case 3: 
		X[vec.index][0]=Xv[0];
		X[vec.index][1]=Xv[1];
		X[vec.index][2]=Xv[2];
      for (int i=0;i<n;i++) {
      	 if (i!=vec.index) {
				advance_time(X[i],e,2);
			};
		};
     if (Xv[3]!=0){order=4;X[vec.index][3]=Xv[3];}  
  break;
  case 4: 
		X[vec.index][0]=Xv[0];
		X[vec.index][1]=Xv[1];
		X[vec.index][2]=Xv[2];
		X[vec.index][3]=Xv[3];
      for (int i=0;i<n;i++) {
      	 if (i!=vec.index) {
				advance_time(X[i],e,3);
			};
		};
  }
	if (outs==0) {
		nextout=0;
		outs=nncols[vec.index];
		for (int i=0;i<outs;i++)	{
			outinds[i]=Acols[vec.index][i];
		}
	} else {

		int newouts=0;
		bool found;
		for (int j=0;j<nncols[vec.index];j++) {
			found=false;
			for (int i=nextout;i<outs;i++) {
				if (outinds[i]==Acols[vec.index][j]) {
					found=true;
					i=outs;
				}
			}
			if (!found) {
				outinds[outs+newouts]=Acols[vec.index][j];
				newouts++;
			}
		}
		outs=outs+newouts;
	}
}
Sigma=0;

}
Event matrix_gain::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
int i;
for (int j=0;j<order;j++) {
	y[j]=0;
	for (int k=0;k<nnrows[outinds[nextout]];k++) {
		i=Arows[outinds[nextout]][k];
		y[j]=y[j]+A[outinds[nextout]][i]*X[i][j];
	};
};
yvec=*(vector*)y;
yvec.index=outinds[nextout];

return Event(&yvec,0);
}
void matrix_gain::exit() {

}
