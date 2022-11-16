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
#include <ctype.h>

#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <fcntl.h>

#include <sys/shm.h>
#include <sys/mman.h>

#include "linked_list.h"

#define MAXTROZOS 512
#define MAX_SIZE 1024

char *trozos[MAXTROZOS];
int numtrozos;
bool condicion = true;

typedef struct cm_entrada {
	char *cm_nombre;
	int (*cm_fun)();
	char *ayuda;
}cm_entrada ;

cm_entrada cm_tabla[];

char* quitarSalto(char *str);
void procesarComando(tList *L, tList *mallocs, tList *shared, tList *mmap);
char LetraTF (mode_t m);
char * ConvierteModo2 (mode_t m);

int cmdMalloc(tList *L, tList *mallocs);
int cmdShared(tList *L, tList *mallocs, tList *shared);
int cmdMmap(tList *L, tList *mallocs ,tList *shared, tList *maps);
void imprimirTodos(tList L, tList mallocs, tList shared, tList mmap);
void imprimirAllocations(tList list, char* allocation_type, bool tag);
int cmdAllocate(tList *L, tList *mallocs, tList *shared, tList *mmap);

void * ObtenerMemoriaShmget (key_t clave, size_t tam, tList *shared);
void do_AllocateCreateshared (char *tr[], tList *shared);
void do_DeallocateDelkey (char *args[]);
ssize_t LeerFichero (char *f, void *p, size_t cont);
void * MapearFichero (char * fichero, int protection, int *df);
int freeMmap(char * fichero, tList *list);
void do_AllocateMmap(char *arg[], tList *maps);

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
