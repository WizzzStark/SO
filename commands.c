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

int cmdFin(tList *L, tList *mallocs, tList *shared, tList *mmaps) {
	freeList(L, free);
	freeList(mallocs, free);
	freeList(shared, free);
	freeList(mmaps, free);

	printf(ROJO_T "\n[!] Saliendo de la shell ...\n\n" RESET);
	condicion = false;
	return 1;
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
		CYAN_T"[+] carpeta " AZUL_T"<path>\n"
		CYAN_T"[+] fecha "AZUL_T "[-d|-h]\n"
		CYAN_T"[+] hist " AZUL_T"[-c|-N]\n"
		CYAN_T"[+] comando " AZUL_T"N\n"
		CYAN_T"[+] infosis\n"
		CYAN_T"[+] ayuda " AZUL_T"<cmd>\n"
		CYAN_T"[+] fin\n"
		CYAN_T"[+] salir\n"
		CYAN_T"[+] bye\n" 
		CYAN_T"[+] stat " AZUL_T"[-long] [-acc] [-link]\n"
		CYAN_T"[+] list " AZUL_T"[-long] [-acc] [-link] [-hid] [-reca] [-recb]\n"
		CYAN_T"[+] create " AZUL_T"[-f] <path1> <path2>...\n"
		CYAN_T"[+] delete " AZUL_T"<path1> <path2>...\n"
		CYAN_T"[+] malloc " AZUL_T"\n"
		CYAN_T"[+] shared " AZUL_T"\n"
		CYAN_T"[+] allocate " AZUL_T"\n"
		CYAN_T"[+] deallocate " AZUL_T"\n"
		CYAN_T"[+] mmap " AZUL_T"\n"
		CYAN_T"[+] pmap " AZUL_T"\n"
		CYAN_T"[+] i-o " AZUL_T"\n"
		CYAN_T"[+] recursiva " AZUL_T"\n"
		CYAN_T"[+] memory " AZUL_T"\n"
		CYAN_T"[+] memfill " AZUL_T"\n"
		CYAN_T"[+] memdump " AZUL_T"\n"
		CYAN_T"[+] deltree " AZUL_T"<path1> <path2>...\n"RESET);
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
		freeList(L, free);
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

bool isANumber(char * cadena) {
	int longitudTrozos1 = strlen(cadena);

	//printf("LONG: %d", longitudTrozos1);
	for (int x = 0; x < longitudTrozos1; x++) {
		if (!isdigit(trozos[1][x])) {
			return false;
		}
	}
	return true;
}

int cmdComando(tList *L, tList *mallocs, tList *shared, tList *mmap) {
	int n = atoi(trozos[1]); 
	int i= 1;
	char* comando = NULL;
	comando = malloc(sizeof(char)*MAX_SIZE);

	tPosL pos;

	if (!isANumber(trozos[1])) {
		printf(ROJO_T"Introduce un valor de índice válido de la lista (numerico) \n" RESET);
    	free(comando);
		return 0;
	}


	if (!isEmptyList(*L)) {
		for(pos = first(*L); i <= n && pos != last(*L); pos = next(pos, *L)){
			i++;
		}
		i--;

		strncpy(comando, getItem(pos, *L), MAX_SIZE);

		if (numtrozos == 1 || n > i) {
			printf(ROJO_T"Introduce un valor de índice válido de la lista\n" RESET);
			free(comando);
			return 0;
		}

		numtrozos = TrocearCadena(comando, trozos);
    	free(comando);
		procesarComando(L, mallocs, shared, mmap);
	}
	else {
		printf(ROJO_T"La lista no tiene comandos\n"RESET);
    	free(comando);
	}
	
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
		char* lineaReservada = malloc(sizeof(char)*1024);
		strcpy(lineaReservada, dir_actual);
		insertItem(lineaReservada, directorios);

		while((dir = readdir(d)) != NULL){

			if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){

				strcpy(path, dir_actual);
				strcat(path, "/");
				strcat(path, dir->d_name);

				reca_func(directorios, path);
			}
		}
		if (closedir(d) == -1) perror("closedir");
		
	}

	return 0;
}

int recb_func(tList *directorios, char* dir_actual) {
	char path[1000];
	struct dirent *dir;
	DIR *d = opendir(dir_actual);

	if (d){
		while((dir = readdir(d)) != NULL){

			if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){

				strcpy(path, dir_actual);
				strcat(path, "/");
				strcat(path, dir->d_name);

				recb_func(directorios, path);
			}
		}
		if (closedir(d) == -1) perror("closedir");

		char* lineaReservada = malloc(sizeof(char)*1024);
		strcpy(lineaReservada, dir_actual);
		insertItem(lineaReservada, directorios);
	}

	return 0;

}

