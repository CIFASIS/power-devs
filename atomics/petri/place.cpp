#include "place.h"
void place::init(double t,...) {
//The 'parameters' variable contains the parameters transferred from the editor.
va_list parameters;
va_start(parameters,t);
char *fvar;
fvar = va_arg(parameters, char*);
m = getScilabVar(fvar);                 //initial total tokens

fvar = va_arg(parameters, char*);
time = getScilabVar(fvar);
nin = (int)va_arg(parameters,double);

inf = 1e20; //this number represent infinit
tokens = 0;
menssaje = new Menssaje;

list = new LinkedList();          //Linked List that have all the messajes with its time in order of apear 
if (m > 0)            //if ther initial tokens is greater than zero, set Sigma with "time"
{
 Sigma = time;
 TimeFlag = 1;
 for (int i=1;i<=m;i++)
  list -> Insert(menssaje,time);
}
else
{
 Sigma = inf;
 TimeFlag = 0; //No messajes waiting in the list
}
}
double place::ta(double t) {
//This function returns a double.
return Sigma;
}
void place::dint(double t) {
int length = list -> GetLength();
if (TimeFlag == 1) //if the token's time is zero enter here
{
 int auxtokens;
 list -> UpdateTime (list -> GetLessTime());
 auxtokens = tokens;
 tokens = tokens + list -> CheckMenssajes(); // here take out the tokens of the Linked List
 //printLog("[t=%g] Marcas disponibles en %s: %d (%d recien sacada/s de la lista)\n", t, name, tokens, tokens - auxtokens);
 Sigma = 0;
 TimeFlag = 4;
}
else if (TimeFlag == 4)
{
 if (length == 0)
 {
  TimeFlag = 0;
  Sigma = inf;
 }
 else
 {
  TimeFlag = 1;
  Sigma = list -> GetLessTime();
 }
}
}
void place::dext(Event x, double t) {
Menssaje* mens = (Menssaje*)x.value;
list -> UpdateTime(e); //Update de time of Linked List always

if ( x.port == -1)
{
 Sigma = 0;
 TimeFlag = 4;
}
else
{
 //printLog("[t=%g] Se agrega una marca a la lista de %s\n", t, name);
 list -> Insert(mens,time);
 if (Sigma > time) //Sigma = inf when it is the first messaje on arrived
 {
  Sigma = time;
  TimeFlag = 1;
 }
 else
  Sigma = Sigma - e;
}
}
Event place::lambda(double t) {
if (TimeFlag == 1) //enter here when the token's time on the list is zero
{
 return Event();
}
else if (TimeFlag == 4)
{
 //printLog("[t=%g] Marcas disponibles en %s: %d\n", t, name, tokens);
 menssaje -> y[0] = tokens;
 menssaje -> src = this;
 menssaje -> tokens = tokens;
 return Event(menssaje,0);
}
else
 printLog("[t=%g] %s Error\n",t,name);
}
void place::exit() {
delete menssaje;
delete list;
}
