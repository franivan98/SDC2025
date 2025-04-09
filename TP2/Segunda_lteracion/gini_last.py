import tkinter as tk
from tkinter import ttk
from api_gini import obtener_gini_pais  # ahora usamos la versión dinámica
import pycountry
import ctypes
import traceback

# Cargar la librería compartida
lib = ctypes.CDLL('./libprocesamiento.so')
lib.procesar_gini.argtypes = [ctypes.c_float]
lib.procesar_gini.restype = ctypes.c_int

# Tkinter GUI
root = tk.Tk()
root.title("Visualizador del Índice de GINI")

# Combo con países
country_names = sorted([country.name for country in pycountry.countries])
selected_country = tk.StringVar()
combo = ttk.Combobox(root, textvariable=selected_country)
combo['values'] = country_names
combo.pack(padx=20, pady=20, ipadx=150)

# Label para mostrar resultados
resultado_label = tk.Label(root, text="", font=("Arial", 14))
resultado_label.pack(pady=20)

def on_select(event=None):
    country_name = selected_country.get()
    print("\n----------------------------")
    print(f"[DEBUG] País seleccionado: {country_name}")

    try:
        country = pycountry.countries.get(name=country_name)
        if not country:
            print("[ERROR] No se pudo encontrar el país con pycountry.")
            resultado_label.config(text="❌ País no encontrado en pycountry")
            return

        iso_code = country.alpha_3
        print(f"[DEBUG] Código ISO3: {iso_code}")

        # Obtener GINI desde la API
        gini_val = obtener_gini_pais(country_name)
        print(f"[DEBUG] Valor GINI bruto desde API: {gini_val}")

        if gini_val is None:
            print(f"[WARNING] No se obtuvo valor GINI para {country_name}.")
            resultado_label.config(text=f"⚠️ No hay datos GINI disponibles para {country_name}.")
            return

        # Procesar con .so
        try:
            gini_proc = lib.procesar_gini(gini_val)
            print(f"[DEBUG] GINI procesado con .so: {gini_proc}")
        except Exception as ce:
            print("[EXCEPTION] Error al ejecutar .so:", ce)
            resultado_label.config(text="❌ Error en el procesamiento en C")
            return

        resultado_texto = (
            f"{country_name} (ISO: {iso_code})\n"
            f"GINI bruto: {gini_val}\n"
            f"GINI procesado: {gini_proc}"
        )
        resultado_label.config(text=resultado_texto)
    except Exception as e:
        print("[EXCEPTION] Error inesperado en on_select:")
        traceback.print_exc()
        resultado_label.config(text="❌ Error inesperado al procesar país")


# Asociar evento al combo
combo.bind("<<ComboboxSelected>>", on_select)

root.mainloop()

