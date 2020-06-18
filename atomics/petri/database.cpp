#include "database.h"

DataBase::DataBase(int n)
{
 ports = new Menssaje* [n];
 places = new PlaceWeight* [n];
 length = n;
 for(int i=0;i<length;i++)
 {
   ports[i] = NULL;
   places[i] = NULL;
 }
 last = 0;
 totalPlaces = 0;
}


DataBase::~DataBase()
{
 delete []ports;
 delete []places;
}


void DataBase::Update(int port,Menssaje *item) 	// cada posicion del arreglo reprensenta un puerto de entrada
{
ports[port] = item;
}

bool DataBase::CalculateTotalPlaces()
{
 int i, j, weight;
 bool visto;
 for (i=0;i<length;i++)                        
   if (ports[i] == NULL)   //Me fijo si estan todos los puertos ocupados si no estan
    return false;
 
 int cant_lug = 0; //cantidad de lugares precendetes
 
 for (i=0;i<length;i++)
 {
  visto = false;
  for (j=0;j<cant_lug;j++)                //ya fue contado este lugar?
   if (ports[i]->src == places[j]->src)
     visto = true;
  
  if (visto == false)                //si el lugar no fue contado entra
  {
   weight = 0;                      // no cuento con el que comparo se cuenta en la comparacion
   for (j=i;j<length;j++)
    if(ports[i]->src == ports[j]->src)
     weight++;
  places[cant_lug] = new PlaceWeight;
  places[cant_lug] -> src = ports[i] -> src;
  places[cant_lug] -> tokens = ports[i] -> tokens;
  places[cant_lug] -> weight = weight;
  cant_lug++;
  }
 }
 
 totalPlaces = cant_lug;
 return true;
}

void DataBase::Update2 (int port, Menssaje *item)
{
 for (int i=0;i<totalPlaces;i++)
  if (places[i] -> src == item -> src)
  {
   places[i] -> tokens = item -> tokens;
   return;
  }
}


bool DataBase::Check()
{
 int i, valida = 0;
 
 for (i=0;i<totalPlaces;i++)
  if (places[i]->weight <= places[i]->tokens)
   valida++;
 
 if(valida == totalPlaces)
  return true;
 else
  return false;
}


/*Obtiene la proxima record (Placesweight) a enviar*/
PlaceWeight* DataBase::GetPlace()
{
 if (places[last]!= NULL)
 {
  last++;
  return places[last-1];
 }
 printLog ("Error sobre paso la cantidad de lugares totales Getplace()\n");
 return NULL; //esto nunca deberia pasar
}

//limpia todas las entradas del arreglo
void DataBase::Clean()
{
 last = 0;
 for(int i=0;i<length;i++)
  ports[i] = NULL;
}

int DataBase::GetTotalPlaces()
{
 return totalPlaces;
}

void DataBase::Remove()
{
  for (int i=0;i<totalPlaces;i++)
   delete places[i];
}
