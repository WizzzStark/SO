//AUTOR 1: Alberto Fernández Valiño - login: alberto.fernandezv
//AUTOR 2: Juan Villaverde Rodríguez - login: juan.villaverde.rodriguez
//Grupo 1.4 


#include "headers.h"

int printLongStats(bool opacc, struct stat st){
	struct passwd *p;
	struct group *g;
	struct tm *t;

	t = localtime((opacc) ? &st.st_atime : &st.st_mtime);
	char *permisos = ConvierteModo2(st.st_mode);
	permisos[0] = LetraTF(st.st_mode); 


	printf("%04d/%02d/%02d-", t->tm_year+1900, t->tm_mon+1, t->tm_mday);
	printf("%02d:%02d,", t->tm_hour, t->tm_min);
	printf("%2ld ", st.st_nlink);
	printf("(%lu)", st.st_ino);
	printf("\t%s", ((p = getpwuid(st.st_uid)) == NULL) ? "???" : p->pw_name);
	printf("\t%s", ((g = getgrgid(st.st_gid)) == NULL) ? "???" : g->gr_name);
	printf("\t%s", permisos);

	return 0;
}

char LetraTF (mode_t m){
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

char * ConvierteModo2 (mode_t m){
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

int cmdFin(tList *L) {
	deleteList(L);
	printf(ROJO_T "\n[!] Saliendo de la shell ...\n\n" RESET);
	exit(0);
	return 0;
}

int cmdAutores() {
	int flagL=1, flagN=1;
	
	if (numtrozos >=2 && strcmp(trozos[1], "-n") == 0) flagL=0;
	else if (numtrozos >= 2 && strcmp(trozos[1], "-l") == 0) flagN=0;

	if (flagN)
		printf(VERDE_T "[+] autor 1: Alberto Fernández\n[+] autor 2: Juan Villaverde\n" RESET);
	if (flagL)
		printf(VERDE_T"[+] login 1: alberto.fernandezv\n[+] login 2: juan.villaverde.rodriguez\n" RESET);
	
	return 0;
}

int cmdPid() {
	if (numtrozos == 1) {
		printf(VERDE_T"pid de shell: %d\n"RESET, getpid());
	}
	if (numtrozos >= 2 && strcmp(trozos[1], "-p") == 0) {
		printf(VERDE_T"pid del padre del shell: %d\n"RESET, getppid());
	}
	if (numtrozos >= 2 && strcmp(trozos[1], "-p") != 0) {
		printf(VERDE_T"pid de shell: %d\n"RESET, getpid());
	}
	return 0;
}

int cmdFecha() {
	int flagD = 1, flagH = 1;
	time_t tiempo;
	struct tm *t;
	
	if (numtrozos > 1 && strcmp(trozos[1], "-d") == 0) flagH = 0;
	if (numtrozos > 1 && strcmp(trozos[1], "-h") == 0) flagD = 0;
	
	if (time(&tiempo) == -1) {perror("tiempo"); }
	t = localtime(&tiempo);
	if (numtrozos > 1 && strcmp(trozos[1], "-d") != 0)
		if (numtrozos > 1 && strcmp(trozos[1], "-h") != 0)
			flagD = 0, flagH = 0;
		
	if (flagH)
		printf(VERDE_T"%02d:%02d:%02d\n"RESET, t->tm_hour, t->tm_min, t->tm_sec);
	if (flagD)
		printf(VERDE_T"%02d/%02d/%04d\n"RESET, t->tm_mday, t->tm_mon +1, t->tm_year + 1900);
	
	return 0;
}

int cmdInfosis() {
	struct utsname info[1];
	if (uname(info) == -1) {perror("uname"); }
	else {
		printf(VERDE_T"System name: %s\n"RESET, info->sysname);
		printf(VERDE_T"Node name: %s\n"RESET, info->nodename);
		printf(VERDE_T"Release: %s\n"RESET, info->release);
		printf(VERDE_T"Version: %s\n"RESET, info->version);
		printf(VERDE_T"Machine: %s\n"RESET, info->machine);
	}
	return 0;
}

int cmdAyuda() {
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
	return 0;
}

int cmdHist(tList *L) {
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
	return 0;
}

int cmdCarpeta() {
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
	return 0;
}

int cmdComando(tList *L) {
	int n = atoi(trozos[1]); 
	int i= 1;
	char* comando = NULL;
	comando = malloc(sizeof(char)*MAX_SIZE);

	tPosL pos;
	for(pos = first(*L); i <= n && pos != last(*L); pos = next(pos, *L)){
			i++;
	}
	i--;

	strncpy(comando, getItem(pos, *L), MAX_SIZE);

	if (numtrozos == 1 || n > i) {
		printf(ROJO_T"Introduce un valor de índice válido de la lista\n" RESET);
	}


	numtrozos = TrocearCadena(comando, trozos);
	procesarComando(L);
	return 0;
	
} 

// -----------------------------------------------------------------------------
// --------------------------------- P1 ----------------------------------------
// -----------------------------------------------------------------------------

int reca_func(tList *directorios, char* dir_actual){

	char path[1000];
	struct dirent *dir;
	DIR *d = opendir(dir_actual);

	if (d){

		if(isEmptyList(*directorios)){
			insertItem(dir_actual,first(*directorios),directorios);

		}else{
			insertItem(dir_actual,next(last(*directorios), *directorios),directorios);
		}

		while((dir = readdir(d)) != NULL){


			if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){

				strcpy(path, dir_actual);
				strcat(path, "/");
				strcat(path, dir->d_name);

				reca_func(directorios, path);
			}
		}
		closedir(d);
	}

	return 0;
}

// IMPORTANTE ACTUALIZAR COMANDO DE AYUDA CON LOS NUEVOS Y PONERLOS EN STRUCT
// Pendiente cuando se imprimer es link pone la ruta absoluta y no la relativa
int cmdStat(){

	struct stat info;
	bool lg=false, link=false, acc=false;
	char ruta[4096];

	if (numtrozos == 1){
		cmdCarpeta();
	}else{
		for(int i=1; i<numtrozos; i++){
	
			if (strcmp(trozos[i],"-long") == 0){ lg = true; }
			else if (strcmp(trozos[i],"-acc") == 0){ acc = true; }
			else if (strcmp(trozos[i],"-link") == 0){ link = true; }
		}

		for(int i=1; i<numtrozos; i++){
			char *cadena = trozos[i];
			int ascii = cadena[0];
			if(ascii ==  45){
				continue;
			}
			lstat(trozos[i], &info);
			if (lg){ printLongStats(acc, info); }		
			printf("%6ld %s", info.st_size, trozos[i]);
			if (link){
				if(LetraTF(info.st_mode)== 'l'){
					printf(" ->%s", (realpath(trozos[i], ruta) == NULL)?"":ruta); 
				}
			}
			printf("\n");
		}
	}
	return 0;
}

//falta recb
int cmdList(){
	bool lg=false, link=false, acc=false, hid=false, reca=false, recb=false;

	tList directorios;
	tPosL pos;

	char *path;
	char copia[MAX_SIZE];

	DIR *d, *d2;
	struct dirent *dir;

	char ruta[MAX_SIZE];
	struct stat info;

	bool primer_elemento = true;

	if (numtrozos == 1){
		cmdCarpeta();
	}else{
		for(int i=1; i<numtrozos; i++){
			if (strcmp(trozos[i],"-reca") == 0){ reca = true; recb = false; }
			else if (strcmp(trozos[i],"-recb") == 0){ recb = true; reca = false; }
			else if (strcmp(trozos[i],"-hid") == 0){ hid = true; }
			else if (strcmp(trozos[i],"-long") == 0){ lg = true; }
			else if (strcmp(trozos[i],"-link") == 0){ link = true; }
			else if (strcmp(trozos[i],"-acc") == 0){ acc = true; }
		}

		for(int i=1; i<numtrozos; i++){
			char *cadena = trozos[i];
			if(cadena[0] ==  45){ // control de que no sea una de las flags
				continue;
			}

			//IMPLEMENTAR LAS FUNCIONESNUEVAS A AYUDA

			if((d2 = opendir(trozos[i]))){

				if (reca){
					createEmptyList(&directorios);
					reca_func(&directorios, trozos[i]);
				//}else if (reca){
				//	createEmptyList(&directorios);
				//	reca_func(&directorios, trozos[i]);
				}else{
					createEmptyList(&directorios);
					if(isEmptyList(directorios)){
						insertItem(trozos[i],first(directorios),&directorios);
					}else{
						insertItem(trozos[i],next(last(directorios), directorios),&directorios);
					}
				}

				if (!isEmptyList(directorios)) {
					pos = first(directorios);
					while (pos != LNULL) {
						path = getItem(pos, directorios);
						if((d = opendir(path))){

							printf("************%s\n", path);
							while((dir = readdir(d)) != NULL){ 

								strcpy(copia, path);

								if (primer_elemento){
									lstat(dir->d_name, &info); 
								}else{							 
									strcat(copia, "/");
									strcat(copia, dir->d_name);
									lstat(copia, &info);
								}
					
								if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){

									if (lg){ printLongStats(acc, info); }	
									printf("%6ld %s\n", info.st_size, dir->d_name);
									if (link)
										if(LetraTF(info.st_mode)== 'l')
											printf(" ->%s", (realpath(dir->d_name, ruta) == NULL)?"":ruta); 	
								}

								if(hid){
									if (dir->d_name[0] == '.'){
										if (lg){ printLongStats(acc, info); }	
										printf("%6ld %s\n", info.st_size, dir->d_name);
									}
								}

							}
							pos = next(pos, directorios);
							closedir(d);
							primer_elemento = false;
						}
					}
				}
				deleteList(&directorios);
				closedir(d2);
			}else{
				perror("opendir");
			}
		}
	}
	return 0;
}

int cmdCreate(){

	if (numtrozos == 1){
		cmdCarpeta();
	}else{
		char path[MAX_SIZE];
		bool fichero = false;

		getcwd(path, sizeof(path));
		strcat(path, "/");

		for(int i=1; i<numtrozos; i++)
			if (strcmp(trozos[i],"-f") == 0){ fichero = true; }

		if (fichero){ // fichero
			char *nombre = trozos[2];
			if(creat(strcat(path, nombre), 0666) == -1)
				perror("creat");
		}else{ //directorio
			char *nombre = trozos[1];
			if (mkdir(strcat(path, nombre), 0755) == -1){
				perror("mkdir");
			}
		}
	}
	return 0;
}

int cmdBorrar(){

	if (numtrozos == 1){
		cmdCarpeta();
	}else{
		for(int i=1; i<numtrozos; i++){
			if(remove(trozos[i]) != 0){
				perror("remove");
			}
		}
	}
	return 0;
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
	{"list", cmdList},
	{"create", cmdCreate},
	{"delete", cmdBorrar},
	{"fin", cmdFin},
	{"salir", cmdFin},
	{"bye", cmdFin},
	{"comando", cmdComando},
	{NULL, NULL}
};
