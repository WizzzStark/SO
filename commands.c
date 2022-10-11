//AUTOR 1: Alberto Fernández Valiño - login: alberto.fernandezv
//AUTOR 2: Juan Villaverde Rodríguez - login: juan.villaverde.rodriguez
//Grupo 1.4 


#include "headers.h"

void procesarComando(tList *L);


void printLongStats(bool opacc, struct stat st){
	struct passwd *p;
	struct group *g;
	struct tm *t;

	t = localtime((opacc) ? &st.st_atime : &st.st_mtime);
	printf("%04d/%02d/%02d-", t->tm_year+1900, t->tm_mon+1, t->tm_mday);
	printf("%02d:%02d,", t->tm_hour, t->tm_min);
	printf("%2ld ", st.st_nlink);
	printf("(%lu)", st.st_ino);
	printf("\t%s", ((p = getpwuid(st.st_uid)) == NULL) ? "???" : p->pw_name);
	printf("\t%s\n", ((g = getgrgid(st.st_gid)) == NULL) ? "???" : g->gr_name);

	// if link True :

}

char LetraTF (mode_t m)
{
     switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */ 
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
     }
}

char * ConvierteModo2 (mode_t m)
{
    static char permisos[12];
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    
    return permisos;
}



char* quitarSalto(char *str){
	int length = strlen(str)-1;
	if(str[length] == '\n')
          str[length] = '\0';
	return str;
}

void imprimirLista (tList list) {
	tPosL pos;
	char* comando;

	if (!isEmptyList(list)) {
		pos = first(list);
		int j = 0;
		while (pos != LNULL) {
			comando = getItem(pos, list);
			printf(VERDE_T"%d->%s\n"RESET, j, comando);
			pos = next(pos, list);
			j++;
		}
	}else{
		printf(ROJO_T"[+] No se han introducido comandos"RESET);
	}
}

int TrocearCadena(char * cadena, char * trozos[]) { 
	
	int i=1;
	if ((trozos[0]=strtok(cadena," \n\t")) == NULL)
		return 0;
	while ((trozos[i]=strtok(NULL," \n\t")) != NULL)
		i++;
	return i;
}

void cmdFin(tList *L) {
	deleteList(L);
	printf(ROJO_T "\n[!] Saliendo de la shell ...\n\n" RESET);
	exit(0);
}

void cmdAutores() {
	int flagL=1, flagN=1;
	
	if (numtrozos >=2 && strcmp(trozos[1], "-n") == 0) flagL=0;
	else if (numtrozos >= 2 && strcmp(trozos[1], "-l") == 0) flagN=0;

	if (flagN)
		printf(VERDE_T "[+] autor 1: Alberto Fernández\n[+] autor 2: Juan Villaverde\n" RESET);
	if (flagL)
		printf(VERDE_T"[+] login 1: alberto.fernandezv\n[+] login 2: juan.villaverde.rodriguez\n" RESET);
}

void cmdPid() {
	if (numtrozos == 1) {
		printf(VERDE_T"pid de shell: %d\n"RESET, getpid());
	}
	if (numtrozos >= 2 && strcmp(trozos[1], "-p") == 0) {
		printf(VERDE_T"pid del padre del shell: %d\n"RESET, getppid());
	}
	if (numtrozos >= 2 && strcmp(trozos[1], "-p") != 0) {
		printf(VERDE_T"pid de shell: %d\n"RESET, getpid());
	}
}

void cmdFecha() {
	int flagD = 1, flagH = 1;
	time_t tiempo;
	struct tm *t;
	
	if (numtrozos > 1 && strcmp(trozos[1], "-d") == 0) flagH = 0;
	if (numtrozos > 1 && strcmp(trozos[1], "-h") == 0) flagD = 0;
	
	if (time(&tiempo) == -1) {perror("tiempo"); return; }
	t = localtime(&tiempo);
	if (numtrozos > 1 && strcmp(trozos[1], "-d") != 0)
		if (numtrozos > 1 && strcmp(trozos[1], "-h") != 0)
			flagD = 0, flagH = 0;
		
	if (flagH)
		printf(VERDE_T"%02d:%02d:%02d\n"RESET, t->tm_hour, t->tm_min, t->tm_sec);
	if (flagD)
		printf(VERDE_T"%02d/%02d/%04d\n"RESET, t->tm_mday, t->tm_mon +1, t->tm_year + 1900);
}

void cmdInfosis() {
	struct utsname info[1];
	if (uname(info) == -1) {perror("uname"); return; }
	else {
		printf(VERDE_T"System name: %s\n"RESET, info->sysname);
		printf(VERDE_T"Node name: %s\n"RESET, info->nodename);
		printf(VERDE_T"Release: %s\n"RESET, info->release);
		printf(VERDE_T"Version: %s\n"RESET, info->version);
		printf(VERDE_T"Machine: %s\n"RESET, info->machine);
	}
}

