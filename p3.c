/*
Autor 1: Pablo Fernandez Marti
Login 1: pablo.fernandez8@udc.es

Autor 2: Lucas Lopez Meijide
Login 2: lucas.lopez.meijide@udc.es

Autor 3: Nefer Labrada Suarez
Login 3: nefer.suarez@udc.es
*/

#include "p3.h" 
#include "globals.h"
#include "auxFunctions.h"

char dirUse[MAX_COMM];

hList HIS;
fList FILELIST;
memList MEMLIST;
dList DIRLIST;
pList PROCLIST;

// obtener el pid con una llamada al sistema
void Cmd_pid(char *tr[]) {
    printf("PID del proceso: %d\n", getpid());
}

// obtener el ppid con una llamada al sistema
void Cmd_ppid(char *tr[]) {
    printf("PID del proceso padre: %d\n", getppid());
}

//imprime los autores. Si se usa el -l, los logins. Si se usa el -n, los nombres. Si no, todo.
void Cmd_authors(char *tr[]) {
    if(tr[0] == NULL){
        printf("nefer.suarez@udc.es\n");
        printf("pablo.fernandez8@udc.es\n");
        printf("lucas.lopez.meijide@udc.es\n");
        printf("Nefer Labrada Suare\n");
        printf("Pablo Fernandez Marti\n");
        printf("Lucas Lopez Meijide\n");
    } else if (strcmp(tr[0], "-l") == 0) {
        printf("nefer.suarez@udc.es\npablo.fernandez8@udc.es\nlucas.lopez.meijide@udc.es\n");
    } else if (strcmp(tr[0], "-n") == 0) {
        printf("Nefer Labrada Suarez\nPablo Fernandez Marti\nLucas Lopez Meijide\n");
    } else { 
        printf("sso: el argumento para 'authors' no es válido. Intente (-l)(-n) \n");
    }
    
}

// Indica, con una llamada al sistema, el directorio actual. 
void Cmd_chdir(char *tr[]) {
    if (tr[0] == NULL) {
        if (getcwd(dirUse, MAX_COMM) != NULL) {
            printf("Directorio actual: %s\n", dirUse);
        } else {
            perror("Error obteniendo el directorio actual");
        }
    } else if (chdir(tr[0]) == -1) {
        perror("Ha sido imposible cambiar de directorio");
    }
}

// Sale del shell con una llamada al sistema.
void Cmd_exit(char *tr[]) {
    if (isEmptyFileList(FILELIST) && isEmptyHistoricList(HIS) && isEmptyMemList(MEMLIST)){
        exit(0);
    } else { // si quedan listas con objetos, las borra antes de salir
        clearHistoricList(&HIS);
        clearFileList(&FILELIST);
        clearMemoryList(&MEMLIST);
        clearDirList(&DIRLIST);
        clearProcessList(&PROCLIST);
        exit(0);
    } 
}

//obtiene la fecha actual mediante llamadas al sistema
void Cmd_date(char *tr[]) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    if (tr[0] == NULL || (strcmp(tr[0], "-?") == 0)){
        printf("uso: date [-d(ate)][-t(ime)]\n");
        return;
    } else if (strcmp(tr[0], "-d") == 0) { // time -d muestra la fecha
        printf("%02d/%02d/%d\n", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
    } else if (strcmp(tr[0], "-t") == 0) { // time -t muestra la hora
        printf("%02d:%02d:%02d\n", local->tm_hour, local->tm_min, local->tm_sec);
    } else { 
        printf("sso: el argumento para 'date' no es válido. Intente (-d)(-t) \n");
    }
}

// muestra el historial de comandos usados
void Cmd_history(char *tr[]){
    if (tr[0] == NULL) { // si solo se pone historic, imprime la lista
        printf("\nHistorial de comandos\n");
        printHistoricList(&HIS, 0);
    } else { // caso historic N
        int N = atoi(tr[0]);
        if (N > 0) {  // Comprobamos que no tenga - delante
            char *command = getHistoricItem(&HIS, N); 
            if (command != NULL) { // Mientras sea valido el comando
                printf("Repitiendo el comando número %d del historial: %s\n", N, command);
                char *args[MAX_COMM / 2];
                funcBreakLine(command, args);  
                DoCommand(args); // repetimos el comando con esta funcion
            }
        } else if (N < 0) { // Si tiene un - delante, imprimimos los ultimos N
            N = -N;  
            printf("\nMostrando los últimos %d comandos del historial\n", N);
            printHistoricList(&HIS, -N); 
        } else { 
            perror("Argumento no válido para 'historic' ");
        }
    }
}

