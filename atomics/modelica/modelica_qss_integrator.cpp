#include "modelica_qss_integrator.h"

DATA* initializeDataStruc();
void modelica_qss_integrator::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
Index=(int)va_arg(parameters,double);
Method=int_method();
init_runtime();

double dQmin=function_abs_acc();
double dQrel=function_rel_acc();
double X0=state_values(Index);
char fvar1[128],fvar2[128],fvar3[128];
sprintf(fvar1,"%g",dQmin);
sprintf(fvar2,"%g",dQrel);
sprintf(fvar3,"%g",X0);
printLog("Index=%i, Initial state=%g, dQmin=%g, dQrel=%g \n",Index,X0,dQmin,dQrel);

/**********************************************
* NOTE: The actual implementation of the QSS solvers
* have been split in separate files for maintenance 
* reasons.
* QSS solver is in atomics/qss/qss.cpp
* QSS2 solver is in atomics/qss/qss2.cpp
* ...
* ...
* You can edit any one of them like a normal Atomic
* model (from the Atomic Editor)
***********************************************/
if (strcmp(Method,"QSS")==0) {
	met=QSS;
	solver= new qss(name);
} else if (strcmp(Method,"QSS2")==0) {
  met=QSS2;       
	solver= new qss2(name);
} else if (strcmp(Method,"QSS3")==0){
	met=QSS3;  
	solver= new qss3(name);
} else if (strcmp(Method,"QSS4")==0){
    met=QSS4;  
	solver= new qss4(name);
} else if (strcmp(Method,"BQSS")==0) {
    met=BQSS;  
	solver= new bqss(name);
} else if (strcmp(Method,"CQSS")==0) {
    met=CQSS;  
	solver= new cqss(name);
} else if (strcmp(Method,"LIQSS")==0) {
    met=LIQSS;  
	solver= new liqss(name);
} else if (strcmp(Method,"LIQSS2")==0) {
    met=LIQSS2;  
	solver= new liqss2(name);
} else if (strcmp(Method,"LIQSS3")==0) {
    met=LIQSS3;  
	solver= new liqss3(name);
} 
solver->init(t,fvar1,fvar2,fvar3);
}
double modelica_qss_integrator::ta(double t) {
//This function return a double.
return solver->ta(t);
}
void modelica_qss_integrator::dint(double t) {
  globalData->timeValue = t;
  solver->dint(t);
}
void modelica_qss_integrator::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value
//     'x.port' is the port number

  globalData->timeValue = t;
  solver->e = e;
  solver->dext(x,t);
}
Event modelica_qss_integrator::lambda(double t) {
//This function return an event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% is a direction to the variable that contain the value.
//     %NroPort% is the port number (from 0 to n-1)
Event e=solver->lambda(t);
printLog("[t=%g] Int: %d Emitting {%g,%g}\n",t,Index,e.getDouble(),e.getDouble(1));
return e;
}
void modelica_qss_integrator::exit() {
  solver->exit();
  delete solver;
}
