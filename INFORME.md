# Tarea 0: Instalación de xv6

## Grupo 5: José Joaquín Clavería, Faustina Parada

### Objetivo
Para esta tarea debimos instalar el sistema operativo xv6, compilarlo y ejecutarlo utilizando el emulador QEMU, y documentar el proceso.


### Pasos para la instalación

1. **Clonación del repositorio:** Primero, se hizo  un "fork" del repositorio original del MIT a nuestras cuentas personales en GitHub y luego se clonaron a nuestras computadoras de forma local.
2. **Instalación de dependencias:** Luego, se instaló Homebrew para instalar RISC-V Toolchain usando el comando "brew install riscv-tools" y luego "brew install --cc=gcc-15 riscv-tools". Después, se instaló QUEMU utilizando "brew install qemu".
3. **Compilación y ejecución:** Una vez que se verificó la correcta instalación de RISC-V y QEMU con "riscv64-unknown-elf-gcc --version" y "qemu-system-riscv64 --version", nos aseguramos que nos encontrábamos en el directorio correcto utilizando "cd xv6-riscv" y utilizamos el comando "make" para compilar xv6. Para ejecutarlo, el siguiente paso fue utilizar el comando "make qemu", que indicó que el núcleo de xv6 se inició correctamente.
4. **Uso del terminal:** Tras ejecutarse correctamente, pudimos comenzar a trabajar en la terminal. Para verificar la instalación, ejecutamos los comandos 'ls', 'echo "Hola xv6"' y 'cat README'.  

### Problemas encontrados y soluciones

* **Error 'No makefile found':** Al inicio, intentamos ejecutar 'make' sin estar en el directorio correcto, lo que nos entregó este error. Para solucionarlo, nos dirigimos a la carpeta de 'xv6-riscv' ubicada en el Escritorio con el comando 'cd', lo que nos permitió lograr acceder a la carpeta indicada.
* **Error 'No rule to make target':** Al investigar para la tarea, leímos que era necesario utilizar el comando 'make qemu-nox' para ejecutar QEMU. Sin embargo, al utilizar este comando no logramos ejecutar el emulador. Luego de más investigación, encontramos que la versión más reciente de xv6 usa el comando 'make qemu' para ejecutar QEMU, lo que solucionó el problema.

### Confirmación de funcionamiento correcto

La instalación de xv6 fue exitosa. El sistema operativo se compiló y se ejecutó correctamente en el emulador QEMU y fue posible ejecutar los comandos de prueba solicitados, como se evidencia en las capturas de pantalla en la entrega de WebCursos y adjuntas a continuación:

https://raw.githubusercontent.com/fauparada/xv6-riscv/fauparada/captura1_xv6.JPG