// listado de las funciones y lo que hacen
void Cmd_help(char *tr[]) {
    if (tr == NULL || tr[0] == NULL) {
        printf("Lista de comandos disponibles:\n");
        for (int i = 0; comandos[i] != NULL; i++) {
            printf(" - %s\n", comandos[i]);
        }
        return;
    }
    if((strcmp(tr[0], "-?") == 0)){
        printf("uso: help [comando]\n");
        return;
    }
    for (int i = 0; comandos[i] != NULL; i++) {
        if (strstr(comandos[i], tr[0]) != NULL) {
            printf("%s\n", comandos[i]);
            return;
        }
    }
    printf("Comando '%s' no reconocido. Usa 'help' para ver la lista de comandos disponibles.\n", tr[0]);
}

// Imprime la informacion del sistema con una llamada al sistema
void Cmd_infosys(char *tr[]){
    struct utsname sysInfo;

    if (uname(&sysInfo)==-1){
        perror("Error al obtener la información del sistema:\n");
        return;
    }

    printf("\nInformacion del sistema\n\n");
    printf("Sistema operativo: %s\n",sysInfo.sysname);
    printf("Nombre del nodo: %s\n",sysInfo.nodename);
    printf("Version del sistema operativo: %s\n",sysInfo.release);
    printf("Version del kernel: %s\n",sysInfo.version);
    printf("Arquitectura de la maquina: %s\n",sysInfo.machine);
}

// Abre un archivo con un modo a especificar.
void Cmd_open(char *tr[]) {
    int df, mode = 0;
    char modo_str[50] = ""; 

    if (tr[0] == NULL) {
        printFiles(&FILELIST);
        return;
    }

    if (tr[1] == NULL) {
        printf("Error: Debe especificar al menos un modo de acceso. Use 'help' para más información.\n");
        return;
    }

    for (int i = 1; tr[i] != NULL; i++) {
        if (!strcmp(tr[i], "cr")) { mode |= O_CREAT; strcat(modo_str, "c"); }
        else if (!strcmp(tr[i], "ex")) { mode |= O_EXCL; strcat(modo_str, "e"); }
        else if (!strcmp(tr[i], "ro")) { mode |= O_RDONLY; strcat(modo_str, "r"); }
        else if (!strcmp(tr[i], "wo")) { mode |= O_WRONLY; strcat(modo_str, "w"); }
        else if (!strcmp(tr[i], "rw")) { mode |= O_RDWR; strcat(modo_str, "rw"); }
        else if (!strcmp(tr[i], "ap")) { mode |= O_APPEND; strcat(modo_str, "a"); }
        else if (!strcmp(tr[i], "tr")) { mode |= O_TRUNC; strcat(modo_str, "t"); }
        else { 
            printf("Error: Modo de acceso '%s' no válido.\n", tr[i]);
            return;
        }
    }

    if ((df = open(tr[0], mode, 0644)) == -1) {
        perror("Imposible abrir fichero");
    } else {
        insertFile(&FILELIST, df, tr[0], modo_str);
        printf("Archivo '%s' abierto con descriptor %d en modo %s.\n", tr[0], df, modo_str);
    }
}

// Duplica un comando por su df.
void Cmd_dup(char *tr[]) {
    int df, duplicado;

    if (tr[0] == NULL) {
        printFiles(&FILELIST);
        return;
    }

    if ((strcmp(tr[0], "-?") == 0)) {
        printf("uso: dup [descriptor]\n");
        return; 
    }

    df = atoi(tr[0]);
    if (df < 0) {
        printf("Error: Descriptor inválido. Debe ser un número no negativo.\n");
        return;
    }

    duplicado = dup(df);
    if (duplicado == -1) {
        perror("Error al duplicar el descriptor");
        return;
    }

    char *fileName = getFileName(&FILELIST, df);
    if (fileName == NULL) {
        printf("No se encontró el archivo a duplicar\n");
        close(duplicado); 
        return;
    }
    
    insertFile(&FILELIST, duplicado, fileName, "O_RDWR");
    printf("Descriptor %d duplicado. Nuevo descriptor: %d.\n", df, duplicado);
}

// Cierra los archivos abiertos
void Cmd_close(char *tr[]) {
    int df;
    if (tr[0] == NULL) {
        printFiles(&FILELIST);
        return;
    }
    if ((strcmp(tr[0], "-?") == 0)) {
        printf("uso: close [descriptor]\n");
        return; 
    }
    df = atoi(tr[0]);

    if (tr[0][0] < '0' || tr[0][0] > '9' || df < 0) {
        printf("Error: Descriptor inválido. Debe ser un número no negativo.\n");
        return;
    }
    
}

