//AUTOR 1: Alberto Fernández Valiño - login: alberto.fernandezv
//AUTOR 2: Juan Villaverde Rodríguez - login: juan.villaverde.rodriguez
//Grupo 1.4 

#include <stdio.h>
#include "commands.c"


int main() {

	char lineaCompleta[1024] ;
	char linea[1024];
	
	tList L;
	tList allocations;

	createEmptyList(&L);
	createEmptyList(&allocations);

	while(condicion) {
		printf(AMARILLO_T"># "RESET);

		if (fgets(linea, 1024, stdin) == NULL) { 
			exit(0);
		}
		quitarSalto(linea);
		strcpy(lineaCompleta, linea);
		numtrozos = TrocearCadena(linea, trozos);
		if (numtrozos == 0 || (numtrozos == 1 && strcmp("comando", trozos[0]) == 0)) continue;

		if(strcmp("comando", trozos[0]) != 0){
			char* lineaReservada = malloc(sizeof(char)*1024);
			strcpy(lineaReservada, lineaCompleta);
			insertItem(lineaReservada, &L);
		}

		procesarComando(&L, &allocations);
	}
}