int delRec(char * path) {
	struct dirent *dir;
	DIR *d;
	char pathCopy[1000];

	d = opendir(path);

	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
				strcpy(pathCopy, path);
				strcat(pathCopy, "/");
				strcat(pathCopy, dir->d_name);
				delRec(pathCopy);
			}
		}
		rmdir(path);
		if (closedir(d) == -1) perror("closedir");

	}
	else {
		if(unlink(path) == -1) perror("unlink");
	}

	return 0;
}

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
			if (lstat(trozos[i], &info) == -1) perror("lstat");
			else {
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
	}
	return 0;
}

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

			if((d2 = opendir(trozos[i]))){

				if (reca) {
					createEmptyList(&directorios);
					reca_func(&directorios, trozos[i]);
				}else if (recb){
					createEmptyList(&directorios);
					recb_func(&directorios, trozos[i]);
				}else{
					createEmptyList(&directorios);
					char* lineaReservada = malloc(sizeof(char)*1024);
					strcpy(lineaReservada, trozos[i]);
					insertItem(lineaReservada, &directorios);
				}

				if (!isEmptyList(directorios)) {
					pos = first(directorios);
					while (pos != LNULL) {
						path = getItem(pos, directorios);
						
						if ((reca || recb) && (strlen(path) >= 3)) {
							if (path[0] == '.' && path[1] == '/' && path[2] == '.' && !hid) {
								pos = next(pos, directorios);
								continue;
							}
						}
						
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
								if (dir->d_name[0] != '.'){
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
							if (closedir(d) == -1) perror("closedir");
							primer_elemento = false;
						}
						else {
							perror("opendir");
						}
					}
				}
				freeList(&directorios, free);
				if (closedir(d2) == -1) perror("closedir");
			}else{
				cmdStat();
			}
		}
	}
	return 0;
}

int cmdDelTree() {
	for (int i = 1; i < numtrozos; i++) 
		delRec(trozos[i]);

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

// -----------------------------------------------------------------------------
//------------------------------P2----------------------------------------------
// -----------------------------------------------------------------------------

// ------------------------ Memory functions ------------------------
int cmdMemory(tList *L, tList *mallocs, tList *shared, tList *mmap){
	if (numtrozos == 1 || strcmp(trozos[1], "-all") == 0){
		print_variables();
		print_funcs();
		imprimirTodos(*L, *mallocs, *shared, *mmap);
	}else if (strcmp(trozos[1], "-vars") == 0){
		print_variables();
	}else if (strcmp(trozos[1], "-blocks") == 0){
		imprimirTodos(*L, *mallocs, *shared, *mmap);
	}else if (strcmp(trozos[1], "-funcs") == 0){
		print_funcs();
	}else if (strcmp(trozos[1], "-pmap") == 0){
		cmdPmap();
	}
	return 0;
}

int gl1, gl2, gl3;
void print_variables(){
	static int st1, st2, st3;
	int i1,i2,i3;
	printf("Variables locales\t %p, \t %p, \t %p\n", &i1, &i2, &i3);
	printf("Variables globales\t %p, \t %p, \t %p\n", &gl1, &gl2, &gl3);
	printf("Variables locales\t %p, \t %p, \t %p\n", &st1, &st2, &st3);
}

void print_funcs(){
	printf("Funciones programa\t %p, \t %p, \t %p\n", cmdAutores, cmdInfosis, cmdFecha);
	printf("Funciones librería\t %p, \t %p, \t %p\n", printf, strcat, strcmp);
}

// ------------------------ Memfill y recursive functions ------------------------

void Recursiva (int n){
  char automatico[MAX_SIZE];
  static char estatico[MAX_SIZE];

  printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

  if (n>0)
    Recursiva(n-1);
}

int cmdRecursiva (){
	if (numtrozos==2){ // comprobar si es un numero el segundo argumento no me da la cabez aahora mismo
		int n = atoi(trozos[1]);
		Recursiva(n);
	}
	return 0;
}

// ------------------------ pmap function ------------------------
int cmdPmap (void) /*sin argumentos*/
 { pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
   char elpid[32];
   char *argv[4]={"pmap",elpid,NULL};
   
   sprintf (elpid,"%d", (int) getpid());
   if ((pid=fork())==-1){
      perror ("Imposible crear proceso");
      return 0;
      }
   if (pid==0){
      if (execvp(argv[0],argv)==-1)
         perror("cannot execute pmap (linux, solaris)");
         
      argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;   
      if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
         perror("cannot execute procstat (FreeBSD)");
         
      argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;    
            if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
         perror("cannot execute procmap (OpenBSD)");
         
      argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
      if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
         perror("cannot execute vmmap (Mac-OS)");      
      exit(1);
  }
  waitpid (pid,NULL,0);

  return 0;
}

// ------------------------ Print lists functions ------------------------
void imprimirTodos(tList L, tList mallocs, tList shared, tList mmap){
	printf("******Lista de bloques asignados para el proceso %d\n", getpid());
	imprimirAllocations(mallocs, "malloc", false);
	imprimirAllocations(shared, "shared", false);
	imprimirAllocations(mmap, "mmap", false);
}

void imprimirAllocations(tList list, char* allocation_type, bool tag) {
	tPosL pos;
	void *data;
    tAllocData *allocData;
	tSharedData *sharedData; 
    tMmapData *mmapData;

	if(tag) printf("******Lista de bloques asignados %s para el proceso %d\n", allocation_type, getpid());
	if (!isEmptyList(list)) {
		pos = first(list);
		while (pos != LNULL) {
			data = getItem(pos, list);

			if (strcmp(allocation_type, "malloc") == 0){
                allocData = data;
				printf("\t%s\t\t%d %s %s\n",allocData->allocation, allocData->size, allocData->date, allocation_type);
			} else if (strcmp(allocation_type, "shared") == 0){
				sharedData = data;
				printf("\t%s\t\t%d %s %s (key %d)\n",sharedData->allocation, sharedData->size, sharedData->date, allocation_type, sharedData->key);
			} else if (strcmp(allocation_type, "mmap") == 0){
                mmapData = data;
				printf("\t%s\t\t%d %s %s (%d)\n",mmapData->allocation, mmapData->size, mmapData->date, mmapData->file_name, mmapData->file_descriptor);
			}

			pos = next(pos, list);
		}
	}
	
}

// ------------------------ Malloc functions ------------------------
int mallocFree(int size, tList *mallocs) {
	tPosL pos;

	if ((pos = findAllocDataBySize(size, *mallocs)) != LNULL) {
		removeItem(pos, mallocs);
	}

	return 0;
}

int cmdMalloc(tList *L, tList *mallocs) {
	time_t mallocTime;
    tAllocData allocData;
	char * allocationAddress;

	if (numtrozos == 1 || (numtrozos == 2 && strcmp(trozos[1], "-free") == 0)){
		imprimirAllocations(*mallocs, "malloc", true);
	}else {
		if (strcmp(trozos[1], "-free") == 0) mallocFree(atoi(trozos[2]), mallocs);
		else{
			allocationAddress = malloc(atoi(trozos[1]));
			if (allocationAddress == NULL) {perror("malloc"); return 0;}

			if (time(&mallocTime) == -1) {perror("time"); return 0;}

			allocData.size = atoi(trozos[1]);
			sprintf(allocData.allocation, "%p", allocationAddress);
			strcpy(allocData.date, ctime(&mallocTime));

			printf("----------------------------------\n");
			printf("Tamaño: %d\n",allocData.size);
			printf("Dirección: %s\n",allocData.allocation);
			quitarSalto(allocData.date);
			printf("Fecha: %s\n",allocData.date);
			printf("AllocationType: malloc\n");

			insertAllocData(allocData, mallocs);

			printf("Asignados %d bytes en %p\n", atoi(trozos[1]), allocationAddress);
			printf("----------------------------------\n");

			free(allocationAddress);
		}
	}
	return 0;
}
// ------------------------------------------------------------------

// ------------------------ Shared functions ------------------------
void do_DeallocateDelkey (int keyInt){
   key_t clave;
   int id;
   char key[50];
   sprintf(key, "%d", keyInt);

   if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("      delkey necesita clave_valida\n");
        return;
   }
   if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return;
   }
   if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar memoria compartida\n");
}