// cwd
void Cmd_cwd(char *tr[]){
    if (tr[0] == NULL) {
        if (getcwd(dirUse, MAX_COMM) != NULL) {
            printf("Directorio actual: %s\n", dirUse);
        }
    }
}

// crea un directorio
void Cmd_mkdir(char *tr[]){
    if (tr[0] == NULL || (strcmp(tr[0], "-?") == 0))
    {
      printf("uso: mkdir [nombre_fichero]\n");
      return;  
    }
    if (mkdir(tr[0], S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
        printf("Error al crear el directorio: %s\n", strerror(errno));
    }
}

//crea un archivo
void Cmd_mkfile (char *tr[]){
    int df;

    if (tr[0]==NULL || (strcmp(tr[0], "-?") == 0)){
        printf("uso: mkfile [nombre_fichero]");
        return;  
    }
    if ((df=open(tr[0],O_CREAT | O_RDWR, 0777))==-1){
        printf("Error al crear el fichero: %s\n", strerror(errno));
    }
}

//lista archivos
void Cmd_listfile(char *tr[])
{
    if (tr[0] == NULL || (strcmp(tr[0], "-?") == 0)) {
        printf("uso: listfile [-long] [-link] [-acc]\n");
        return;
    }
    int lng = 0, acc = 0, lnk = 0;
    char *directories[100];  //num directorios (por poner alguno)
    int dirCount = 0;
    processAndGetDirectory(tr,directories, &dirCount, NULL, &lng, &acc, &lnk);
    if (dirCount==0)
    {
        printf("sso: se debe especificar al menos un directorio.\n");
        return;
    }
    for (int i = 0;i<dirCount ; i++) {
        aux_listFilesInDir(directories[i], lng, acc, lnk);
    } 

}

//lista directorios
void Cmd_listdir(char *tr[]){
    int showHidden = 0, longFormat = 0, showAccessTime = 0, showLink = 0;
    char *directories[100];
    int dirCount = 0;

    if (tr[0] == NULL || (strcmp(tr[0], "-?") == 0)) {
        printf("uso: listdir [-hid][-long] [-link] [-acc]\n");
        return;
    }
    processAndGetDirectory(tr,directories, &dirCount, &showHidden, &longFormat, &showAccessTime, &showLink);

    //procesamos cada directorio
    for(int i=0; i<dirCount; i++){
        DIR *dir;
        struct dirent *entry;

        //verificar si el directorio ya esta abierto
        if((dir=opendir(directories[i]))==NULL){
            perror("opendir");
            continue;
        }
        printf("************%s\n",directories[i]);

        while ((entry=readdir(dir))!=NULL){
            aux_printDir(directories[i], entry, showHidden, longFormat, showAccessTime, showLink);
        }
    closedir(dir);
    }
}

// lista archivos y directorios recursivamente
void Cmd_reclist(char *tr[]){
    int hid = 0, lng = 0, acc = 0, lnk = 0;
    char *dirNum[100];
    int dirCount = 0;

    if (tr[0] == NULL || (strcmp(tr[0], "-?") == 0)) {
        printf("uso: reclist [-hid][-long] [-link] [-acc]\n");
        return;
    }
    processAndGetDirectory(tr, dirNum, &dirCount, &hid, &lng, &acc, &lnk);

    for (int i = 0; i < dirCount; i++) {
        aux_listDir(dirNum[i], hid, lng, acc, lnk, 0); // 0 para recList
    }
}

// igual pero en orden inverso
void Cmd_revlist(char *tr[]){
    int hid = 0, lng = 0, acc = 0, lnk = 0;
    char *dirNum[100];
    int dirCount = 0;
    if (tr[0] == NULL || (strcmp(tr[0], "-?") == 0)) {
        printf("uso: revlist [-hid][-long] [-link] [-acc]\n");
        return;
    }
    processAndGetDirectory(tr, dirNum, &dirCount, &hid, &lng, &acc, &lnk);

    for (int i = 0; i < dirCount; i++) {
        aux_listDir(dirNum[i], hid, lng, acc, lnk, 1); // 1 para revList
    }
}

// elimina un directorio vacio
void Cmd_erase(char *tr[]){
    if (tr[0] == NULL || (strcmp(tr[0], "-?") == 0)) {
        printf("uso: erase [archivo, directorio]\n");
        return;
    }
    aux_erase(tr);
}

// elimina directorios recursivamente
void Cmd_delrec(char *tr[]){
    if (tr[0] == NULL || (strcmp(tr[0], "-?") == 0)) {
        printf("uso: delrec [directorio]\n");
        return;
    }
    aux_delrec(tr[0]);
}

void Cmd_allocate(char *tr[]){
    if (tr[0] == NULL || tr[1] == NULL){
        aux_memory_blocks(&MEMLIST);
        return;
    } else if(strcmp(tr[0], "-?") == 0){
        printf("uso: allocate [-malloc, size] [-mmap, archivo, tipo_apertura] [-shared, num_clave, size]\n");
        return;
    } else if (strcmp(tr[0], "-malloc") == 0 && tr[1]!=NULL) {
        aux_allocate_malloc((size_t)atoll(tr[1])); //pasamos size
    } else if (strcmp(tr[0], "-mmap") == 0 && tr[1]!=NULL&& tr[2]!=NULL) { /*si ponemos solo -mmap nos debe aparecer la lista con los bloques*/
        aux_allocate_nmap(tr[1], tr[2]); //pasamos archivo y tipo_apertura
    } else if (strcmp(tr[0], "-shared") == 0 && tr[1]!=NULL&& tr[2]!=NULL) {
        aux_allocate_shared(atoi(tr[1]), atoi(tr[2])); //pasamos num_clave y size
    } else if (strcmp(tr[0], "-createshared") == 0 && tr[1]!=NULL&& tr[2]!=NULL) {
        aux_allocate_shared(atoi(tr[1]), atoi(tr[2])); //pasamos num_clave y size
    }
}

void Cmd_deallocate(char *tr[]){
    if (tr[0] == NULL) {
        aux_memory_blocks(&MEMLIST);
        return;
    } else if(strcmp(tr[0], "-?") == 0){
        printf("uso: allocate [-malloc, size] [-mmap, archivo, tipo_apertura] [-shared, num_clave, size]\n");
        return;
    }else if (strcmp(tr[0], "-malloc") == 0 && tr[1]!=NULL) {
        aux_deallocate_malloc((size_t)atoll(tr[1])); 
    } else if (strcmp(tr[0], "-mmap") == 0 && tr[1]!=NULL) { 
        aux_deallocate_mmap(tr[1]);
    } else if (strcmp(tr[0], "-shared") == 0 && tr[1]!=NULL) {
        aux_deallocate_shared(atoi(tr[1])); 
    } else if (strcmp(tr[0], "-delkey") == 0 && tr[1]!=NULL) {
        aux_deallocate_delkey(atoi(tr[1]));
    } else {
        void *addr = (void *)strtol(tr[0], NULL, 16);
        aux_deallocate_addr(&MEMLIST, addr);
    }   
    
}

void Cmd_memfill(char *tr[]){
    if(tr[0]==NULL||tr[1]==NULL||tr[2]==NULL){
        fprintf(stderr, "uso: memfill [addr] [cont] [ch]\n");
        return;
    }
    
    aux_mem(tr, false);
}

void Cmd_memdump(char *tr[]){
    if(tr[0]==NULL||tr[1]==NULL){
        fprintf(stderr, "uso: memdummp [addr] [cont]\n");
        return;
    }
    aux_mem(tr, true);
}

void Cmd_memory(char *tr[]){
    if (tr[0] == NULL) {
        aux_memory_all(); 
        return;
    } else if ((strcmp(tr[0], "-?") == 0)){
        printf("uso: memory [-funcs] [-vars] [-blocks], [-all], [-pmamp]\n");
        return;
    } else if (strcmp(tr[0], "-funcs") == 0) {
        aux_memory_funcs();
    } else if (strcmp(tr[0], "-vars") == 0) { 
        aux_memory_vars(); 
    } else if (strcmp(tr[0], "-blocks") == 0) {
        aux_memory_blocks(&MEMLIST); 
    } else if (strcmp(tr[0], "-all") == 0) {
        aux_memory_all(); 
    } else if (strcmp(tr[0], "-pmap") == 0) {
        aux_memory_pmap();
    }
}

void Cmd_read(char *tr[]){
    if(tr[0]==NULL || tr[1]==NULL || tr[2]==NULL || (strcmp(tr[0], "-?") == 0)){
        fprintf(stderr, "uso: read [archivo] [direccion_memoria] [num_bytes]\n");
        return;
    }
    aux_read(tr);
}

void Cmd_readfile(char *tr[]) {
    if (tr[0] == NULL || tr[1] == NULL || tr[2] == NULL || (strcmp(tr[0], "-?") == 0)) {
        printf("uso: readfile <nombre_archivo> <dirección> <número_bytes>\n");
        return;
    }
    aux_readFile(tr);
}

void Cmd_write(char *tr[]){
    if (tr[0] == NULL || tr[1] == NULL || tr[2] == NULL
        ||(strcmp(tr[0],"-o")==0 && tr[3]==NULL)){
         fprintf(stderr, "Uso: write[-o] <fd> <direccion_memoria> <tamano>\n");
        return;
    }
    aux_write(tr);
}

void Cmd_writefile(char *tr[]) {
    if (tr[0] == NULL || tr[1] == NULL || tr[2] == NULL
        ||(strcmp(tr[0],"-o")==0 && tr[3]==NULL)){
         fprintf(stderr, "Uso: writefile [-o] <archivo> <direccion_memoria> <tamano>\n");
        return;
    }


    int overWrite=0;
    int argIndex=0;

    if(strcmp(tr[0],"-o")==0){
        overWrite=1;
        argIndex=1;
    }

    const char *file=tr[argIndex];

    if(file==NULL){
        fprintf(stderr, "Error: archivo no especificado.\n");
        return;
    }
    
    char *endPtr;
    void *addr=(void *)strtoul(tr[1+argIndex],&endPtr,16);
    if(*endPtr!='\0'){
        fprintf(stderr, "Error: dirección de memoria '%s' inválida.\n", tr[argIndex + 1]);
        return;
    }

    size_t cont= (size_t)strtoul(tr[2+argIndex],&endPtr,10);
    if (*endPtr != '\0' || cont == 0) {
        fprintf(stderr, "Error: tamaño '%s' inválido.\n", tr[argIndex + 2]);
        return;
    }

    aux_writefile(file, addr, cont, overWrite);
}

void Cmd_recurse(char *tr[]){
    if(tr[0]==NULL || (strcmp(tr[0], "-?") == 0)){
        fprintf(stderr, "Uso: recurse [n] donde n es el número de invocaciones recursivas.\n");
        return;
    }

    int n=atoi(tr[0]);
    if(n<=0){
        fprintf(stderr, "Error: El número de invocaciones debe ser mayor que 0.\n");
        return;
    }

    static int arr_static=0;
    aux_recurse(n, &arr_static);
}

void Cmd_clear(char *tr[]){
    system("clear");
}

void Cmd_getuid(char *tr[]) {
    printf("user UID: %d\n", getuid());
    printf("user eUID: %d\n", geteuid());
}

void Cmd_setuid(char *tr[]) {
    if (tr[0] == NULL || strcmp(tr[0], "-?") == 0) {
        fprintf(stderr, "uso: setuid [-l] uid\n");
        return;
    }
    if (tr[1] != NULL && (strcmp(tr[1], "-l") == 0 || strcmp(tr[1], "-L") == 0)) {
        printf("Modo login detectado (-l).\n");
        aux_setuid(tr +1 , 1);
    } else {
        aux_setuid(tr, 0);
    }
}

void Cmd_showvar(char *tr[]) {
    int i;
    
    if (tr[0] == NULL) {
        fprintf(stderr, "uso: showvar var1 var2 ...\n");
        return;
    }
    
    for (i = 0; tr[i] != NULL; i++) {
        aux_showvar(tr[i]);
    }
}

void Cmd_changevar(char *tr[]) {
    char *var, *val;
    int opt = 0; // 0: setenv, 1: putenv

    if (tr[0] == NULL || tr[1] == NULL || (strcmp(tr[0], "-p") == 0 && tr[2] == NULL)) {
        fprintf(stderr, "uso: changevar [-a|-e|-p] var valor\n");
        return;
    }

    if (strcmp(tr[0], "-a") == 0 || strcmp(tr[0], "-e") == 0) {
        var = tr[1];
        val = tr[2];
    } else if (strcmp(tr[0], "-p") == 0) {
        opt = 1;
        var = tr[1];
        val = tr[2];
    } else {
        var = tr[0];
        val = tr[1];
    }

    if (aux_changevar(var, val, opt) != 0) {
        perror("Error al cambiar la variable de entorno");
    }
}

void Cmd_subsvar(char *tr[]) {
    char *var1, *var2, *val;

    if (tr[0] == NULL || tr[1] == NULL || tr[2] == NULL) {
        fprintf(stderr, "uso: subsvar [-a|-e] var1 var2 valor\n");
        return;
    }

    if (strcmp(tr[0], "-a") == 0 || strcmp(tr[0], "-e") == 0) {
        var1 = tr[1];
        var2 = tr[2];
        val = tr[3];
    } else {
        var1 = tr[0];
        var2 = tr[1];
        val = tr[2];
    }

    if (aux_subsvar(var1, var2, val) != 0) {
        perror("Error al sustituir la variable de entorno");
        return;
    }

    printf("Variable %s eliminada\n", var1);
    printf("Variable %s creada con valor %s\n", var2, val);
}

void Cmd_environ(char *tr[]) {
    if (tr[0] != NULL && strcmp(tr[0], "-environ") == 0) {
        aux_environ_addr(0);
    } else if (tr[0] != NULL && strcmp(tr[0], "-addr") == 0) {
        aux_environ_addr(1);
    } else {
        aux_environ_print();
    }
}

void Cmd_fork(char *tr[]){
    pid_t pid;

    if((pid=fork())==0){
        printf("Ejecutando proceso %d\n", getpid());
    } else if(pid != -1){
        waitpid(pid, NULL, 0);
    }
}

void Cmd_search(char *tr[]){
    if(tr[0]==NULL){
        showDirList(DIRLIST);
    }else if (strcmp(tr[0], "-add") == 0 && tr[1] != NULL) {
        addDir(&DIRLIST, tr[1]);
    } else if (strcmp(tr[0], "-del") == 0 && tr[1] != NULL) {
        if (deleteDir(&DIRLIST, tr[1])) {
            printf("Directorio eliminado: %s\n", tr[1]);
        } else {
            printf("El directorio %s no está en la lista.\n", tr[1]);
        }
    } else if (strcmp(tr[0], "-clear") == 0) {
        clearDirList(&DIRLIST);
        printf("Lista de búsqueda vaciada.\n");
    } else if (strcmp(tr[0], "-path") == 0) {
        importPath(&DIRLIST);
        printf("Directorios importados desde PATH.\n");
    } else {
        printf("Uso: search [-add dir] [-del dir] [-clear] [-path]\n");
    }
}

void Cmd_exec(char *tr[]){
    if (tr[0] == NULL) {
        fprintf(stderr, "Error: No se ha especificado programa a ejecutar\n");
        return;
    }

    if (tr[0][0] == '-') {
        fprintf(stderr, "Error: Opción no válida para exec. Uso: exec progspec [args...]\n");
        return;
    }
    char *program=NULL;
    char **newEnv=NULL;
    int envCount=0;

    for(int i=0; tr[i]!=NULL; i++){
        if(strchr(tr[i], '=')==NULL && getenv(tr[i])!=NULL){
            envCount++;
        }else{
            program=tr[i];
            break;
        }
    }

    if(program==NULL){
        fprintf(stderr,"Error: No se ha especificado programa a ejecutar\n");
        return;
    }
    if(envCount>0){
        newEnv=malloc((envCount+1)*sizeof(char *));
        if (newEnv == NULL) {
            perror("Error al asignar memoria para el entorno");
            return;
        }
        int index=0;
        for(int i=0; tr[i]!=NULL && index<envCount;i++){
            if(getenv(tr[i])!=NULL)
                newEnv[index++]=strdup(tr[i]);
        }
        newEnv[index]=NULL;
    }
    aux_execProgram(program, tr, newEnv);
}

void Cmd_execpri(char *tr[]){
    if (tr[0] == NULL || tr[1] == NULL) {
        fprintf(stderr, "Uso: execpri prio progspec [args...]\n");
        return;
    }
    char *endptr;
    int priority = strtol(tr[0], &endptr, 10); // Base 10
    if (*endptr != '\0') {
        fprintf(stderr, "Error: La prioridad debe ser un número entero.\n");
        return;
    }

    if (priority < -20 || priority > 19) {
        fprintf(stderr, "Error: La prioridad debe estar entre -20 y 19.\n");
        return;
    }

    if (priority < 0 && getuid() != 0) {
        fprintf(stderr, "Error: Las prioridades negativas requieren permisos de administrador.\n");
        return;
    }

    // Cambiar prioridad
    if (setpriority(PRIO_PROCESS, getpid(), priority) == -1) {
        fprintf(stderr, "Error al cambiar la prioridad: %s\n", strerror(errno));
        return;
    }

    // Identificar variables de entorno y el programa
    char *program = NULL;
    char **newEnv = NULL;
    int envCount = 0;

    for (int i = 1; tr[i] != NULL; i++) {
        if (strchr(tr[i], '=') == NULL && getenv(tr[i]) != NULL) {
            envCount++;
        } else {
            program = tr[i];
            break;
        }
    }

    if (program == NULL) {
        fprintf(stderr, "Error: No se ha especificado un programa a ejecutar.\n");
        return;
    }

    // Crear nuevo entorno si es necesario
    if (envCount > 0) {
        newEnv = malloc((envCount + 1) * sizeof(char *));
        if (newEnv == NULL) {
            perror("Error al asignar memoria para el entorno");
            return;
        }
        int index = 0;
        for (int i = 1; tr[i] != NULL && index < envCount; i++) {
            if (getenv(tr[i]) != NULL)
                newEnv[index++] = strdup(tr[i]);
        }
        newEnv[index] = NULL;
    }

    // Ejecutar el programa
    aux_execProgram(program, &tr[1], newEnv);
}

void Cmd_fg(char *tr[]){
    if(tr[0]==NULL){
        fprintf(stderr, "Error: No se ha especificado programa a ejecutar\n");
        return;
    }

    pid_t pid=fork();
    if(pid==-1){
        perror("Error al crear el proceso hijo");
        return;
    }
    if(pid==0){
        //proceso hijo: ejecuta programa
        aux_execProgram(tr[0], tr, NULL);
    }else{
        //proceso padre: esperar al hijo
        aux_waitForChild(pid);
    }
}

void Cmd_fgpri(char *tr[]){
    if (tr[0] == NULL || tr[1] == NULL) {
        fprintf(stderr, "Uso: fgpri prio prog args...\n");
        return;
    }

    char *endptr;
    int priority = strtol(tr[0], &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "Error: La prioridad debe ser un número entero.\n");
        return;
    }

    if (priority < -20 || priority > 19) {
        fprintf(stderr, "Error: La prioridad debe estar entre -20 y 19.\n");
        return;
    }
    pid_t pid=fork();
    if(pid==-1){
        perror("Error al crear el proceso hijo");
        return;
    }

    if(pid==0){
        //proceso hijo: ejecuta programa
        if (aux_setChildPriority(priority) == -1) {
            exit(EXIT_FAILURE); // Salir del hijo si falla el cambio de prioridad
        }
        aux_execProgram(tr[1], &tr[1], NULL);
    }else{
        //proceso padre: esperar al hijo
        aux_waitForChild(pid);
    }
}

