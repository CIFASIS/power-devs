#ifndef MODELICA_FUNCS_H
#define MODELICA_FUNCS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <modelica.h>
#include <simulation_runtime.h>
#include <simulation_result.h>

#ifdef __cplusplus
extern "C" {
#endif
int init_runtime();
int function_staticBlocks(int,double,double*,double*);
bool functionQssWhen(int,double,double*,double*);
int function_sample_setup(int, double* , double*); 
int function_updateDepend(double t, int zerocrossing);
double state_values (int);
double quantum_values (int);
char* int_method ();
double function_rel_acc ();
double function_abs_acc ();
bool condition(unsigned int);
void set_condition_to(unsigned int, bool);
void function_sample(int i, double *y);

#ifdef __cplusplus
}
#endif

#endif