void do_AllocateCreateshared (char *tr[], tList *shared){
	key_t cl;
	size_t tam;
	void *p;

	if (tr[2]==NULL || tr[3]==NULL) {
		imprimirAllocations(*shared, "shared", true);
		return;
	}
	
	cl=(key_t)  strtoul(tr[1],NULL,10);
	tam=(size_t) strtoul(tr[2],NULL,10);
	if (tam==0) {
		printf ("No se asignan bloques de 0 bytes\n");
		return;
	}
   	if ((p=ObtenerMemoriaShmget(cl,tam, shared))!=NULL){
		printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
   	}else{
		printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
	}
}

void * ObtenerMemoriaShmget (int clave, size_t tam, tList *shared){
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;

	tSharedData sharedData;
	time_t sharedTime;

    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL;
    if (clave==IPC_PRIVATE)  /*no nos vale*/
        {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*)-1){
        aux=errno;
        if (tam)
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);

	if (time(&sharedTime) == -1) {perror("time");}
	sprintf(sharedData.allocation, "%p", p);
	sharedData.size = s.shm_segsz;
	sharedData.key = clave;
	strcpy(sharedData.date, ctime(&sharedTime));
	quitarSalto(sharedData.date);

	insertSharedData(sharedData,shared);

 /* Guardar en la lista   InsertarNodoShared (&L, p, s.shm_segsz, clave); */
    return (p);
}

int freeShared(int key, tList *shared) {
	tPosL pos;
	if ((pos = findSharedDataByKey(key, *shared)) != LNULL) {
		removeItem(pos, shared);
	}

	return 0;
}

