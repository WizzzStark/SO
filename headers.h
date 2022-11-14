//AUTOR 1: Alberto Fernández Valiño - login: alberto.fernandezv
//AUTOR 2: Juan Villaverde Rodríguez - login: juan.villaverde.rodriguez
//Grupo 1.4 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/utsname.h>
#include <errno.h>

#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <fcntl.h>

#include "linked_list.h"

#include <ctype.h>

#define MAXTROZOS 512
#define MAX_SIZE 1024

char *trozos[MAXTROZOS];
int numtrozos;

typedef struct cm_entrada {
	char *cm_nombre;
	int (*cm_fun)();
	char *ayuda;
}cm_entrada ;

cm_entrada cm_tabla[];

void procesarComando(tList *L, tList *mallocs);
char LetraTF (mode_t m);
char * ConvierteModo2 (mode_t m);

#define RESET          "\x1b[0m"
#define NEGRO_T        "\x1b[30m"
#define NEGRO_F        "\x1b[40m"
#define ROJO_T         "\x1b[31m"
#define ROJO_F         "\x1b[41m"
#define VERDE_T        "\x1b[32m"
#define VERDE_F        "\x1b[42m"
#define AMARILLO_T     "\x1b[33m"
#define AMARILLO_F     "\x1b[43m"
#define AZUL_T         "\x1b[34m"
#define AZUL_F         "\x1b[44m"
#define MAGENTA_T      "\x1b[35m"
#define MAGENTA_F      "\x1b[45m"
#define CYAN_T         "\x1b[36m"
#define CYAN_F         "\x1b[46m"
#define BLANCO_T       "\x1b[37m"
#define BLANCO_F       "\x1b[47m"
