/*
secant_solve.h
by Ernesto Kofman

*/

#ifndef _SECANT_SOLVE_H
#define _SECANT_SOLVE_H

#include<stdlib.h>
#include<math.h>
#include "mathexpr.h"

double secant_solve(ROperation* pop, double uaux[10][10], double *f,double tol);

#endif
