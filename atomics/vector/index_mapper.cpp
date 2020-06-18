#include "index_mapper.h"
void index_mapper::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
//To get a parameter: %Name% = va_arg(parameters,%Type%)
//where:
//      %Name% is the parameter name
//	%Type% is the parameter type

char *fvar= va_arg(parameters,char*);
char *fvarN= va_arg(parameters,char*);
N=getScilabVar(fvarN);
sigma=INF;
map=new std::list<int>[N];
double **A = new double* [N];;
for (int i = 0; i < N; i++)
	A[i] = new double[N];
int rows=N,cols=N;
getScilabMatrix(fvar, &rows, &cols, A);
for (int i=0;i<N;i++)
	for (int j=0;j<N;j++) {
		if (A[i][j]==1.0) {
			map[i].push_back(j);
	}
}
state=0;

}
double index_mapper::ta(double t) {
//This function returns a double.
return sigma;
}
void index_mapper::dint(double t) {
index++;
if (index==map[from].end()) {
	state=0;
	sigma=INF;
} else {
	sigma=0;
}
}
void index_mapper::dext(Event x, double t) {
//The input event is in the 'x' variable.
//where:
//     'x.value' is the value (pointer to void)
//     'x.port' is the port number
//     'e' is the time elapsed since last transition
vec=*(vector*)x.value;
if (vec.index >= N)
	return;
//printLog("t:%g Got an event from %d\n",t,vec.index);
if (vec.index!=-1 && state==0 && map[vec.index].size()){
		sigma=0;
		state=1;
		index=map[vec.index].begin();
		from=vec.index;
};
}
Event index_mapper::lambda(double t) {
//This function returns an Event:
//     Event(%&Value%, %NroPort%)
//where:
//     %&Value% points to the variable which contains the value.
//     %NroPort% is the port number (from 0 to n-1)
vec.index=*index;
//printLog("t:%g Emitting event from %d to index %d\n",t,from,vec.index);
return Event(&vec,0);
}
void index_mapper::exit() {
//Code executed at the end of the simulation.
delete []map;
}