void Cmd_back(char *tr[]) {
   if (tr[0] == NULL) {
        fprintf(stderr, "Uso: back progspec [args...]\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0){
        execvp(tr[0], tr);
        perror("Error al ejecutar el programa");
        exit(EXIT_FAILURE);
    } else if(pid > 0){
        addProcess(&PROCLIST, pid, tr[0], ACTIVE, 0);
        printf("Proceso %d ejecutandose en segundo plano\n", pid);
    } else{
        perror("Error al crear el proceso hijo");
    }
}

void Cmd_backpri(char *tr[]) {
    if (tr[0]== NULL || tr[1]==NULL){
        fprintf(stderr, "Uso: backpri prio progspec [args...]\n");
        return;
    }

    int priority = atoi(tr[0]);
    if (priority < -20 || priority >19){
        fprintf(stderr, "Error: la prioridad debe estar entre -20 y 19.\n");
        return;
    }    

    pid_t pid = fork();
    if (pid == 0){
        if (setpriority(PRIO_PROCESS, getpid(), priority) == -1){
            perror("Error al establecer prioridad");
            exit(EXIT_FAILURE);
        }
        execvp(tr[1], &tr[1]);
        perror("Error al ejecutar el programa");
        exit(EXIT_FAILURE);
        
    } else if (pid > 0){
        addProcess(&PROCLIST, pid, tr[1], ACTIVE, priority);
        printf("Proceso %d ejecutandose en segundo plano con prioridad %d\n", pid, priority);
    } else{
        perror("Error al crear el proceso hijo");
    }

}


void Cmd_listjobs() {
    showProcessList(PROCLIST);
}


void Cmd_deljobs(char *tr[]) {
    if (tr[0] == NULL) {
        fprintf(stderr, "Uso: deljobs -term|-sig\n");
        return;
    }

    if (strcmp(tr[0], "-term") == 0) {
        deleteProcessByState(&PROCLIST, FINISHED);
        printf("Procesos terminados eliminados.\n");
    } else if (strcmp(tr[0], "-sig") == 0) {
        deleteProcessByState(&PROCLIST, SIGNALED);
        printf("Procesos señalizados eliminados.\n");
    } else {
        fprintf(stderr, "Error: opción no válida. Usa -term o -sig.\n");
    }
}


void Cmd_external(char *tr[]) {
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo
        execvp(tr[0], tr);
        perror("Error al ejecutar el programa");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Proceso padre
        waitpid(pid, NULL, 0);
    } else {
        perror("Error al crear el proceso hijo");
    }
}


