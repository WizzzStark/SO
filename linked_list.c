//AUTOR 1: Alberto Fernández Valiño - login: alberto.fernandezv
//AUTOR 2: Juan Villaverde Rodríguez - login: juan.villaverde.rodriguez
//Grupo 1.4 


#include "linked_list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



bool createNode(tPosL *p) {
    *p = malloc(sizeof(struct tNode)); 
    return *p != LNULL; //Si malloc devulve LNULL (No se pudo asignar memoria suficiente para el noodo), no se puede crear nodo

}

void createEmptyList(tList *L) {
    *L = LNULL; //Como le estamos pasando L como variable, y queremos modificar aquello a lo que apunta L, se pone el asterisco
}

/*void freeMmap(void *p) {
    tMmapData *data = p;


}*/

void removeItem(tPosL p, tList *L) {
    tPosL q;

    if (p == *L) {
        printf("AQUI1\n");
      *L = (*L) -> next;
    } //Borrar en primera posición
    else if (p -> next == LNULL) { //Borrar en ultima posicion
        for (q = *L; q->next != p;  q = q->next);
        q -> next = LNULL;

    } else { //Borrar en posicion intermedia
        q = p->next;
        p -> data = q->data;
        p -> next = q -> next;
        p = q;
    }
    free(p -> data);
    free(p);
}

void freeList(tList *L, void (*free_aux)(void *)){
    tPosL p, a;
    p = *L;
    while(p != LNULL){
        a = p;
        free_aux(p->data);
        p = p->next;
        free(a);
    }
    (*L) = LNULL;
}

bool insertItem(void* d, tList *L) { 
    tPosL q, r;
    //void* lineaReservada = malloc(sizeof(d));
    //d = lineaReservada;

    if (!createNode(&q))//La direcion *p la pasamos por referecia (&) como variable q,
        return false;
    else {
        q->data = d; //Q es una posicion (punntero  a un nodo) por lo tanto para cambiar algo en ese nodo usamos la flecha
        q -> next = LNULL;
        if (*L == LNULL) 
            *L = q; 
        else { 
            for (r = *L; r -> next != NULL; r = r ->next);
                r -> next = q;
        return true;
        }
    }
}

bool insertAllocData(tAllocData d, tList *L) {
    tAllocData *allocData2 = malloc(sizeof(tAllocData));
    allocData2 -> size = d.size;
	strcpy(allocData2 -> allocation, d.allocation);
	strcpy(allocData2 -> date, d.date);

    insertItem(allocData2, L);
}

bool insertString(char * string, tList *L) {
    char* lineaReservada = malloc(sizeof(string));
	strcpy(lineaReservada, string);
	insertItem(lineaReservada, L);

}

bool insertMmapData(tMmapData d, tList *L) {
    tMmapData *mmapData = malloc(sizeof(tMmapData));
    mmapData -> size = d.size;
	strcpy(mmapData -> allocation, d.allocation);
	strcpy(mmapData -> date, d.date);
	strcpy(mmapData -> file_name, d.file_name);
    mmapData -> file_descriptor = d.file_descriptor;

	insertItem(mmapData, L);

}

bool isEmptyList(tList L) {
    return L == LNULL;
}

void*  getItem(tPosL p, tList L) {
    void* elemento;
    elemento = p -> data;
    return elemento;
}

tPosL first(tList L) {
    return L;
}

tPosL last(tList L) {
    tPosL p;
    for (p = L; p->next != LNULL; p = p->next); 
    return p;
}

tPosL next(tPosL p, tList L) {
    return p->next;

}