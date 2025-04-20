# TP Índice de Gini — Implementación hasta la fecha

Este documento resume los pasos y artefactos implementados hasta el momento en el Trabajo Práctico 1 (Índice de Gini) usando una arquitectura Python → C → (próximamente ASM).

---

## 1. Estructura del proyecto

```text
Primera_Iteracion/
├── venv/               # Entorno virtual de Python
├── python
|    └── api_gini.py         # Módulo Python para consumir la API REST
|    └── main.py             # Script Python principal que integra Python y C
├── c
|    └── procesamiento.c         # Código C con la función procesar_gini()
├── lib
|    └── libprocesamiento.so # Biblioteca compartida generada desde procesamiento.c
└── README.md           # Este documento
``` 

---

## 2. Entorno Python (venv)

1. Crear y activar el entorno virtual:
   ```bash
   python3 -m venv venv
   source venv/bin/activate
   ```
2. Instalar dependencias:
   ```bash
   pip install requests
   ```

---

## 3. Consumo de la API REST (api_gini.py)

```python
import requests

# Obtiene la lista de valores de Gini para Argentina (2011–2020)
def obtener_gini_argentina():
    url = (
        "https://api.worldbank.org/v2/en/country/ARG/indicator/SI.POV.GINI"
        "?format=json&date=2011:2020"
    )
    res = requests.get(url)
    res.raise_for_status()
    datos = res.json()[1]
    # Extrae solo valores numéricos
    valores = [registro['value'] for registro in datos if registro['value'] is not None]
    return valores
```  

---

## 4. Capa intermedia en C (procesamiento.c)

```c
#include <stdio.h>
#include <math.h>

// Convierte float a entero (redondeo) y suma 1
int procesar_gini(float gini_float) {
    int gini_int = (int)roundf(gini_float);
    return gini_int + 1;
}

int main(void) {
    float g;
    while (scanf("%f", &g) == 1) {
        printf("%d\n", procesar_gini(g));
    }
    return 0;
}
```  

---

## 5. Compilación de la biblioteca compartida

Se compila el archivo fuente en C (procesamiento.c) como una biblioteca compartida utilizando GCC:

```bash
gcc -shared -fPIC -o lib/libprocesamiento.so procesamiento.c -lm
```  
**Explicacion de las Opciones:**

 - "shared"
Genera una biblioteca compartida (.so) en lugar de un ejecutable.

 - "fPIC"
Produce código independiente de la posición (Position Independent Code), requerido para bibliotecas dinámicas.

 - "o lib/libprocesamiento.so"
Define la ruta y el nombre del archivo de salida.

 - "lm"
Enlaza la librería matemática estándar, necesaria si se utiliza round, roundf, floor, sqrt, etc.

---

## 6. Integración Python → C (main.py)

```python
import ctypes
from api_gini import obtener_gini_argentina

# Cargar la biblioteca compartida
lib = ctypes.CDLL('./libprocesamiento.so')

# Definir firma de la función procesar_gini
lib.procesar_gini.argtypes = [ctypes.c_float]
lib.procesar_gini.restype = ctypes.c_int

if __name__ == '__main__':
    valores = obtener_gini_argentina()
    for v in valores:
        resultado = lib.procesar_gini(v)
        print(f"Gini raw: {v:.2f} → procesado: {resultado}")
```  

**Cómo ejecutar:**

```bash
source venv/bin/activate
python main.py
```  

---

## 7. Siguientes pasos

1. **Implementar la rutina en ensamblador (NASM)**: trasladar `procesar_gini()` a ASM.
2. **Enlazar y probar** la nueva función ASM desde C.
3. **Depurar con GDB**: insertar breakpoints en la rutina ASM y mostrar el stack.
4. **Documentar diagramas** (bloques, secuencia) y casos de prueba.
5. **Versionar en GitHub** siguiendo flujo de forks y pull requests.

---

*Fecha: marzo 2025*
