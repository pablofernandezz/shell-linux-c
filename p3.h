#include "types.h"

typedef char command[MAX_COMM];

struct CMD {
    char *name;
    void (*function)(char **);  // Puntero a función
};

char *comandos[] = {
        "authors            - Imprime la lista de todos los autores (-p) y sus logins (-l).",
        "pid                - Imprime el PID del proceso que ejecuta al shell.",
        "ppid               - Imprime el PPID del proceso que ejecuta al shell.",
        "cd                 - Cambia el directorio actual de trabajo a 'dir'. Si no tiene argumentos, solo imprime el directorio actual.",
        "date               - Imprime la fecha actual.\n\t\tdate -t      - Imprime el tiempo actual en formato hh:mm:ss\n\t\tdate -d      - Imprime la fecha actual en formato DD/MM/YYYY",
        "historic           - Imprime el histórico de comandos ejecutados por el shell.\n\t\thistoric N      - Imprime el comando numero N\n\t\thistoric -N      - Imprime los ultimos N comandos",
        "open [file] modo   - Abre un archivo y lo añade a la lista de abiertos. Los modos disponibles son:\n\t\tcr      - Crear un archivo si no existe\n\t\tap      - Abre un archivo para agregar datos\n\t\tex      - Crear un archivo de forma exclusiva\n\t\tro      - Abrir en forma lectura\n\t\trw      - Abrir en forma lectura y escritura\n\t\two      - Abrir en forma escritura\n\t\ttr      - Trunca el archivo a una tamaño de 0 al abrirlo",
        "close [df]         - Cierra el descriptor de archivo 'df'.",
        "dup [df]           - Duplica el descriptor de archivo 'df'.",
        "infosys            - Imprime información sobre la máquina.",
        "help [cmd]         - Muestra una lista de comandos disponibles o el uso de uno específico.",
        "quit/exit/bye      - Sale del programa.",
        "cwd                - Imprime el directorio de trabajo actual.",
        "mkdir              - Crea un directorio.",
        "mkfile             - Crea un archivo.",
        "listfile           - Imprime contenido de archivos.",
        "listdir            - Imprime contenido de un directorio.",
        "reclist            - Imprime el contenido de un archivo recursivamente",
        "revlist            - Imprime el contenido de un archivo recursivamente de manera inversa",
        "erase              - Elimina un archivo o directorio",
        "delrec             - Elimina un archivo o directorio de manera recursiva",
        "allocate           - Asigna espacios de memoria con malloc, shmem o mmap",
        "deallocate         - Libera espacios de memoria usados con malloc, shmem, mmap o delkey",
        "memfill            - Rellena un espacio de memoria con contenido",
        "memdump            - Libera espacio de memoria de contenido",
        "read               - Lee un espacio de memoria",
        "write              - Escribe un espacio de memoria",

        NULL 
};

void Cmd_pid(char *tr[]);
void Cmd_ppid(char *tr[]);
void Cmd_authors(char *tr[]);
void Cmd_chdir(char *tr[]);
void Cmd_exit(char *tr[]);
void Cmd_date(char *tr[]);
void Cmd_history(char *tr[]);
void Cmd_help(char *tr[]);
void Cmd_infosys(char *tr[]);
void Cmd_open(char *tr[]);
void Cmd_dup(char *tr[]);
void Cmd_close(char *tr[]);
void Cmd_cwd(char *tr[]);
void Cmd_mkdir(char *tr[]);
void Cmd_mkfile (char *tr[]);
void Cmd_listfile(char *tr[]);
void Cmd_listdir(char *tr[]);
void Cmd_reclist(char *tr[]);
void Cmd_revlist(char *tr[]);
void Cmd_erase(char *tr[]);
void Cmd_delrec(char *tr[]);
void Cmd_allocate(char *tr[]);
void Cmd_deallocate(char *tr[]);
void Cmd_memfill(char *tr[]);
void Cmd_memdump(char *tr[]);
void Cmd_readfile(char *tr[]);
void Cmd_writefile(char *tr[]);
void Cmd_recurse(char *tr[]);
void Cmd_write(char *tr[]);
void Cmd_recurse(char *tr[]);
void Cmd_getuid(char *tr[]);
void Cmd_setuid(char *tr[]);
void Cmd_fork(char *tr[]);
void Cmd_search(char *tr[]);
void Cmd_exec(char *tr[]);
void Cmd_execpri(char *tr[]);
void Cmd_fg(char *tr[]);
void Cmd_fgpri(char *tr[]);
void Cmd_back(char *tr[]);
void Cmd_backpri(char *tr[]);
void Cmd_listjobs();
void Cmd_deljobs(char *tr[]);
void DoCommand(char *tr[]);
int funcBreakLine(char *line, char *pz[]);

