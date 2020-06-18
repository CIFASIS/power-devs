/*Estructura de los mensajes que almacenan la base*/
#include "structures.h"

#if !defined DATABASE
#define DATABASE
class DataBase{
 public:
  DataBase(int n);
  ~DataBase();
  void Update(int port, Menssaje *item);
  void Update2 (int port, Menssaje *item);
  bool CalculateTotalPlaces();
  int GetTotalPlaces();
  PlaceWeight* GetPlace();
  void Clean();
  void Remove();
  
 
  bool Check();
  
 private:
  int length;            // tamaño del Array "puertos" (cantidad de de puestos de entrada)
  Menssaje** ports;     //Array que contiene tuplas (10 espacios libres,Lugar,marcas)
  PlaceWeight** places;    //Array places que contiene (Lugar,marcas,peso)
  int last;              //posicion del ultimo lugar enviado de el array "places"
  int totalPlaces;       //Total de lugares precendentes
};
#endif
