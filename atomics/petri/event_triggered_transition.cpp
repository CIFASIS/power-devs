#include "event_triggered_transition.h"
void event_triggered_transition::init(double t,...) {
inf = 1e20;
menssaje = new Menssaje;
menssaje -> y[0] = 0;
Sigma = inf;
}
double event_triggered_transition::ta(double t) {
return Sigma;
}
void event_triggered_transition::dint(double t) {
Sigma = inf;
}
void event_triggered_transition::dext(Event x, double t) {
thing = x.value;
Sigma = 0;
}
Event event_triggered_transition::lambda(double t) {
menssaje-> src = this; //no importa lo que yo ponga aca!!!
 menssaje -> tokens = 0; //no importa lo que yo ponga aca!!!  
 //printLog("[t=%g] %s finalizada\n",t,name); 
return Event(menssaje,0); //por el puerto cero envio el mensaje de suma
}
void event_triggered_transition::exit() {
delete menssaje;
}
