#include "allocationsList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


bool createAllocNode(tAllocPos *p) {
    *p = malloc(sizeof(**p));
    return *p != NULL_ALLOC;
}

void createEmptyAllocList(tAllocList *L) {
    *L = NULL_ALLOC;
}

bool insertElement(tAllocData d, tAllocList *L) {
    tAllocPos q, r;
    if (!createAllocNode(&q))
        return false;
    else {
        q->data = d;
        q->next = NULL_ALLOC;
        if (*L == NULL_ALLOC) { //Lista vacia?
        	*L = q;
        }
        else { //Busca la posicion
            for (r = *L; r -> next != NULL_ALLOC; r = r ->next);
            r -> next = q;
        }
        return true;
    }
}


void removeElement(tAllocPos p, tAllocList *L) {
    tAllocPos q;

    if (p == *L) {
      *L = (*L) -> next;
    } //Borrar en primera posición
    else if (p -> next == NULL_ALLOC) { //Borrar en ultima posicion
        for (q = *L; q->next != p;  q = q->next);
        q -> next = NULL_ALLOC;

    } else { //Borrar en posicion intermedia
        q = p->next;
        p -> data = q->data;
        p -> next = q -> next;
        p = q;
    }
    free(p);
}
tAllocPos firstElement(tAllocList L) {
    return L;
}

tAllocPos lastElement(tAllocList L) {
    tAllocPos p;
    for (p = L; p->next != NULL_ALLOC; p = p->next); //El tiempo de ejecucion de last depende del numero de elementos de la lista
    return p;
}

tAllocPos nextElement(tAllocPos p, tAllocList L) {
    return p->next;
}

tAllocData getElement(tAllocPos p, tAllocList L) {
    return p->data;
}

bool isEmptyAllocList(tAllocList L) {
    return L == NULL_ALLOC;
}

/*tAllocPos findElementBySize(tSize d, tAllocList L, char caso[]) {
    tAllocPos p;

    for (p = L; (p != NULL_ALLOC) && (d != p -> data.size) && strcmp(caso, p -> data.allocationType) == 0; p = p -> next);
    if (p != NULL_ALLOC && (d == p -> data.size))
        return p;
    else
        return NULL_ALLOC;
}

tAllocPos findElementByKey(tKey d, tAllocList L) {
    tAllocPos p;

    for (p = L; (p != NULL_ALLOC) && (d != p -> data.key); p = p -> next);
    if (p != NULL_ALLOC && (d == p -> data.key))
        return p;
    else
        return NULL_ALLOC;
}

tAllocPos findElementByAddr(tAllocation d, tAllocList L) {
  tAllocPos p;

  for (p = L; (p != NULL_ALLOC) && (strcmp(d, p -> data.allocation) != 0); p = p -> next);
  if (p != NULL_ALLOC && (strcmp(d, p -> data.allocation) == 0))
      return p;
  else
      return NULL_ALLOC;
}*/