// definición del array de comandos
// si ponemos lo que hay en "", vamos a la funcion correspondiente
struct CMD C[] = {
    {"authors", Cmd_authors},
    {"pid", Cmd_pid},
    {"ppid", Cmd_ppid},
    {"cd", Cmd_chdir},
    {"exit", Cmd_exit},
    {"bye", Cmd_exit},
    {"quit", Cmd_exit},
    {"date", Cmd_date},
    {"help", Cmd_help},
    {"historic", Cmd_history},
    {"infosys", Cmd_infosys},
    {"open", Cmd_open},
    {"close", Cmd_close},
    {"dup", Cmd_dup},
    {"cwd", Cmd_cwd},
    {"mkdir", Cmd_mkdir},
    {"mkfile", Cmd_mkdir},
    {"listfile", Cmd_listfile},
    {"listdir", Cmd_listdir},
    {"reclist", Cmd_reclist},
    {"revlist", Cmd_revlist},
    {"erase", Cmd_erase},
    {"delrec", Cmd_delrec},
    {"allocate", Cmd_allocate},
    {"deallocate", Cmd_deallocate},
    {"read", Cmd_read},
    {"write", Cmd_write},
    {"memfill", Cmd_memfill},
    {"memdump", Cmd_memdump},
    {"memory", Cmd_memory},
    {"readfile", Cmd_readfile},
    {"writefile",Cmd_writefile},
    {"recurse", Cmd_recurse},
    {"clear", Cmd_clear},
    {"getuid", Cmd_getuid},
    {"setuid", Cmd_setuid},
    {"showvar", Cmd_showvar},
    {"changevar", Cmd_changevar},
    {"subsvar", Cmd_subsvar},
    {"environ", Cmd_environ},
    {"fork", Cmd_fork},
    {"search", Cmd_search},
    {"exec", Cmd_exec},
    {"execpri", Cmd_execpri},
    {"fg", Cmd_fg},
    {"fgpri", Cmd_fgpri},
    {"back", Cmd_back},
    {"backpri", Cmd_backpri},
    {"listjobs", Cmd_listjobs},
    {"deljobs", Cmd_deljobs},
    {"*****", Cmd_external},
    {NULL, NULL}
};

