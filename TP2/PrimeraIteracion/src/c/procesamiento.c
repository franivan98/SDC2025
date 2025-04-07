#include <stdio.h>
#include <math.h>

// Funcion para procesar el valor de Gini
int procesar_gini(float gini_float) {
    int gini_entero = (int)round(gini_float);  // Convertir a entero
    return gini_entero + 1;  // Sumar 1
}