# TP2 - Visualizador del Índice de GINI 

Este trabajo práctico muestra cómo utilizar ensamblador x86 de 32 bits en conjunto con código en C, y cómo integrarlo en una aplicación Python con interfaz gráfica para visualizar el índice de GINI de distintos países.

Tambien se hace un Analisis del Stack usando gdb. Esto esta explicado en el documento `Analisis_stack.md`

---

## 📌 ¿Qué hace el programa?

- El usuario selecciona un país desde una lista desplegable en una ventana gráfica.
- El programa obtiene el índice GINI más reciente del país usando la **API del Banco Mundial**.
- Ese valor se pasa como `float` a una función en **ensamblador**, vía una función intermedia en C.
- El valor retornado (como `int`) se muestra como resultado "procesado".

---

## 🧠 ¿Por qué usar 32 bits?

- El objetivo del trabajo es **vincular código Python con funciones escritas en assembler x86 de 32 bits**.
- Python de 64 bits no puede cargar librerías `.so` de 32 bits.
- Por eso usamos **Miniconda 32 bits** y creamos un entorno llamado `py32bits`.

---

## 📦 Archivos principales

```
.
├── README.md                      # Documentación del proyecto
├── Makefile                       # Automatiza la compilación de C y ASM
├── lib/
│   └── libprocesamiento.so        # Librería compartida compilada en 32 bits
└── src/
    ├── python/
    │   ├── gini_last.py           # Script principal con la GUI y lógica de ejecución
    │   └── api_gini.py            # Código que consulta la API del Banco Mundial
    ├── c/
    │   └── procesamiento.c        # Función en C que llama a la función en ensamblador
    └── assembler/
        └── float_int.asm          # Función en ensamblador 32 bits

```

---

## ⚙️ Instalación paso a paso

### 1. 🔧 Instalar Miniconda 32 bits

```bash
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86.sh
chmod +x Miniconda3-latest-Linux-x86.sh
./Miniconda3-latest-Linux-x86.sh
```

Instalarlo en una ruta como: `~/miniconda32`

---

### 2. 🧪 Crear el entorno `py32bits`

```bash
source ~/miniconda32/bin/activate
conda create -n py32bits python=3.7
conda activate py32bits
```

### 3. ✅ Verificar arquitectura Python

```bash
file $(which python)
# Debería decir: ELF 32-bit LSB executable
```

---

### 4. 📦 Instalar dependencias Python

```bash
pip install requests pycountry
# tkinter ya viene con python3.7 en conda, si no:
sudo apt install python3-tk
```

---

## 🛠️ Compilación de C y ASM

Si es necesario, se deben instalar las herramientas necesarias:

```bash
sudo apt-get install gcc-multilib g++-multilib nasm
```

Compilación manual:

```bash
nasm -f elf32 float_int.asm
gcc -m32 -fPIC -c procesamiento.c -o procesamiento.o
gcc -m32 -shared -o libprocesamiento.so procesamiento.o float_int.o -lm
```

O con Makefile:

```bash
make
```

---

## ▶️ Ejecución del programa

Estando en el entorno `py32bits`:

```bash
python gini_last.py
```

Se abrirá la ventana GUI. Al seleccionar un país, verás el GINI crudo y el GINI procesado por la función en ensamblador.

---

## Análisis de códigos

### `float_int.asm`

Convierte un `float` a `int`, le suma 1, y retorna el resultado:

```nasm
fld dword [ebp + 8]
fistp dword [esp]
mov eax, [esp]
add eax, 1
```

### `procesamiento.c`

Función `procesar_gini(float)` que llama al assembler:

```c
extern int float_to_int(float);
int procesar_gini(float gini_float) {
    return float_to_int(gini_float);
}
```

### `gini_last.py`

GUI hecha con Tkinter + llamada vía `ctypes` a `libprocesamiento.so`.

---

## Logs de depuración

Al ejecutar desde la terminal verás:

- `[DEBUG]` datos internos como ISO, valores GINI
- `[INFO]` resultado de la API
- `[EXCEPTION]` errores si ocurre algo inesperado

---

## Ejecución con script automático

Se utiliza este script para activar el entorno y lanzar la app:

```bash
#!/bin/bash
source ~/miniconda32/bin/activate
conda activate py32bits
python gini_last.py
```

Se guarda como `launch.sh`:

```bash
chmod +x launch.sh
./launch.sh
```

---

## ✅ Estado actual

- [x] GUI funcional
- [x] API del Banco Mundial integrada
- [x] Llamada C + ASM de 32 bits
- [x] Módulo `.so` cargado desde Python con `ctypes`
- [x] Probado en entorno `py32bits` usando Miniconda

## Ejemplo de ejecución

Al ejecutar el script:

```bash
python gini_last.py
```

Se muestra una ventana gráfica con el título "Visualizador del Índice de GINI":

### 1. Interfaz inicial

Se muestra un buscador vacío para elegir un país:

![Interfaz inicial](./images/Screenshot%20from%202025-04-08%2021-07-29.png)

---

### 2. Lista desplegable de países

Al hacer clic, aparece la lista de países disponibles:

![Lista de países](./images/Screenshot%20from%202025-04-08%2021-07-44.png)

---

### 3. País seleccionado y resultados en pantalla

Una vez elegido **Argentina**, se consulta la API y se muestran los valores en pantalla:

![Resultado en GUI](./images/Screenshot%20from%202025-04-08%2021-08-08.png)

---

### 4. Resultados en consola

También se muestran los resultados por consola para fines de depuración:

![Resultados en consola](./images/Screenshot%20from%202025-04-08%2021-08-20.png)
