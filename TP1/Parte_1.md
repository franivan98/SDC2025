# Trabajo practico de Laboratorio n°1: Primera Parte
## Lista de benchmarks:

### Benchmarks Sintéticos
* **Dhrystone**: Evalua el rendimiento de la CPU en operaciones con enteros.

* **Whetstone**: Mide el rendimiento en calculos de punto flotante.

* **LINPACK**: Evalua la capacidad de la CPU para resolver sistemas de ecuaciones.

### Benchmarks Reducidos
* **Livermore Loops**: Evalua el rendimiento en la ejecucion de bucles intensivos en calculos. 

* **STREAM**: MIde el ancho de banda de memoria en operaciones secuenciales 

### Benchmarks Kernel o Núcleo
* **Imbench**: Mide las latencias de memoria, ancho de banda y rendimiento de llamadas al sistema.

* **FIO**: Evalua el rendimiento de entrada/salida en discos y sistemas de archivos.

### Programas Reales
* **SPEC CPU**: Evalua el rendimiento general del sistema con carga de trabajo reales.

* **TPC-C**: Analiza el rendimiento en bases de datos con simulaciones de transacciones comerciales.

* **Blender Benchmark**: Prueba la capacidad de la CPU y GPU en renderizado 3D.
---
## Mas utiles para Franco Mamani:

### Perfil de Franco:
Se centra en proyectos de software afines a la carrera, haciendo uso de interacciones con llamadas del sistema operativo como en la materia de "Sistemas operativos I y II". Como pasatiemp, juega videojuegos.

### Benchmarks mas utiles para este perfil:

* **STREAM**: mide el ancho de banda en memoria, lo cual impacta en compilaciones de codigo y en juegos que carguen grandes volumenes de datos en ram.

* **FIO**: Util para evaluar el rendimiento del disco, lo cual impacta en tiempo de carga de juego y compilaciones.

* **SPEC CPU**: Evalua el rendimiento general del sistema en carga de trabajo reales

* **Imbench**: Mide latencias de memoria, ancho de banda, y el rendimiento de las llamadas del sistema.

| **Tarea**                               | **Benchmark más representativo**     | **Motivo** |
|-----------------------------------------|-------------------------------------|------------|
| Compilar código                         | Build Linux Kernel / SPEC CPU      | Evalúa rendimiento de CPU y memoria en compilaciones. |
| Ejecutar código ensamblador en SO       | lmbench                            | Mide latencias de llamadas al sistema y acceso a memoria. |
| Cargar archivos grandes en el sistema   | FIO                                | Evalúa rendimiento de lectura/escritura en disco. |
| Probar multitarea en el SO              | lmbench                            | Mide rendimiento en operaciones de concurrencia y multitarea. |
| Jugar videojuegos                       | STREAM / FIO                       | STREAM evalúa el rendimiento de la memoria y FIO el acceso a disco, claves en juegos. |
| Procesar datos en memoria               | STREAM                             | Evalúa el ancho de banda de memoria. |
| Ejecutar procesos en paralelo           | lmbench                            | Analiza la latencia de creación de procesos y cambios de contexto. |


