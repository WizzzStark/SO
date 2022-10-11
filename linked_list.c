//AUTOR 1: Alberto Fernández Valiño - login: alberto.fernandezv
//AUTOR 2: Juan Villaverde Rodríguez - login: juan.villaverde.rodriguez
//Grupo 1.4 


#include "linked_list.h"
#include <string.h>
#include <stdlib.h>


bool createNode(tPosL *p) {
    *p = malloc(sizeof(struct tNode)); 
    return *p != LNULL; //Si malloc devulve LNULL (No se pudo asignar memoria suficiente para el noodo), no se puede crear nodo

}

void createEmptyList(tList *L) {
    *L = LNULL; //Como le estamos pasando L como variable, y queremos modificar aquello a lo que apunta L, se pone el asterisco
}

void deleteList(tList *T){
    tPosL p;

    while(*T != LNULL){
        p = *T;
        *T = (*T)-> next;
        free(p);
    }
}

void freeList(tList *T){
    tPosL p;

    while(*T != LNULL){
        p = *T;
        *T = (*T)-> next;
        free(p);
    }
    *T = LNULL;
}



bool insertItem(tComando d, tPosL p, tList *L) { 
    tPosL q, r;

    if (!createNode(&q))//La direcion *p la pasamos por referecia (&) como variable q,
        return false;
    else {
        strcpy(q->data, d); //Q es una posicion (punntero  a un nodo) por lo tanto para cambiar algo en ese nodo usamos la flecha
        q -> next = LNULL;
        if (*L == LNULL) 
            *L = q; 
        else if (p == LNULL) { 
            for (r = *L; r->next != LNULL; r= r->next);//Recorremos toda la lista de izquierda a derecha hasta que el nodo r apunte a LNULL, donde se pondra q en vez de LNULL
            r -> next = q;
        }
        else if (p == *L) {//Si queremos añadir un nodo al principio de la lista
            q ->next = p; //Tenemos que hacer que *L deje de apuntar a el primer nodo ya puesto, para que apunte al nuevo nodo deseado y este nuevo si que apunte al que anteriormente era el primer nodo
            *L = q;
        }
        else { //Si queremos insertar en una posicion intermedia, en vez de almacenar el nuevo dato (data) en la nueva posicion (q), ponemos dicho dato en la posicion que ya estaba (p)
    
            strcpy(q -> data, p -> data); 
            strcpy(p -> data, d);
            q -> next = p -> next; 
            p -> next = q; 
        }
        return true;

    }
}//insertItem


tPosL findItem(tComando d, tList L) {
    tPosL p;

    for(p = L; (p != LNULL) && (strcmp(p -> data, d) != 0); p = p->next);//Mientras no se llegue al final y la data de la posicion p sea distinta de la buscada, sigue buscando
    return p;
}

bool isEmptyList(tList L) {
    return L == LNULL;
}

char* getItem(tPosL p, tList L) {
    char* comando;
    comando = p -> data;
    return comando;
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

