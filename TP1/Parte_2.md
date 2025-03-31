# Trabajo practico de Laboratorio n°1: Segunda Parte

## Propuesta
Conseguir un esp32 o cualquier procesador al que se le pueda cambiar la frecuencia.
Ejecutar un código que demore alrededor de 10 segundos. Puede ser un bucle for con sumas de enteros por un lado y otro con suma de floats por otro lado.

¿Qué sucede con el tiempo del programa al duplicar (variar) la frecuencia ? 

## Desarrollo
Se desarrolo un codigo para la esp32 usando arduino en `TP1/src/esp32-freq.ino`. Presenta dos funciones:
* Suma de numeros enteros.
* Suma de numeros con punto flotante.
la cantidad de numeros a sumar esta ajustado para que cada funcion tarde 10 segundos, con una frecuencia de reloj del procesador de la esp32 de 80 Mhz.

los resultados obtenidos fueron los siguientes: 
```
Frecuencia configurada en: 80 Mhz

Número de sumas de enteros: 71932200
Tiempo transcurrido (Segundos): 10.00
----------------------------------
Número de sumas de floats: 1139322
Tiempo transcurrido (Segundos): 10.02
----------------------------------

Frecuencia configurada en: 160 Mhz

Número de sumas de enteros: 71932200
Tiempo transcurrido (Segundos): 4.97
----------------------------------
Número de sumas de floats: 1139322
Tiempo transcurrido (Segundos): 4.98
----------------------------------

Frecuencia configurada en: 240 Mhz

Número de sumas de enteros: 71932200
Tiempo transcurrido (Segundos): 3.31
----------------------------------
Número de sumas de floats: 1139322
Tiempo transcurrido (Segundos): 3.32
----------------------------------
```
## Calculos de Speedup y eficiencia
Con los datos obtenidos, podemos realizar el calculo de speedup y eficiencia en cada uno de los casos, tomando de base la velocidad a 80 Mhz.

| Frecuencia (MHz) | Speedup (Enteros) | Eficiencia (Enteros) | Speedup (Flotantes) | Eficiencia (Flotantes) |
|------------------|------------------|----------------------|--------------------|----------------------|
| 80              | 1.000             | 1.000                | 1.000               | 1.000                |
| 160             | 2.012             | 1.006                | 2.012               | 1.006                |
| 240             | 3.021             | 1.007                | 3.018               | 1.006                |

## Conclusiones

El rendimiento de la ESP32 escala casi linealmente con la frecuencia, con una eficiencia ligeramente superior al 100%. Esto sugiere que el aumento de frecuencia no solo reduce el tiempo de ejecución, sino que también puede mejorar la utilización del procesador, posiblemente debido a factores como:  
- Mejor aprovechamiento de caché.
- Menor impacto de latencias fijas. 

Si bien el consumo energético no se midió, generalmente, el aumento de frecuencia implica un mayor consumo. Esto podría ser un factor relevante en sistemas embebidos con restricciones de energía.
