#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
  char *addr = sbrk(0);
  sbrk(4096);
  
  addr[0] = 'Z';
  printf("Valor inicial: %c\n", addr[0]);
  
  printf("Aplicando proteccion de lectura...\n");
  if (mrdprotect(addr, 1) < 0) {
    printf("mrdprotect fallo\n");
    exit(1);
  }
  printf("Proteccion aplicada exitosamente\n");
  
  printf("Intentando leer...\n");
  char c = addr[0];
  printf("Valor leido: %c (esto NO deberia imprimirse)\n", c);
  
  exit(0);
}