void cmdAyuda() {
	if (numtrozos == 1) {
		printf(CYAN_T "[+] autores " AZUL_T "[-l|-n]\n"
		CYAN_T"[+] pid "AZUL_T "[-p]\n"
		CYAN_T"[+] carpeta " AZUL_T"[direct]\n"
		CYAN_T"[+] fecha "AZUL_T "[-d|-h]\n"
		CYAN_T"[+] hist " AZUL_T"[-c|-N]\n"
		CYAN_T"[+] comando " AZUL_T"N\n"
		CYAN_T"[+] infosis\n"
		CYAN_T"[+] ayuda " AZUL_T"[cmd]\n"
		CYAN_T"[+] fin\n"
		CYAN_T"[+] salir\n"
		CYAN_T"[+] bye\n" RESET);
	}
	else if (numtrozos > 1) {
		if (strcmp(trozos[1], "autores") == 0)
			puts(AZUL_T"[+] Imprime los nombres y logins del programa autores. autores -l: imprime solo los logins y autores -n imprime solo los nombres."RESET);
		else if (strcmp(trozos[1], "pid") == 0)
			puts(AZUL_T"[+] Imprime el pid del proceso que ejecuta el shell. pid -p: imprime el pid del proceso padre del shell"RESET);
		else if (strcmp(trozos[1], "carpeta") == 0)
			puts(AZUL_T"[+] Cambia el directorio actual de trabajo del shell a la nueva dirección indicada. Si se invoca sin argumentos, devuelve el directorio actual"RESET);
		else if (strcmp(trozos[1], "fecha") == 0)
			puts(AZUL_T"[+] Sin argumentos imprime la fecha y la hora actual. fecha -d: imprime solo la fecha actual. fecha -h imprime solo la hora actual"RESET);
		else if (strcmp(trozos[1], "hist") == 0)
			puts(AZUL_T"[+] hist: Imprime una lista de todos los comandos que se hayan usado. hist -c: Borra el historial de comandos. hist -N: Imprime los primero N comandos."RESET);
		else if (strcmp(trozos[1], "comando") == 0)
			puts(AZUL_T"[+] Repite el comando numero N del historial de comandos"RESET);
		else if (strcmp(trozos[1], "infosis") == 0)
			puts(AZUL_T"[+] Imprime informacion sobre la maquina que está ejecutando el shell"RESET);
		else if (strcmp(trozos[1], "ayuda") == 0)
			puts(AZUL_T"[+] ayuda: muestra una lista de todos los comandos disponibles. ayuda -[cmd]: Muestra una breve descripcion sobre como usar el comando [cmd]"RESET);
		else if ((strcmp(trozos[1], "fin") == 0) || (strcmp(trozos[1], "salir") == 0) || (strcmp(trozos[1], "bye") == 0))
			puts(AZUL_T"[+] Finaliza el shell"RESET);
		else
			puts(ROJO_T"[+] Comando no reconocido ..."RESET);
	}
}

void cmdHist(tList *L) {
	int i=0, n;
	char* comando;
	tPosL pos;
	char *num[10];
	
	if (numtrozos == 1)
		imprimirLista(*L);
	else if (numtrozos == 2 && strcmp(trozos[1], "-c") == 0) {
		freeList(L);
	} 
	else if (numtrozos == 2) {
		num[0] = strtok(trozos[1],"-");
		n = atoi(num[0]) - 1;
		for (pos = first(*L); i <= n  && pos != last(*L); pos= next(pos, *L)) {
			comando = getItem(pos, *L);
			printf(VERDE_T"%d->%s\n"RESET, i, comando);
			i++;
		}
	}
}

void cmdCarpeta() {
	char ruta[MAXTROZOS];

	if (numtrozos == 1) {
		if (getcwd(ruta, MAXTROZOS) == NULL) {
			perror("getcwd"); 
		}
		printf("%s\n", ruta);
	}
	else if (chdir(trozos[1]) == -1) {
		perror("chdir"); 
	}
}

void cmdComando(tList *L) {
	int n = atoi(trozos[1]); 
	int i= 1;
	char* comando = NULL;
	comando = malloc(sizeof(char)*1024);

	tPosL pos;
	for(pos = first(*L); i <= n && pos != last(*L); pos = next(pos, *L)){
			i++;
	}
	i--;

	strncpy(comando, getItem(pos, *L), 1024);

	if (numtrozos == 1 || n > i) {
		printf(ROJO_T"Introduce un valor de índice válido de la lista\n" RESET);
	}


	numtrozos = TrocearCadena(comando, trozos);
	procesarComando(L);
	
	
} 

// -----------------------------------------------------------------------------
// --------------------------------- P1 ----------------------------------------
// -----------------------------------------------------------------------------


void cmdStat(){

	struct stat info;
	bool lg, link, acc;

	if (numtrozos == 1){
		cmdCarpeta();
	}else{
		for(int i=1; i<numtrozos; i++){

			stat(trozos[i], &info);
			if (strcmp(trozos[i],"-long") == 0){ lg = true; }
			if (strcmp(trozos[i],"-acc") == 0){ acc = true; }
			if (strcmp(trozos[i],"-link") == 0){ link = true; }

			if (lg){ printLongStats(acc, info); }
			printf("Caso general");
			if (link){ printf("parte del link"); }

		}
	}
}

void procesarComando(tList *L){
	for (int i = 0; ; i++) {
			if (cm_tabla[i].cm_nombre==NULL) {
				printf(ROJO_T"%s: comando no reconocido\n"RESET, trozos[0]);
				break;
			}
			else if (strcmp(cm_tabla[i].cm_nombre, trozos[0]) == 0) {
				cm_tabla[i].cm_fun(L);
				break;
			}
		}
}

cm_entrada cm_tabla[] = {
	{"autores", cmdAutores},
	{"pid", cmdPid},
	{"carpeta", cmdCarpeta},
	{"fecha", cmdFecha},
	{"hist", cmdHist},
	{"infosis", cmdInfosis},
	{"ayuda", cmdAyuda},
	{"stat", cmdStat},
	{"fin", cmdFin},
	{"salir", cmdFin},
	{"bye", cmdFin},
	{"comando", cmdComando},
	{NULL, NULL}
};
