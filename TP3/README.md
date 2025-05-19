# Sistemas de Computación 2025 - Modo Protegido en x86

## Introducción

Este proyecto tiene como objetivo demostrar la transición del modo real al modo protegido en la arquitectura x86.  
El trabajo práctico fue realizado sobre un entorno Linux.

La implementación incluye:
- Creación de una imagen de arranque (`main.img`).
- Configuración de la GDT (Global Descriptor Table).
- Activación del modo protegido mediante manipulación del registro `CR0`.
- Ejecución y depuración del código en QEMU.

---

## Estructura del proyecto

```
/home/Documents/SDC2025/TP3/
|
|-- Code/
|    |-- activate_protected_mode.S
|    |-- link.ld
|    |-- main.img
|    |-- activate_protected_mode.o
|    |-- activate_protected_mode.elf
|    |-- activate_protected_mode.bin
|
|-- Images/
|    |-- image1.png
|    |-- image2.png
|    |-- image3.png
```

---

## Instrucciones para compilar y ejecutar

1. Crear la imagen base:
```bash
printf '\364%509s\125\252' > main.img
```

2. Compilar el archivo assembly:
```bash
as --32 activate_protected_mode.S -o activate_protected_mode.o
```

3. Linkear el objeto generado usando el linker script:
```bash
ld -m elf_i386 -T link.ld -o activate_protected_mode.elf activate_protected_mode.o
```

4. Convertir el ejecutable a formato binario:
```bash
objcopy -O binary activate_protected_mode.elf activate_protected_mode.bin
```

5. Insertar el código en la imagen:
```bash
dd if=activate_protected_mode.bin of=main.img conv=notrunc
```

6. Ejecutar la imagen en QEMU:
```bash
qemu-system-i386 -drive file=main.img,format=raw
```

7. Para depuración con GDB:
```bash
qemu-system-i386 -fda main.img -boot a -s -S -monitor stdio
```
En otra terminal:
```bash
gdb gdb activate_protected_mode.elf
(gdb) target remote localhost:1234
(gdb) set architecture i8086
(gdb) break *0x7c00
(gdb) continue
```

---

## Capturas de pantalla

Imágenes capturadas durante la ejecución del entorno:

- ![Image 1](./Images/image1.png)
- ![Image 2](./Images/image2.png)
- ![Image 3](./Images/image3.png)

---

### Imágenes capturadas del proceso de Depuración con GDB y llamadas a BIOS

1. Luego de arrancar QEMU con el stub de GDB, lanzamos GDB sobre el ELF y ponemos un break:

![](./Images/InicioGDB.png)

2. Continuar hasta mov %cr0, %eax

![](./Images/mov-cr0-eax.png)
En 0x7c05, CR0 = 0x10 (solo ET=1), confirmando que aún estamos en modo real.

3. Preparar EAX = CR0 | 1

![](./Images/EAX%20=%20CR0%20|%201.png)
Antes de mov %eax,%cr0, EAX = 0x11, listo para activar PE.

4. Ejecutar mov %eax,%cr0 + ljmp

