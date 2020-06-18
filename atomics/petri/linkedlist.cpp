#include "linkedlist.h"

//int main(){return 0;}

LinkedList::LinkedList()
{
 first = last = NULL;
 length = 0;
}

LinkedList::~LinkedList()
{
 while (Remove() != NULL);
}

bool LinkedList::IsEmpty()
{
 if (first == NULL)
  return true;
 else
  return false; 
}


void LinkedList::Insert (Menssaje *m, double t)
{
 length++;
 Node *nuevo = new Node;
 nuevo -> menssaje = m; 
 nuevo -> time = t;
 nuevo -> next = NULL;

 if (IsEmpty())
 {
  first = nuevo;
  last = nuevo;
 }
 else
 {
  last -> next = nuevo;
  last = nuevo;
 }
}

Menssaje* LinkedList::Remove()
{
 Node *element = first;
 Menssaje* thing;
 if (IsEmpty()) 
  return NULL;
 
 length--;
 thing = first -> menssaje;
 
 if (first == last)
 {
  first = NULL;
  last = NULL;
 }
 else
 {
  first = element -> next;
 }

 delete element;
 return thing;
}

int LinkedList::CheckMenssajes()
{
 int contador = 0;
 
 if (IsEmpty())
  return 0;
 
 if (first == last)
 {
  Remove();
  return contador+1;
 }
 
 while (first->time <= 0)
 {
  Remove();
  contador++;
  if (first == last && first->time <= 0)
  {
   Remove();
   return contador+1;
  } 
 }
 return contador;
}

void LinkedList::UpdateTime(double t)
{
 if (IsEmpty() == false)
 {
  Node *ptr;
  for (ptr = first; ptr != NULL; ptr = ptr->next)
  {
   if (ptr -> time >= t) 
    ptr -> time = ptr -> time - t;
   else
    ptr -> time = 0;
  }
 }
}

int LinkedList::GetLength()
{
 return length;
}

double LinkedList::GetLessTime()
{
 if (length == 0)
  return 1e20;         //si la lista esta vacia retorno infinito
 else
  return first -> time;
}
