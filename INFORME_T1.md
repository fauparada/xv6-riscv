# Tarea 1: Llamadas al Sistema en xv6

## Grupo 5: José Joaquín Clavería, Faustina Parada

## PARTE I

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

## PARTE II

### 1. Funcionamiento de la llamada al sistema

Implementamos esta llamada para entregar el PID del ancestro ubicado a n niveles de distancia del proceso que la llama.

La lógica de sys_getancestor() en sysproc.c en kernel usa un loop for para iterar n veces, moviendo el puntero del proceso de ese momento (p) a su padre (p = p->parent) en cada paso.

Definimos algunos casos especiales de prueba, que son los siguientes:

* n = 0: Entrega el PID del proceso actual (p->pid).
* n = 1: Entrega el PID del padre (p->parent->pid), por lo que es equivalente a getppid().
* n > 1: Entrega el PID del ancestro n (abuelo, bisabuelo, etc.).
* Casos límite: Si en la iteración se llega a un PID 0 o si n excede los ancestros disponibles, la función entrega -1.

### 2. Modificaciones adicionales

Para integrar la función getancestor(n) se hicieron las siguientes modificaciones además de las que hicimos en la parte I:

* Modificamos syscall.h en kernel para agregar "#define SYS_getancestor 23", asignando el 23 a la nueva syscall.
* Modificamos syscall.c en kernel para agregar "sys_getancestor" y lo mapeamos en el array syscall para enlazar el número 23 con la implementación de la función.
* Modificamos user.h en user para agregar "int getancestor(int n)", para poder llamar a la función.
* Modificamos usys.S en user para agregar el "stub" de getancestor para evitar el error de linking que tuvimos en la parte I.
* Reemplazamos el programa que habíamos hecho anteriormente para yosoytupadre.c con una versión que verificara correctamente todos los niveles de ancestros.

### 3. Dificultades encontradas y solución

Tuvimos tres dificultades principales durante esta parte:

1. Error de "linking".
Al compilar yosoytupadre.c, nos apareció el error "undefined reference to 'getancestor'", que fue lo mismo que había ocurrido en la parte I. Para solucionarlo, tal como lo hicimos anteriormente, agregamos manuelmente el código de ensamblador para getancestor en el archivo usys.S en user.

2. Error del compilador.
Al compilar se generó el error "void value not ignored". Al investigar, entendimos que fue un error en sysproc.c en kernel al intentar verificar el valor de retorno de la función argint(0, &n), y que en esta versión xv6, la función argint no entrega un int para mostrar un éxito o falla, sino que entrega void. Para solucionarlo eliminamos la verficación del valor de retorno, dejando la llamada argint(0, &n), asumiendo que la lectura del argumento fue existosa para la implementación del syscall.

3. Error en casos límite.
Al compilar y luego usar el programa yosoytupadre en el terminal, el programa fallaba al verificar los ancestros n = 3 y n = 4, entregando PIDs incorrectos en vez de los esperados. Después de analizarlo, nos dimos cuenta de que asumimos que la jerarquía de procesos empezaba con PID 1, lo que no estaba bien. Además la lógica en sys_getancestor no manejaba bien la trancisión al proceso con PID 0. Para solucionarlo, primero actualizamos el programa de prueba yosoytupadre.c para que considerara la jerarquía real y ajustamos los PIDs esperados para n = 3 y n = 4. Después, corregimos la lógica en sysproc.c en kernel para que el loop se detuviera correctamente y entregara -1 si el puntero del proceso subía más que PID 0.
