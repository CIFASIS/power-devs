#include "secant_solve.h"

double secant_solve(ROperation* pop, double uaux[10][10], double *f,double tol) {
	double z1,g1,z2,z3,g2;

	f[0]=f[0]+tol;
	z1=f[0];
	g1=pop->Val();
	if (g1==0) {
		z2=z1;
		g2=g1;
	} else {
		z2=z1+(z1/100+tol);
		f[0]=z2;
		g2=pop->Val();
	};
for (int i=0;i<10;i++) {
  if (fabs(g2)<tol) {
     i=10;
  }
  else {
    z3 = 1 / (g1 - g2) * (g1 * z2 - g2 * z1);
    z1 = z2;
    z2 = z3;
    g1 = g2; 
    f[0]=z2;
    g2=pop->Val();
  };
};



	return f[0];
}

