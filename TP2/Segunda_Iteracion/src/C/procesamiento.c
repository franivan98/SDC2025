#include <stdio.h>

// Declaración externa
extern int float_to_int(float); // Función externa para convertir float a int en assembler

// Función para procesar el valor de Gini
int procesar_gini(float gini_float) {
    return float_to_int(gini_float); // Llama a la función externa
}
