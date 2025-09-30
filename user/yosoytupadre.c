#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void test_ancestor(int n, int expected_pid) {
    int actual_pid = getancestor(n);
    printf("getancestor(%d): PID entregado: %d", n, actual_pid);
    if (actual_pid == expected_pid) {
        printf("\nOK\n");
    } else {
        printf("\nERROR (Esperado: %d)\n", expected_pid);
    }
}

int
main(int argc, char *argv[])
{
    int pid_padre = getpid();
    int fork_pid1 = fork();

    if (fork_pid1 == 0) {
        //código proceso hijo
        int pid_hijo = getpid();
        int fork_pid2 = fork();

        if (fork_pid2 == 0) {
            //código proceso nieto
            int pid_nieto = getpid();
            printf("\nProceso nieto (PID: %d)\n", pid_nieto);

            //pruebas del nieto:
            test_ancestor(0, pid_nieto); //él mismo
            test_ancestor(1, pid_hijo); //padre (hijo)
            test_ancestor(2, pid_padre); //abuelo (padre)
            test_ancestor(3,2); //"bisabuelo" (init)
            test_ancestor(4,1); //no existe 
            test_ancestor(5,-1); //caso límite, no existe

        } else if (fork_pid2 > 0) {
            //codigo del proceso hijo (espera al nieto)
            wait(0);
        }
    } else if (fork_pid1 > 0) {
        //codigo proc. padre (espera al hijo)
        wait(0);
    }

    printf("\nPruebas terminadas\n");
    exit(0);
}