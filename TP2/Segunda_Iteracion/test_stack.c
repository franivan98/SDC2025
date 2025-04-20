#include <stdio.h>

extern int float_to_int(float);

int main(){
    float valor=3.14;
    int resultado;

    printf("Antes de llamar a float_to_int: %f\n", valor);
    resultado=float_to_int(valor);
    printf("Despues de llamar a float_to_int: %d\n", resultado);
    return 0;
    
}