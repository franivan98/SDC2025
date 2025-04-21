import tkinter as tk                    # módulo de Python para crear interfaces gráficas (ventanas, botones, combos, etc.).
from tkinter import ttk                 # módulo de Python para crear widgets (elementos gráficos) en la interfaz.
from api_gini import obtener_gini_pais  # modulo propio tque trae datos GINI desde una API.
import pycountry                        # módulo de Python que permite acceder a información sobre países, como nombres, códigos ISO, etc.
import ctypes                           # módulo de Python que permite interactuar con bibliotecas compartidas (librerías C/C++).
import traceback                        # módulo de Python que permite imprimir el seguimiento de la pila de excepciones (errores) para depuración.

# Cargar la librería compartida
lib = ctypes.CDLL('../../lib/libprocesamiento.so')      # Aqui se carga la librería compartida que contiene la función de procesamiento del índice de Gini.
lib.procesar_gini.argtypes = [ctypes.c_float]   # Definimos los tipos de los argumentos de la función
lib.procesar_gini.restype = ctypes.c_int        # Definimos el tipo de retorno de la función

# Tkinter GUI
root = tk.Tk() # Crear la ventana principal
root.title("Visualizador del Índice de GINI") # Título de la ventana

# Combo con países
country_names = sorted([country.name for country in pycountry.countries])   # Obtener nombres de países y ordenarlos alfabéticamente
selected_country = tk.StringVar()                                           # Variable para almacenar el país seleccionado
combo = ttk.Combobox(root, textvariable=selected_country)                   # Crear un combo (desplegable) para seleccionar el país
combo['values'] = country_names                                             # Asignar la lista de países al combo
combo.pack(padx=20, pady=20, ipadx=150)                                     # Añadir un espacio entre el combo y el label

# Label para mostrar resultados
resultado_label = tk.Label(root, text="", font=("Arial", 14))               # Crear un label para mostrar el resultado
resultado_label.config(justify="left")                                      # Configurar el label para que el texto esté alineado a la izquierda
resultado_label.pack(pady=20)                                               # Añadir un espacio entre el label y el botón

# Botón para procesar el país seleccionado
def on_select(event=None):
    country_name = selected_country.get()                                   # Obtener el país seleccionado del combo
    print("\n----------------------------")
    print(f"[DEBUG] País seleccionado: {country_name}")

    try:
        country = pycountry.countries.get(name=country_name)                # Obtener el objeto del país seleccionado
        if not country:
            print("[ERROR] No se pudo encontrar el país con pycountry.")
            resultado_label.config(text="❌ País no encontrado en pycountry")
            return

        iso_code = country.alpha_3                                          # Obtener el código ISO3 del país
        print(f"[DEBUG] Código ISO3: {iso_code}")

        # Obtener GINI desde la API
        gini_val = obtener_gini_pais(country_name)  # Llamar a la función para obtener el índice de Gini desde la API
        print(f"[DEBUG] Valor GINI bruto desde API: {gini_val}")

        if gini_val is None:
            print(f"[WARNING] No se obtuvo valor GINI para {country_name}.")
            resultado_label.config(text=f"⚠️ No hay datos GINI disponibles para {country_name}.")
            return

        # Procesar con .so
        try:
            gini_proc = lib.procesar_gini(gini_val)                         # Llamar a la función de procesamiento de la librería compartida
            print(f"[DEBUG] GINI procesado con .so: {gini_proc}")
        except Exception as ce:
            print("[EXCEPTION] Error al ejecutar .so:", ce)
            resultado_label.config(text="❌ Error en el procesamiento en C")
            return

        # Mostrar resultados
        resultado_texto = (                             
            f"{country_name} (ISO: {iso_code})\n"
            f"GINI bruto: {gini_val}\n"
            f"GINI procesado: {gini_proc}"
        )
        resultado_label.config(text=resultado_texto)                        # Mostrar el resultado en el label
    except Exception as e:
        print("[EXCEPTION] Error inesperado en on_select:")
        traceback.print_exc()
        resultado_label.config(text="❌ Error inesperado al procesar país")


# Asociar evento al combo
combo.bind("<<ComboboxSelected>>", on_select)   # Cuando se selecciona un país en el combo, se llama a la función on_select

root.mainloop() # Iniciar el bucle principal de la interfaz gráfica

