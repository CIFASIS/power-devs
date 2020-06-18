#include "structures.h"
/*Lista enlazada donde cada casilla tiene un valor y un puntero que apunta al siguiente*/

#if !defined LINKEDLIST
#define LINKEDLIST
class LinkedList{
 public:
  LinkedList();
  ~LinkedList();
  bool IsEmpty();
  void Insert (Menssaje *m, double t);
  Menssaje* Remove();
  int CheckMenssajes();  //se fija cuantos mensajes cumplieron con su tiempo y los elimina
  void UpdateTime(double t);
  int GetLength();
  double GetLessTime();

 private:
  Node *first;
  Node *last;
  int length;
};
#endif
