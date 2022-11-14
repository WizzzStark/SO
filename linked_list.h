#ifndef LISTS_POINTERS_H
#define LISTS_POINTERS_H

#include <stdbool.h>

#define LNULL NULL 

/*Definiciones de tipos*/

typedef struct tNode* tPosL;
struct tNode{ //Nodos
    void* data; //Comando almacenados en la lista
    tPosL next; //El campo next es un puntero al siguiente nodo, que corresponde con las posicones de la lista
};
typedef tPosL tList;

typedef struct tAllocData {
    int size;
    char allocation[1024];
    char date[1024];
} tAllocData;

/*Prototipos de funciones*/ 
void createEmptyList(tList *L);
void deleteList(tList *T);
void freeList(tList *T);
bool insertItem(void* d, tPosL p, tList *L);
tPosL findItem(void* d, tList L);
bool isEmptyList(tList L);
void* getItem(tPosL p, tList L);
tPosL first(tList L);
tPosL last(tList L);
tPosL next(tPosL p, tList L);

#endif