int cmdShared(tList *L, tList *mallocs, tList *shared){
	void* p;

	if (numtrozos == 1){
		imprimirAllocations(*shared, "shared", true);
	}else{
		if (strcmp(trozos[1], "-free") == 0) freeShared(atoi(trozos[2]), shared);
		else if(strcmp(trozos[1], "-create") == 0){
			do_AllocateCreateshared(trozos, shared);
		}
		else if(strcmp(trozos[1], "-delkey") == 0){
			do_DeallocateDelkey(atoi(trozos[2]));
			return 0;
		}else{
			int clave = atoi(trozos[1]);
			if((p=ObtenerMemoriaShmget(clave,0, shared))!=NULL){
				printf ("Memoria compartida de clave %d en %p\n",clave, p);
			}else{
				printf ("Imposible asignar memoria compartida clave %d:%s\n",clave,strerror(errno));
			}
			return 0;
		}
	}
	return 0;
}

// ------------------------ Mmap functions --------------------------
ssize_t LeerFichero (char *f, void *p, size_t cont){
   struct stat s;
   ssize_t  n;  
   int df,aux;

   if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
	return -1;     
   if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
	cont=s.st_size;
   if ((n=read(df,p,cont))==-1){
    perror("READ");
	aux=errno;
	close(df);
	errno=aux;
	return -1;
   }
   close (df);
   return n;
}

void * MapearFichero (char * fichero, int protection, int *df){
    int map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;

    if (protection&PROT_WRITE)
          modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (*df=open(fichero, modo))==-1) {
        return NULL;
    }
    if ((p=mmap (NULL,s.st_size, protection,map,*df,0))==MAP_FAILED) 
        return NULL;
    return p;
}

void * removeMmap(tPosL pos, tList *list) {
    tMmapData *mmapData;
    void *p;

	mmapData = getItem(pos, *list);
    p = (void *) strtoul(mmapData -> allocation, NULL, 16);

	if (munmap(p, mmapData -> size) == -1) {perror("munmap"); return 0;}
    close(mmapData -> file_descriptor);

    removeItem(pos, list);

	return p;
}

int freeMmap(char * fichero, tList *list) {
    tPosL pos;
	void *p;

	if ((pos = findMmapDataByFileName(fichero, *list)) != LNULL) {
		p = removeMmap(pos, list);
        printf("Fichero %s unmapeado en %p\n", trozos[2], p);
	} else printf("El archivo no existe o no fue mapeado\n");
    
    return 0;
}

int cmdMmap(tList *L, tList *mallocs , tList *shared, tList *maps) {
    int protection = 0;
    void *p;
    struct stat s;
	time_t mmapTime;
    tMmapData mmapData;
    int df = 0;

    if (numtrozos == 1 || (numtrozos == 2 && strcmp(trozos[1], "-free") == 0)) imprimirAllocations(*maps, "mmap", true);
    else if (numtrozos == 3 && strcmp(trozos[1], "-free") == 0) freeMmap(trozos[2], maps);
    else {
        if (numtrozos == 3) {
            if (strchr(trozos[2],'r')!=NULL) protection|=PROT_READ;
            if (strchr(trozos[2],'w')!=NULL) protection|=PROT_WRITE;
            if (strchr(trozos[2],'x')!=NULL) protection|=PROT_EXEC;
        }

        if ((p=MapearFichero(trozos[1],protection, &df))==NULL) {
            perror ("Imposible mapear fichero");
            return 0;
        }

        if (time(&mmapTime) == -1) {perror("time"); return 0;}
        if (stat(trozos[1], &s) == -1) {perror("Stat"); return 0;}

	    sprintf(mmapData.allocation, "%p", p);
	    mmapData.size = s.st_size;
        strcpy(mmapData.date, ctime(&mmapTime));
        mmapData.file_descriptor = df;
        strcpy(mmapData.file_name, trozos[1]);

        printf("----------------------------------\n");
        quitarSalto(mmapData.date);

	    printf("Tamaño: %d\n",mmapData.size);
	    printf("Dirección: %s\n",mmapData.allocation);
	    printf("Fecha: %s\n",mmapData.date);
	    printf("Descriptor: %d\n", mmapData.file_descriptor);
	    printf("Nombre del archivo: %s\n", mmapData.file_name);
	    printf("AllocationType: mmap\n");

        insertMmapData(mmapData, maps);
            
        printf ("fichero %s mapeado en %p\n", trozos[1], p);
	    printf("----------------------------------\n");
    }
    return 0;
}

// ------------------------ Allocate/deallocate functions ------------------------
int cmdAllocate(tList *L, tList *mallocs, tList *shared, tList *mmap) {
	imprimirTodos(*L, *mallocs, *shared, *mmap);
	return 0;
}

