import ctypes
from api_gini import obtener_gini_argentina

# Cargar librería C
lib_procesamiento = ctypes.CDLL('../../lib/libprocesamiento.so')
# Definimos los tipos de los argumentos de la función
lib_procesamiento.procesar_gini.argtypes = [ctypes.c_float]
# Definimos el tipo de retorno de la función
lib_procesamiento.procesar_gini.restype = ctypes.c_int

def obtener_gini():
    # Obtener el índice de Gini de Argentina
    return obtener_gini_argentina()

# Obtener y procesar GINI
gini_float = obtener_gini()
if gini_float is not None:
    resultado = lib_procesamiento.procesar_gini(gini_float)
    print(f"Índice GINI procesado: {resultado}")
else:
    print("Error al obtener datos.")