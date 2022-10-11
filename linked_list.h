#ifndef LISTS_POINTERS_H
#define LISTS_POINTERS_H

#include <stdbool.h>

#define LNULL NULL 

/*Definiciones de tipos*/

typedef char tComando[1024];

typedef struct tNode* tPosL;
struct tNode{ //Nodos
    tComando data; //Comando almacenados en la lista
    tPosL next; //El campo next es un puntero al siguiente nodo, que corresponde con las posicones de la lista
};
typedef tPosL tList;

/*Prototipos de funciones*/ 
void createEmptyList(tList *L);
void deleteList(tList *T);
void freeList(tList *T);
bool insertItem(tComando d, tPosL p, tList *L);
tPosL findItem(tComando d, tList L);
bool isEmptyList(tList L);
char* getItem(tPosL p, tList L);
tPosL first(tList L);
tPosL last(tList L);
tPosL next(tPosL p, tList L);

#endif