int cmdDeallocate(tList *L, tList *mallocs, tList *shared, tList *mmap) {
	tPosL pos;

	if (numtrozos == 1) imprimirTodos(*L, *mallocs, *shared, *mmap);
	if (numtrozos == 2) {
		if ((pos = findAllocData(trozos[1], *mallocs))!= LNULL) removeItem(pos, mallocs);
		else if ((pos = findSharedData(trozos[1], *shared))!= LNULL) removeItem(pos, shared);
		else if ((pos = findMmapData(trozos[1], *mmap))!= LNULL) removeMmap(pos, mmap);

	}
	if (numtrozos == 3) {
		if (strcmp(trozos[1], "-malloc") == 0) mallocFree(atoi(trozos[2]), mallocs);
		if (strcmp(trozos[1], "-shared") == 0) freeShared(atoi(trozos[2]), shared);
		if (strcmp(trozos[1], "-delkey") == 0) do_DeallocateDelkey(atoi(trozos[2]));
		if (strcmp(trozos[1], "-mmap") == 0) freeMmap(trozos[2], mmap);
	}

	return 0;
}
// ------------------------------------------------------------------

void LLenarMemoria(void *p, size_t cont, unsigned char byte) {
	unsigned char *arr = (unsigned char * ) p;
	ssize_t i;
	
	for (i = 0; i < cont; i++) 
		arr[i] = byte;
}

int cmdMemFill() {
	if (numtrozos == 4) {
		void *p = (void*) strtoul(trozos[1], NULL, 16);
		int cont = atoi(trozos[2]);
		unsigned char byte;
		char * caracter = malloc(sizeof(char));

		if ((byte = atoi(trozos[3])) == 0) {
			sprintf(caracter, "%d", trozos[3][1]);
			byte = (unsigned char) atoi(caracter);
		}

		LLenarMemoria(p, cont, byte);
		printf("LLenando %d bytes de memoria con el byte %c(%x) a partir de la direccion %p\n", cont, byte, byte, p);
		free(caracter);
}

	return 0;
}

int cmdMemDump() {
    void *p = (void*) strtoul(trozos[1],NULL,16);
	void *chars = p;
	int n = 25;
	int cnt = 25;
	if(numtrozos==3){
		cnt = atoi(trozos[2]);
	}

	for (int i = 0; i < n; i++) {
		printf(" %c ", *(char *)chars);
		chars++;
		if (--cnt == 0) {
			puts("");
			for (int j = 0; j < i+1; j++) {
				printf("%02X ", *(char *)p);
				p++;
			}
			break;
		}
		if (i == 24) {
			puts("");
			for (int j = 0; j < n; j++) {
				printf("%02X ", *(char *)p);
				p++;
			}
			puts("");
			i = -1;
			chars = p;
		}
	}
	puts("");

	return 0;

}

ssize_t EscribirFichero (char *f, char *addr, size_t cont, bool overwrite) {
	void *p = (void*) strtoul(addr, NULL, 16);

   	ssize_t  n;
   	int df,aux, flags=O_CREAT | O_EXCL | O_WRONLY;

   	if (overwrite)
		flags=O_CREAT | O_WRONLY | O_TRUNC;

  	if ((df=open(f,flags,0777))==-1)
		return -1;

    if ((n=write(df,p,cont))==-1){
		aux=errno;
		close(df);
		errno=aux;
		return -1;
    }
    close (df);
    return n;
}

void do_I_O_read () {
	size_t cont=-1;
	ssize_t n;

	if (trozos[2]==NULL || trozos[3]==NULL){
		printf ("faltan parametros\n");
		return;
	}

	void *p = (void*) strtoul(trozos[3], NULL, 16);

	if (trozos[4]!=NULL)
		cont=(size_t) atoll(trozos[4]);

	if ((n=LeerFichero(trozos[2],p,cont))==-1)
		perror ("Imposible leer fichero");
	else
		printf ("leidos %lld bytes de %s en %p\n",(long long) n,trozos[2],p);
}


void do_I_O_write() {
	if (strcmp(trozos[2], "-o") == 0) EscribirFichero(trozos[3], trozos[4], atoi(trozos[5]), true);
	else EscribirFichero(trozos[2], trozos[3], atoi(trozos[4]), false);
}

int cmdIO() {
	if (strcmp("read", trozos[1]) == 0) do_I_O_read();
	else do_I_O_write();
	return 0;
}

int cmdCreateShared(tList *L, tList *mallocs, tList *shared) {
	do_AllocateCreateshared(trozos, shared);

	return 0;
}