// permite leer y ejecutar el comando
void DoCommand(char *tr[]) {
    int i;
    char command[MAX_COMM] = "";

    if (tr[0] == NULL) { // solo el comando, sin parametros
        return;
    }
    for (i = 0; tr[i] != NULL; i++) { // concatena comando con parametros
        strcat(command, tr[i]);
        if (tr[i + 1] != NULL) {
            strcat(command, " ");
        }
    }
    insertHistoricItem(&HIS, command); // inserta en el historial el comando
    for (i = 0; C[i].name != NULL; i++) { // comprueba el nombre con lo que tenemos en la funcion del CMD
        if (!strcmp(tr[0], C[i].name)) {
            (*C[i].function)(tr + 1);
            return;
        } 
    }
    printf("sso: comando no encontrado: %s \n", tr[0]);//
}

// funcion para dividir las lineas
int funcBreakLine(char *line, char *pz[]) {
    int i = 1;
    if ((pz[0] = strtok(line, " \n\t")) == NULL)
        return 0;
    while ((pz[i] = strtok(NULL, " \n\t")) != NULL)
        i++;
    return i;
}

int main(int argc, char *argv[]) {
    char line[MAX_COMM];
    char *pcs[MAX_COMM / 2]; 
    initHistoricList(&HIS); // lista de comandos vacia
    initFileList(&FILELIST); // lista de ficheros vacia
    initMemList(&MEMLIST); // lista de memoria vacia
    initDirList(&DIRLIST); // lista de directorios vacia
    initProcessList(&PROCLIST); // lista de procesos vacia
    const char *username = getUsername();
    const char *osname = getOsName();
    while (1) {
        printf("%s@%s-%s >> ", username, username, osname);
        fgets(line, MAX_COMM, stdin);
        if (funcBreakLine(line, pcs) == 0) {  // Si no hay entrada válida
            continue;
        }
        DoCommand(pcs);
    }
}
