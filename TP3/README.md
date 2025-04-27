# Sistemas de Computación 2025 - Modo Protegido en x86

## Introducción

Este proyecto tiene como objetivo demostrar la transición del modo real al modo protegido en la arquitectura x86.  
El trabajo práctico fue realizado sobre un entorno Linux, utilizando ensamblador GAS (sintaxis AT&T) y herramientas como `as`, `ld`, `objcopy` y `qemu-system-i386`.

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
gdb
(gdb) target remote localhost:1234
(gdb) set architecture i8086
(gdb) break *0x7c00
(gdb) continue
```

---

## Capturas de pantalla

Imágenes capturadas durante la ejecución del entorno:

- ![Image 1](../Images/image1.png)
- ![Image 2](../Images/image2.png)
- ![Image 3](../Images/image3.png)

---

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

## Licencia

Este proyecto está bajo la licencia Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0).

---

## Referencias

- [Clase teórica de modo protegido](https://fcefyn.aulavirtual.unc.edu.ar/)
- [Documentación OSDev](https://wiki.osdev.org/Protected_Mode)
- [Ejemplos bare-metal x86](https://github.com/cirosantilli/x86-bare-metal-examples)

---

Trabajo práctico realizado para la materia **Sistemas de Computación 2025**.