void procesarComando(tList *L, tList *mallocs, tList *shared, tList *mmap){
		if (strcmp(trozos[0], "ayuda") == 0 && numtrozos > 1) {
			for (int i = 0; ;i++) {
				if (cm_tabla[i].cm_nombre==NULL) {
					printf(ROJO_T"%s: comando no reconocido\n"RESET, trozos[1]);
					break;
				}
				else if (strcmp(cm_tabla[i].cm_nombre, trozos[1]) == 0) {
					printf(AZUL_T"%s\n"RESET, cm_tabla[i].ayuda);
					break;
				}
			}
		}
		else if (strcmp(trozos[0], "allocate") == 0 && numtrozos > 1) {
			for (int i = 0; ;i++) {
				if (alloc_tabla[i].cm_nombre==NULL) {
					printf(ROJO_T"%s: uso: allocate [-malloc|-shared|-createshared|-mmap] ....\n"RESET, trozos[1]);
					break;
				}
				else if (strcmp(alloc_tabla[i].cm_nombre, &trozos[1][1]) == 0) {
					if (numtrozos >= 2) trozos[0] = &trozos[1][1];
					if (numtrozos >= 3) 
						for (int x = 1; x < numtrozos-1; x++) trozos[x] = trozos[x+1];
					
					numtrozos = numtrozos - 1;
					alloc_tabla[i].cm_fun(L, mallocs, shared, mmap);
					break;
				}
			}
		}
		else {
			for (int i = 0; ;i++) {
				if (cm_tabla[i].cm_nombre==NULL) {
					printf(ROJO_T"%s: comando no reconocido\n"RESET, trozos[0]);
					break;
				}
				else if (strcmp(cm_tabla[i].cm_nombre, trozos[0]) == 0) {
					cm_tabla[i].cm_fun(L, mallocs, shared, mmap);
					break;
				}
			}
		}
}

// -----------------------------------------------------------------------------
//------------------------------P3----------------------------------------------
// -----------------------------------------------------------------------------

short file_exists(char *filename)
{
  int fd=open(filename, O_RDONLY);
  if (fd==-1)
    {
      if (errno==2)             /* If errno==2 it means file not found */
        return 0;               /* otherwise there is another error at */
      else                      /* reading file, for example path not  */
        return -1;              /* found, no memory, etc */
    }
  close(fd);                    /* If we close the file, it exists */
  return 1;
}

int findInPath(char *result, char *executable)
{
  char *path = getenv("PATH");
  char *saveptr;
  char *tmpstr = malloc(strlen(path)+strlen(executable)+2);
  char *directory = strtok_r(path, ":", &saveptr);
  char *slash = "/";
  short found = 0;
  while ( (directory != NULL) && (!found) )
    {
      sprintf (tmpstr, "%s%s%s", directory, (directory[strlen(directory)-1]=='/')?"":slash, executable);
      if (file_exists(tmpstr))
    found = 1;
      directory = strtok_r(NULL, ":", &saveptr);
    }

  if (found)
    strcpy(result, tmpstr);

  free(tmpstr);

  return found;
}

int cmdPriority () {
	int prio;
	errno = 0;

	if (numtrozos == 1) {
		prio = getpriority(PRIO_PROCESS, getpid());
		if (errno == EACCES) {perror("getpriority"); return 0;}
		printf("Prioridad del proceso %d: %d\n",getpid() ,prio);
	}
	else if (numtrozos == 3){
		prio = setpriority(PRIO_PROCESS, atoi(trozos[1]), atoi(trozos[2]));
		if (errno == EACCES) {perror("setpriority"); return 0;}
	}
	else if (numtrozos == 2){
		prio = getpriority(PRIO_PROCESS, atoi(trozos[1]));
		if (errno == EACCES) {perror("getpriority"); return 0;}
		printf("Prioridad del proceso %d: %d\n",atoi(trozos[1]) ,prio);
	}

	return 0;
}

int cmdShowenv(){
	if (numtrozos == 1){
		for (int i = 0; __environ[i]; i++){
			printf("0x%p->main arg3[%d]=(0x%p) %s\n", &__environ[i], i, __environ[i], __environ[i]);
		}
	}
	return 0;
}

int cmdShowvar(){
    // Si no se especifica una variable, se muestran todas las variables
	if (numtrozos == 1){
		cmdShowenv();
	}
    // Si se especifica una variable, se muestra solo su valor y dirección
    else{
		char *var = trozos[1];

		// Obtiene el valor de la variable de entorno a través del tercer argumento de main()
		printf("Con arg3 main %s=%s(0x%p) @0x%p\n", var, getenv(var), getenv(var), &__environ[0]);

		// Obtiene el valor de la variable de entorno a través de la variable global environ
		extern char **environ;
		char **env = environ;
		while (*env){
			if (strncmp(*env, var, strlen(var)) == 0)
			{
				printf(" Con environ %s(0x%p) @0x%p\n", *env, *env, env);
				break;
			}
			env++;
		}
    // Obtiene el valor de la variable de entorno a través de la función getenv()
    printf("  Con getenv %s(0x%p)\n", getenv(var), getenv(var));
	}
	return 0;
}

