#include "simulator.h"

#if !defined ESTRUCTURAS
#define ESTRUCTURAS
typedef struct sms{
        double y[10];
        Simulator *src;
        int tokens;
        }Menssaje;
        
typedef struct node {
        Menssaje *menssaje;
        double time;
        struct node *next;
        } Node;

typedef struct placeweight{
        Simulator *src;
        int tokens;
        int weight;
        }PlaceWeight;
#endif
