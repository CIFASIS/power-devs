#include "transition.h"
void transition::init(double t,...) {
va_list parameters;
va_start(parameters,t);

char *fvar;
fvar = va_arg(parameters, char*);
time = getScilabVar(fvar);

nin = (int) va_arg(parameters,double); //number of inports 
inf = 1e20;
enabled = false;
working = false;
menssaje = new Menssaje;
portsinit = false;
if (nin == 0) //SOURCE TRANSITION (only ought to have one inport
{
 source = true;
 Sigma = time;
}
else
{
 data = new DataBase(nin);
 source = false;
 Sigma = inf;
}
}
double transition::ta(double t) {
return Sigma;
}
void transition::dint(double t) {
PlaceWeight *datos;
int weight;
if (enabled)
{ 
 enabled = false; 
 data -> Clean(); //limpia sus mensajes recibidos y pone a last a 0
 Sigma = inf;
 int j = 0;
 for (int i=0;i<data -> GetTotalPlaces(); i++)
 {
  datos = data -> GetPlace();     //obtiene el lugar que para el cual se va a enviar el mensaje de request
  predecessor = (place*)datos -> src;       //este es el lugar al que se le ejecuta 
  weight = datos -> weight;
  if (weight <= predecessor -> GetTokens())
   j++;
 }
 data -> Clean();
 if (j == data -> GetTotalPlaces())
 {
  //printLog("[t=%g] %s inicializada\n",t,name);
  data -> Clean();
  for (int i=0;i<data -> GetTotalPlaces(); i++)
  {
   datos = data -> GetPlace();     //obtiene el lugar que para el cual se va a enviar el mensaje de request
   predecessor = (place*)datos -> src;       //este es el lugar al que se le ejecuta 
   weight = datos -> weight;
   predecessor -> SetTokens(predecessor -> GetTokens() - weight);
   //printLog("[t=%g] %s resto %d marcas a %s \n",t,name,weight, predecessor -> name);
  }
  Sigma = 0;
  working = true;
 }
 else
  Sigma = inf;  //Dejo de estar activa
}
else if (working)
{
 working = false;
 data -> Clean(); //limpia sus mensajes recibidos y pone a last a 0
 Sigma = inf;
 for (int i=0;i<data -> GetTotalPlaces(); i++)
 {
  datos = data -> GetPlace();     //obtiene el lugar que para el cual se va a enviar el mensaje de request
  predecessor = (place*)datos -> src;       //este es el lugar al que se le ejecuta 
  predecessor -> externalinput(t,1);
 }
 //printLog("[t=%g] %s finalizada\n",t,name);
}
else if (source)
{
 Sigma = time;
}
else
 Sigma = inf;
}
void transition::dext(Event x, double t) {
//en el mensaje viene el puntero del lugar (id)y la cantidad de marcas que tiene este en un record
Menssaje* mens = (Menssaje*)x.value;

bool activ = false;
data -> Update(x.port,mens);       //actualiza la base de datos (puerto,mensaje)
//printLog("[t=%g] Update en %s puerto: %d marcas: %d \n", t, name, x.port, mens -> tokens);


if(portsinit == false && data->CalculateTotalPlaces())
 portsinit = true;

if(portsinit)
{
 data -> Update2(x.port,mens); 
 //printLog("TOTALPLACES: %d\n",tabla->GetTotalPlaces());
 activ = data -> Check();         //se fija si la transicion esta activa o no 
}
 /* ***************************************************************SE ACTIVA***************************************************************/ 

if (activ)
{
 if (enabled)                        //ya estaba activada
 { 
  ////printLog("[t=%g] %s esperando tiempo de transicion\n",t,name);
  Sigma = Sigma - e;
 }
 else                                 //SE ACABA DE ACTIVAR
 {
  Sigma = time;
  enabled = true;
  //printLog("[t=%g] %s activada\n",t,name);
 }
}
else                                   //no esta activa
{
 //printLog("[t=%g] %s Desactivada\n",t,name);
 enabled = false;
 Sigma = inf;
}
}
Event transition::lambda(double t) {
if (enabled) //primero mando el mensaje de que tengo ganas de transicionar
 return Event();//avisa que esta lista para transicionar

else if (working) // ahora comienzo a enviar los mensajes de resta y el de suma
{
 menssaje -> y[0] = 0;
 menssaje-> src = this;
 menssaje -> tokens = 0;
 return Event(menssaje,0); //por el puerto cero envio el mensaje de suma
}

else if (source) // Uniccamente para transiciones FUENTES
{
 menssaje -> y[0] = 0;
 menssaje -> src = this;
 menssaje -> tokens = 0;
 //printLog("[t=%g] %s finalizada\n",t,name); 
 return Event(menssaje,0); //por el puerto cero envio el mensaje de suma*/
}
//printLog("[t=%g] %s NO ENTRO AL IFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF\n",t,name);
}
void transition::exit() {
if (source == false) //si quedo activa y se termino la simulacion antes de que transicione
 data -> Remove();

if (nin != 0) 
 delete data;

delete menssaje;
}
