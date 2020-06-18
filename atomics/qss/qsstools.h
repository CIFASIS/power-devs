/*
qsstools.h
by Ernesto Kofman

*/

#ifndef _QSSTOOLS_H
#define _QSSTOOLS_H

#include<stdlib.h>
#include<math.h>
#define INF 1e20

double minposroot(double *coeff, int order);
void advance_time(double *coeff, double dt, int order);
double evaluate_poly(double *coeff, double dt, int order);
double estimate_sigma(double y_n1, double y_n2, int order, double tol, double *input, double *output,double (*f)(double));
double estimate_sigma_prod(double y_n1, double y_n2, int order, double tol, double input[2][10], double output[10]);
#endif