int cmdChangevar(){
    // Comprueba si se especifica un modo válido
    if (strcmp(trozos[1], "-p") != 0 && strcmp(trozos[1], "-e") != 0 && strcmp(trozos[1], "-a") != 0){
        printf("Uso: cambiarvar [-a|-e|-p] var valor\n");
		return 1;
    }

    // Cambia el valor de la variable de entorno según el modo especificado
    if (strcmp(trozos[1], "-a") == 0){
        // Cambia el valor de la variable de entorno a través del tercer argumento de main()
        int len = strlen(trozos[2]) + strlen(trozos[3]) + 2;
        char *env = malloc(len);
        snprintf(env, len, "%s=%s", trozos[2], trozos[3]);
        putenv(env);
    }
    else if (strcmp(trozos[1], "-e") == 0){
        // Cambia el valor de la variable de entorno a través de la variable global environ
        char **env = __environ;
        while (*env)
        {
            if (strncmp(*env, trozos[2], strlen(trozos[2])) == 0)
            {
                *env = malloc(strlen(trozos[2]) + strlen(trozos[3]) + 2);
                snprintf(*env, strlen(trozos[2]) + strlen(trozos[3]) + 2, "%s=%s", trozos[2], trozos[3]);
                break;
            }
            env++;
        }
    }
    else if (strcmp(trozos[1], "-p") == 0)
    {
        // Cambia el valor de la variable de entorno a través de la función putenv()
        int len = strlen(trozos[2]) + strlen(trozos[3]) + 2;
        char *env = malloc(len);
        snprintf(env, len, "%s=%s", trozos[2], trozos[3]);
        putenv(env);
    }
	return 0;
}

int cmdFork(){
    pid_t pid = fork();

    if (pid == 0){
        // Este es el proceso hijo
        printf("Ejecutando proceso %d\n", getpid());
    }
    else if (pid > 0){
        // Este es el proceso padre
        wait(NULL);
    }
    else{
        // fork() ha fallado
        printf("Failed to create child process\n");
    }

    return 0;
}

int cmdExecute() {

	int i = 1, x = 0;
	bool useEnviron = true;

	char **args3 = malloc(sizeof(char *) * numtrozos);
	char **args2 = malloc(sizeof(char *) * numtrozos);

	char * environVar = malloc(100);
	char copia[1000];
	char path[2000];

	extern char **environ;

	for (int j = 0; j < numtrozos; j++) {
		args3[j] = malloc(2000);
		args2[j] = malloc(2000);
	}

	printf("%d\n", numtrozos);

	strcpy(environVar, trozos[1]);
	while (getenv(environVar) != NULL) {
		useEnviron = false;
		strcpy(copia, trozos[i]);
		strcat(copia, "=");
		strcpy(args3[i-1], strcat(copia, getenv(environVar)));
		i++;
		if (i != numtrozos) strcpy(environVar, trozos[i]);
		else break;
	}
	args3[i-1] = NULL;
	
	printf("%s\n", trozos[1]);
	if (findInPath(path, trozos[i])) {
    	printf ("Found at: %s\n", path);
    }
    else printf ("Not found!\n"); 

	strcpy(args2[x], path);
	x++;
	i++;
	while(i < numtrozos) {
		if (trozos[i][0] == '@') {
			if (setpriority(PRIO_PROCESS, getpid(), atoi(&trozos[i][1])) == -1) {perror("setpriority"); return 0;}
			break;
		}
		strcpy(args2[x], trozos[i]);
		i++;
		x++;
		
	}
	args2[x] = NULL;
	
	//if (setenv("SHELL", path, 1) == -1) {perror("setenv"); return 0;}
	if (setenv("PATH", "/home/juan/software/idea/bin:/home/juan/software/apache-maven-3.8.6/bin:/home/juan/software/jdk-17.0.4.1+1/bin:/home/juan/software/idea/bin:/home/juan/software/apache-maven-3.8.6/bin:/home/juan/software/jdk-17.0.4.1+1/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin", 1) == -1) {perror("setenv"); return 0;}


	printf("%s\n", __environ[56]);

	if (useEnviron) {
		printf("ASDASD\n");
		if (execve(path, args2, __environ) == -1) {perror("execve"); return 0;}
	}
	else {
		if (execve("/bin/xterm", args2, args3) == -1) {perror("execve"); return 0;}
	}

	return 0;
}