![](./Images//mov-eax-cr0+ljmp.png)
Tras ljmp, CR0 = 0x11 (PE=1) y CS = 0x08, confirmando modo protegido activo.

5. Recarga de segmentos de datos

![](./Images/segmentos%20de%20datos.png)
DS, ES, FS, GS y SS recargados con el selector 0x10.

6. Configurar stack y preservación de registros

![](./Images/Configurar%20stack%20y%20preservación%20de%20registros.png)
Se monta un nuevo stack en 0x7000 y se preservan registros para la impresión.

7. Impresión en VGA
Comprobamos punteros y volvamos memoria de vídeo

![](./Images//Impresión%20en%20VGA.png)
ECX apunta a “message” y EDX a 0xB8000.
Memoria 0xB8000 mostrando ASCII + atributo, confirmando la impresión.

8. Conclusión
Con esta depuración hemos verificado:
- Preparación de la GDT con lgdt.
- Activación de PE en CR0 y transición a modo protegido.
- Recarga de segmentos (CS, DS, ES, FS, GS, SS).
- Configuración de stack y preservación de registros.
- Impresión en memoria de vídeo desde modo protegido.




## Respuestas teóricas

**¿Qué es un Linker?**  
Un linker es una herramienta que combina uno o más archivos objeto (.o) generados por el compilador en un solo archivo ejecutable o binario. Resuelve referencias a funciones o variables entre los distintos archivos.

**¿Qué hace el Linker?**  
- Asigna direcciones de memoria a las instrucciones y datos.
- Resuelve direcciones de etiquetas.
- Agrupa las diferentes secciones (.text, .data, .bss) de manera coherente.

**¿Qué es la dirección en el script del linker?**  
La dirección que se fija (en este caso `0x7C00`) indica en qué posición de memoria RAM debe cargarse el código. Es necesaria para que el sistema sepa desde dónde comenzar a ejecutar instrucciones.

**¿Por qué es necesaria la dirección?**  
Porque el BIOS carga el sector de arranque en `0x7C00` y empieza la ejecución desde allí. Si el código estuviera preparado para otra dirección, fallaría.

**Comparación de `objdump` y `hd`:**  
- `objdump` muestra el contenido de un binario de forma entendible (ensamblador).
- `hd` (hexdump) muestra directamente los bytes hexadecimales.

Ambas herramientas permiten verificar dónde se colocó el programa en la imagen (`0x7C00`).

**¿Para qué se utiliza `--oformat binary`?**  
Se utiliza para generar un archivo binario puro (sin cabeceras de formato ELF u otros) que pueda ser cargado directamente por el BIOS o un emulador.

---

## Prueba en Hardware Real

1. Limpiar pendrive:

```bash
sudo dd if=/dev/zero of=/dev/sdb bs=1M
```
2. Grabar main.img:

```bash
sudo dd if=main.img of=/dev/sdb bs=512 conv=notrunc
sudo sync
```

3. Verificar firma 0x55AA:
```bash
sudo hexdump -C -n 512 /dev/sdb | tail -n 2
```

4. Para probarlo en Hardware real primero debemos confirgurar el mismo 
- 4a
![](./Images/Configuracion.jpeg)
Entramos a la configuracion de la BIOS

- 4b
![](./Images/BootMode.jpeg)
Configuramos el Boot Mode en Legacy

5. Iniciamos desde el dispositivo USB
![](./Images/InicioPorUSB.jpeg)

Luego:

```markdown
[Arranque desde Pendrive](./Images/ArranquePendrive.mp4)
```

Arranque real desde pendrive, mensaje impreso y CPU reiniciado tras finalizar.

**Con esto se demostro:**
- La transición de modo real a modo protegido en x86 mediante:
   - Configuración de la GDT y activación de PE en CR0.
   - Salto a código de 32 bits y recarga de segmentos.
   - Impresión en VGA.

## Desafío Final: Modo Protegido

Este desafío tiene como objetivo crear un programa en ensamblador que cambie al modo protegido, defina dos segmentos de memoria distintos (código y datos) y modifique los permisos del segmento de datos para observar el comportamiento al intentar escribir.

---

### Paso 1: Entender el modo protegido

El modo protegido permite:
- Uso de segmentación avanzada con protección por privilegios y permisos.
- Acceso a más de 1MB de memoria (limitación del modo real).
- Control de acceso a memoria mediante la GDT (Global Descriptor Table).

---

### Paso 2: Crear el archivo main.asm

Escribe un archivo llamado main.asm con lo siguiente:

```asm
BITS 16
org 0x7c00

start:
    cli
    xor ax, ax
    mov ds, ax

    lgdt [gdt_descriptor]

    ; Habilitar modo protegido
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_mode

[BITS 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax
    ; Intentar escribir en segmento de solo lectura (offset 0x0)
    mov dword [0x0], 0x12345678 ; Dirección lineal: 0x00200000 (base) + 0x0

hang:
    jmp hang

; GDT Corregida
gdt_start:
    ; Null descriptor
    dd 0x00000000
    dd 0x00000000

    ; Código: base 0x7C00, límite 0xFFFFF (4GB), ejec/lectura
    dw 0xFFFF       ; Límite 0-15
    dw 0x7C00       ; Base 0-15
    db 0x00         ; Base 16-23
    db 0x9A         ; Acceso: Presente, Ring 0, Código, Lectura
    db 0xCF         ; Límite 16-19 (0xF), Flags (G=1, D/B=1)
    db 0x00         ; Base 24-31

    ; Datos: base 0x00200000, límite 0xFFF (4KB), solo lectura
    dw 0x0FFF       ; Límite 0-15
    dw 0x0000       ; Base 0-15
    db 0x20         ; Base 16-23
    db 0x90         ; Acceso: Presente, Ring 0, Datos, Solo lectura
    db 0x00         ; Límite 16-19 (0x0), Flags (G=0)
    db 0x00         ; Base 24-31

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xAA55
```

Nota:
   - El descriptor de código (selector 0x08) permite ejecución/lectura.

   - El descriptor de datos (selector 0x10) es solo lectura.

---

### Paso 3: Compilar y ejecutar
Compilar:
```bash
nasm -f bin -o pmode.img main.S
```
Probar en QEMU:
```bash
qemu-system-x86_64 -hda pmode.img -s -S
```
- -s inicia un servidor GDB en el puerto 1234
- -S pausa la ejecución al arranque

---

### Paso 4: Verificar con GDB
Conecta con gdb y examina registros y memoria:
```bash
gdb
(gdb) target remote :1234
(gdb) c                    # Continúa ejecución
```

### Preguntas teoricas:

**¿Qué sucede al intentar escribir en un segmento de solo lectura?**
Si intentamos escribir en un segmento de solo lectura, el procesador generara una excepcion de proteccion de memoria. Esto es parte del modo protegido que impide el acceso no autorizado a memoria de solo lectura o solo de ejecucion. 

**¿Qué debería suceder a continuación?**
Segun la teoria del modo protegido, cuando se intenta escribir en un segmento de solo lectura, se genera una excepción de acceso a memoria (generalmente 0x0D, que indica "General Protection Fault"). El sistema operativo o el entorno de ejecución debería capturar esta excepción y gestionarla adecuadamente.

**¿Con qué valor se cargan los registros de segmento en modo protegido? ¿Por qué?**

En el modo protegido, los registros de segmento, como CS, DS, SS, se cargan con un valor que hace referencia a un descriptor de segmento específico en la GDT. En este desafio, el valor de CS se carga con el selector del segmento de código, mientras que DS se carga con el selector del segmento de datos.
Los valores de los registros de segmento determinan los permisos y la base de cada segmento. En el caso del segmento de datos, configuramos los permisos para que sea solo lectura, lo que evita que se escriba en él.

# Conclusión

Este desafío nos permitió explorar en profundidad el funcionamiento del modo protegido en la arquitectura x86. A través de la implementación de un pequeño programa en ensamblador, logramos:

- Cambiar de modo real a modo protegido.
- Definir una GDT con dos descriptores: uno para código y otro para datos, cada uno con bases y límites distintos.
- Configurar el descriptor de datos con permisos de solo lectura.
- Intentar escribir en una dirección válida dentro del segmento de datos para provocar una violación de acceso.

Como resultado, el sistema generó una excepción (#GP - General Protection Fault) al detectar la escritura no autorizada. Al no definirse una IDT con manejadores, esto derivó en un triple fault, provocando el reinicio de QEMU, lo cual confirma el correcto funcionamiento de los mecanismos de protección del modo protegido.

También observamos que los registros de segmento, en este modo, toman valores de selectores que apuntan a descriptores definidos en la GDT, lo que permite controlar privilegios, acceso y segmentación de manera precisa.

Por otro lado, se intentó ejecutar los programas generados en computadoras físicas reales mediante el uso de imágenes booteables grabadas en USB. Sin embargo, salvo por el ejemplo que simplemente entra en modo protegido e imprime un texto desde memoria de video, la mayoría de los casos no funcionaron correctamente en hardware.

En resumen, se cumplió el objetivo del desafío al lograr una implementación funcional de modo protegido con segmentación diferenciada y protección efectiva, aunque su ejecución fue limitada mayormente al entorno virtualizado proporcionado por QEMU.



## Referencias

- [Clase teórica de modo protegido](https://fcefyn.aulavirtual.unc.edu.ar/)
- [Documentación OSDev](https://wiki.osdev.org/Protected_Mode)
- [Ejemplos bare-metal x86](https://github.com/cirosantilli/x86-bare-metal-examples)

---

Trabajo práctico realizado para la materia **Sistemas de Computación 2025**.


