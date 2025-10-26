#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NPROC_TEST 10

void
worker(int id, int tickets)
{
  settickets(tickets);
  
  printf("Proceso %d: tickets=%d, PID=%d\n", id, tickets, getpid());
  
  // Realizar trabajo intensivo en CPU
  volatile int sum = 0;
  for(int i = 0; i < 50000000; i++) {
    sum += i;
  }
  
  printf("Proceso %d finalizado (sum=%d)\n", id, sum);
  exit(0);
}

int
main(int argc, char *argv[])
{
  int i;
  int pid;
  
  printf("=== Iniciando prueba de Lottery Scheduling ===\n");
  printf("Creando %d procesos con diferentes tickets\n\n", NPROC_TEST);
  
  for(i = 0; i < NPROC_TEST; i++) {
    pid = fork();
    if(pid < 0) {
      printf("Error en fork\n");
      exit(1);
    }
    if(pid == 0) {
      // Proceso hijo
      int tickets = 50 * (i + 1);
      worker(i, tickets);
    }
  }
  
  // Proceso padre espera a todos los hijos
  for(i = 0; i < NPROC_TEST; i++) {
    wait(0);
  }
  
  printf("\n=== Prueba finalizada ===\n");
  exit(0);
}