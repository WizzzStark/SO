//AUTOR 1: Alberto Fernández Valiño - login: alberto.fernandezv
//AUTOR 2: Juan Villaverde Rodríguez - login: juan.villaverde.rodriguez
//Grupo 1.4 

#include <stdio.h>
#include "commands.c"


int main() {
	tComando lineaCompleta;
	tList L;
	createEmptyList(&L);
	while(1) {
		printf(AMARILLO_T"># "RESET);
		if (fgets(linea, 1024, stdin) == NULL) { 
			exit(0);
		}
		quitarSalto(linea);
		strcpy(lineaCompleta, linea);
		numtrozos = TrocearCadena(linea, trozos);
		if (numtrozos == 0 || (numtrozos == 1 && strcmp("comando", trozos[0]) == 0)) continue;

		if(strcmp("comando", trozos[0]) != 0){
			if(isEmptyList(L)){
				insertItem(lineaCompleta,first(L),&L);
			}else{
				insertItem(lineaCompleta,next(last(L), L),&L);
			}
		}

		procesarComando(&L);
		
	}
}
