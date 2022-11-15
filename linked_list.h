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

typedef struct tSharedData {
    int size;
    char allocation[1024];
    char date[1024];
    int key;
} tSharedData;

typedef struct tMmapData {
    int size;
    char allocation[1024];
    char date[1024];
    int file_descriptor;
    char file_name[1024];
} tMmapData;

/*Prototipos de funciones*/ 
void createEmptyList(tList *L);
void freeList(tList *L, void (*free_aux)(void *));
bool insertItem(void* d, tList *L);
bool isEmptyList(tList L);
void* getItem(tPosL p, tList L);
tPosL first(tList L);
tPosL last(tList L);
tPosL next(tPosL p, tList L);

bool insertAllocData(tAllocData d, tList *L);
bool insertString(char * string, tList *L);

#endif