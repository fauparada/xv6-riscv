#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int pid_padre = getpid();

    printf("PROCESO INICIAL\n");
    printf("    Mi PID es: %d\n", pid_padre);
    printf("    Mi padre (PPID) es: %d\n", getppid()); //Llama al nuevo syscall

    //Crear un hijo para mostrar la relación
    int fork_pid = fork();

    if (fork_pid == 0) {
        //Proceso hijo (entrega 0 en el hijo)
        printf("\n Soy el hijo \n");
        printf("    Mi PID es: %d\n", getpid());
        printf("    Mi padre (PPID) es: %d\n", getppid());

    } else if (fork_pid > 0) {
        //Proceso padre (entrega el PID del hijo)
        wait(0); //Espera a que el hijo termine
        printf("\n Soy el padre (original) \n");
        printf("    Mi hijo con PID %d ha terminado.\n ", fork_pid);

    } else {
        //Error
        printf("Error en el proceso\n");
        exit(1);
    }

    exit(0);
}