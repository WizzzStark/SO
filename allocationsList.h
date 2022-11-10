#ifndef ALLOCATIONS_LIST_H
#define ALLOCATIONS_LIST_H

#include <stdbool.h>

#define NULL_ALLOC NULL 
#define N 50

/*Definiciones de tipos*/

typedef int tSize;
typedef char tAllocation[N];
typedef char tDate[N];
//typedef char tFileName[N];
typedef char tAllocationType[N];
//typedef int tFileDescriptor;
//typedef int tKey;

typedef struct tAllocData {
    tSize size;
    tAllocation allocation;
    tDate date;
    //tFileName fileName;
    tAllocationType allocationType;
} tAllocData;

typedef struct tAllocNode* tAllocPos;
struct tAllocNode{ //Nodos
    tAllocData data; //Comando almacenados en la lista
    tAllocPos next; //El campo next es un puntero al siguiente nodo, que corresponde con las posicones de la lista
};
typedef tAllocPos tAllocList;

/*Prototipos de funciones*/ 
void createEmptyAllocList (tAllocList *);
bool insertElement(tAllocData , tAllocList *);
void removeElement(tAllocPos , tAllocList *);
tAllocData getElement(tAllocPos , tAllocList );
tAllocPos firstElement(tAllocList );
tAllocPos lastElement(tAllocList );
tAllocPos nextElement(tAllocPos , tAllocList );
bool isEmptyAllocList(tAllocList);
//tAllocPos findElementBySize(tSize , tAllocList, char caso[]);
//tAllocPos findElementByKey(tKey d, tAllocList L);
//tAllocPos findElementByAddr(tAllocation d, tAllocList L);

#endif