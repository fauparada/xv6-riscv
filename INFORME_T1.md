# Tarea 1: Llamadas al Sistema en xv6

## Grupo 5: José Joaquín Clavería, Faustina Parada

### 1.  Funcionamiento de la llamada al sistema

Implementamos la llamada al sistema "getppid()" para retornar el ID del proceso padre (PPID) del proceso que la invoca. Esta llamada requiere acceder a la estructura de procesos del kernel para obtener el pid del proceso padre asociado, no como getpid() que retorna el ID del proceso actual.

#### Verificando con "yosoytupadre.c"

Creamos el programa de prueba "yosoytupadre.c" para verificar que la relación padre-hijo sea correcta. Al ser ejecutado en el terminal en xv6 con QEMU ocurre lo siguiente:

1. El proceso inicial (padre) llama a getpid() y getppid().
2. El padre ejecuta fork() para crear un hijo.
3. El hijo llama a getppid(). El valor entregado es el PID del padre original, confirmando que se implementó.

### 2. Explicación de las modificaciones

Para integrar la llamada al sistema getppid() se debió modificar o crear los siguientes archivos:

* Modificamos syscall.h en kernel/ para agregar "#define SYS_getppid 22". Esto asigna el número 22 a la nueva syscall en el kernell.

* Modificamos syscall.c en kernel/ para declarar "extern uint64 sys_getppid(void);" y mapear "[SYS_getppid] sys_getppid" en el arreglo "syscalls[]". Esto enlazó la syscall 22 con la función de implementación en el kernel.

* Modificamos sysproc.c en kernel/ para implementar la lógica de la función 
"sys_getppid()", que usa "myproc()" para obtener la estructura del proceso actual y entrega el pid del proceso padre o 0 si no existe padre.

* Modificamos user.h en user/ para agregar "int getppid(void);", para permitir que el programa "yosoytupadre.c" que creamos conozca y pueda llamar a la función.

* Modificamos usys.S en user/ para agregar manualmente el "stub" para getppid. Esto lo hicimos para resolver el error de "linking" que obtuvimos al compilar.

* Modificamos el Makefile en la carpeta principal para agregar "_yosoytupadre" a la lista "UPROGS". Esto indica al compilador que debe compilar e incluir el programa.

* Creamos yosoytupadre.c en user/ para crear el programa de prueba que verifica que funcione correctamente el nuevo syscall.

### 3. Dificultades encontradas y solución

La principal dificultad que tuvimos fue en el proceso de linking del programa de prueba, pues obtuvimos el error "undefined reference to 'getppid'" al intentar compilar yosoytupadre.c usando QEMU, a pesar de haberlo definido en user.h.

Luego de hacer una búsqueda en internet, entendimos que el compilador encontró la declaración en user/user.h pero no el código del "stub" (ensamblador) que hace la llamada al kernel. Esto pasó porque la versión de xv6-riscv que usamos no generó automáticamente ese "stub" para la syscall que creamos.

Para solucionarlo, identificamos el archivo "usys.S" en user/ porque este es el responsable de tener los "stubs" de llamadas al sistemas. Lo abrimos para agregar de forma manual  el bloque ensamblador de getppid.