cm_entrada cm_tabla[] = {
	{"autores", cmdAutores, "[+] autores: Imprime los nombres y logins del programa autores. autores -l: Imprime solo los logins. autores -n: Imprime solo los nombres."},
	{"pid", cmdPid, "[+] pid : Imprime el pid del proceso que ejecuta el shell. pid -p: Imprime el pid del proceso padre del shell."},
	{"carpeta", cmdCarpeta, "[+] carpeta: Devuelve el directorio actual. carpeta <path>: Cambia el directorio actual de trabajo del shell a <path>."},
	{"fecha", cmdFecha, "[+] fecha: Imprime la fecha y la hora actual. fecha -d: Imprime solo la fecha actual. fecha -h: Imprime solo la hora actual."},
	{"hist", cmdHist, "[+] hist: Imprime una lista de todos los comandos que se hayan usado. hist -c: Borra el historial de comandos. hist -N: Imprime los primero N comandos."},
	{"infosis", cmdInfosis, "[+] Imprime informacion sobre la maquina que está ejecutando el shell."},
	{"ayuda", cmdAyuda, "[+] ayuda: Muestra una lista de todos los comandos disponibles. ayuda <cmd>: Muestra una breve descripcion sobre como usar el comando <cmd>."},
	{"stat", cmdStat, "[+] stat: Devuelve el directorio actual.\n stat <path1> <path2>...: Da informacion sobre archivos o directorios.\n stat -long: Da mas informacion. stat -link: Tiene en cuenta los links simbolicos.\n stat -acc: Combinado con -long, muestra la fecha de ultimo acceso."},
	{"list", cmdList, "[+] \n list: Devuelve el directorio actual.\n list <path1> <path2>...: Muestra una lista con informacion sobre todos los archivos y directorios de <path>.\n list -long: Da mas informacion.\n list -link: Tiene en cuenta los links simbolicos.\n list -acc: Combinado con -long, muestra la fecha de ultimo acceso. list -reca: recursivo (antes).\n list -recb: recursivo (despues)\n"},
	{"create", cmdCreate, "[+] create: Crea un directorio vacio. create -f: Crea un fichero."},
	{"delete", cmdBorrar, "[+] borrar <path1> <path2>...: Borra <path> si es un fichero o un directorio vacio"},
	{"fin", cmdFin, "[+] Finaliza el shell."},
	{"salir", cmdFin, "[+] Finaliza el shell."},
	{"bye", cmdFin, "[+] Finaliza el shell."},
	{"comando", cmdComando, "[+] comando N: Repite el comando numero N del historial de comandos."},
	{"deltree", cmdDelTree, "[+] deltree <path1> <path2>...: Borra recursivamente archivos y directorios"},
	{"allocate", cmdAllocate, "[+] Asigna un bloque de memoria"},
	{"malloc", cmdMalloc, "[+] malloc [-free] [tam]	asigna un bloque memoria de tamano tam con malloc\n\t-free: desasigna un bloque de memoria de tamano tam asignado con malloc"},
	{"shared", cmdShared, "[+] shared [-free|-create|-delkey] cl [tam]	asigna memoria compartida con clave cl en el programa\n\t-create cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam\n\t-free cl: desmapea el bloque de memoria compartida de clave cl\n\t-delkey clelimina del sistema (sin desmapear) la clave de memoria cl"},
	{"mmap", cmdMmap, "[+] mmap [-free] fich prm	mapea el fichero fich con permisos prm\n\t-free fich: desmapea el ficherofich"},
    {"pmap", cmdPmap, "[+] Ejecuta pmap"},
	{"deallocate", cmdDeallocate, "[+] deallocate [-malloc|-shared|-delkey|-mmap|addr]..	Desasigna un bloque de memoria\n\t-malloc tam: desasigna el bloque malloc de tamano tam\n\t-shared cl: desasigna (desmapea) el bloque de memoria compartida de clave cl\n\t-delkey cl: elimina del sistema (sin desmapear) la clave de memoria cl\n\t-mmap fich: desmapea el fichero mapeado fich\n\taddr: desasigna el bloque de memoria en la direccion addr"},
	{"memdump", cmdMemDump, "[+] memdump addr cont 	Vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr"},
	{"recursiva", cmdRecursiva, "[+] recursiva [n]	Invoca a la funcion recursiva n veces"},
	{"memfill", cmdMemFill, "[+] memfill addr cont byte 	Llena la memoria a partir de addr con byte"},
	{"memory", cmdMemory, "[+] memory [-blocks|-funcs|-vars|-all|-pmap] ..	Muestra muestra detalles de la memoria del proceso\n\t-blocks: los bloques de memoria asignados\n\t-funcs: las direcciones de las funciones\n\t-vars: las direcciones de las variables\n\t:-all: todo\n\t-pmap: muestra la salida del comando pmap(o similar)"},
	{"i-o", cmdIO, "[+] i-o [read|write] [-o] fiche addr cont\n\tread fich addr cont: Lee cont bytes desde fich a addr\n\twrite [-o] fich addr cont: Escribe cont bytes desde addr a fich. -o para sobreescribir\n\taddr es una direccion de memoria"},
	{"priority", cmdPriority, "prioriza cosas"},
	{"showvar", cmdShowvar, "showvarea cosas"},
	{"changevar", cmdChangevar, "changevarea cosas"},
	{"showenv", cmdShowenv, "showenvea cosas"},
	{"fork", cmdFork, "forkea cosas"},
	{"execute", cmdExecute, "executea cosas"},
	{NULL, NULL}
};

cm_allocate alloc_tabla[] = {
	{"malloc", cmdMalloc},
	{"shared", cmdShared},
	{"mmap", cmdMmap},
	{"createshared", cmdCreateShared},
	{NULL, NULL}
};