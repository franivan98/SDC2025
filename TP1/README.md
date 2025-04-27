### Sistemas de Computación
# Trabajo Práctico N°1
#### El rendimiento de las computadoras
---
### Integrantes
* **Graham Enrique Lenox**  -   Ing. en Comp.
* **Mamani Franco Ivan**    -   Ing. en Comp.
* **Vega Guadalupe**    -   Ing. en Elect.

### Desarrollo del Trabajo
En este repositorio se respondieron las preguntas propuestas en la consigna, y se llevaron a cabo pruebas con la esp32. El trabajo practico se dividio en 3 partes:

#### 1) Marco Teorico y Time Profiling:
Resuelto en el archivo `TP1/MarcoTeorico_TimeProf.md` en donde se detallan algunas definiciones que usaremos en este tp, y tambien se encuentra resuelto el test del Time Profiling, resuelto por Guadalupe.

#### 2) Parte 1: Benchmarks
Desarrollado en `TP1/Parte_1.md`, se hace una lista de benchmarks y posteriormente seleccionamos los que mas nos serivirian a cada uno. En algunos casos, se agregan mas benchmarks convencionales. Los resultados se encuentran en `TP1/results`

Luego se hace un estudio sobre los procesadores Intel Core i5-13600K
y AMD Ryzen 9 5900X 12-Core desde el punto de vista de su rendimiento.

#### 3) Parte 2: Esp32
Se hizo un programa para ver como impacta el cambio de frecuencia del procesador en una suma de enteros y una suma de flotantes. al final, se calcula el speedup y la eficiencia.
El archivo se encuentra en `TP1/Parte_2.md` y el codigo que usamos se encuentra en el directorio `TP1/src`