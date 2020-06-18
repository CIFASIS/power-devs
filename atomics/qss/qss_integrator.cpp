#include "qss_integrator.h"
void qss_integrator::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//      %Type% is the parameter type
Method=va_arg(parameters,char*);

char *fvar1= va_arg(parameters,char*);
char *fvar2= va_arg(parameters,char*);
char *fvar3= va_arg(parameters,char*);

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
double qss_integrator::ta(double t) {
//This function return a double.

return solver->ta(t);
}
void qss_integrator::dint(double t) {
solver->dint(t);

}
void qss_integrator::dext(Event x, double t) {
solver->e = e;
solver->dext(x,t);

}
Event qss_integrator::lambda(double t) {
Event e=solver->lambda(t);
return e;
}
void qss_integrator::exit() {
solver->exit();
delete solver;
}
