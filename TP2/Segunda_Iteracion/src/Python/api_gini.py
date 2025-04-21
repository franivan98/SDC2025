import requests # Librería para realizar solicitudes HTTP
import pycountry # Librería para manejar códigos de países

# Función para obtener el índice de GINI de un país dado su nombre
def obtener_gini_pais(nombre_pais):
    try:
        print(f"[INFO] Buscando código ISO para: {nombre_pais}")
        country = pycountry.countries.get(name=nombre_pais) # Busca el país por nombre
#        print("[DEBUG] Objeto country:", country)

        if not country: # Imprime un mensaje de error si no se encuentra el país
            print("[ERROR] País no encontrado con pycountry.")
            return None

        iso_code = country.alpha_3  # Obtiene el código ISO 3166-1 alpha-3 que es el que usa el Banco Mundial
        
        print(f"[INFO] Código ISO encontrado: {iso_code}")
        # URL de la API del Banco Mundial para obtener el índice de GINI
        url = f"https://api.worldbank.org/v2/en/country/{iso_code}/indicator/SI.POV.GINI?format=json&date=2011:2020"
        print(f"[INFO] Consultando API del Banco Mundial: {url}")
        response = requests.get(url) # Realiza la solicitud HTTP a la API

        print(f"[DEBUG] Código de estado HTTP: {response.status_code}")
        if response.status_code != 200:
            print("[ERROR] Falló la solicitud HTTP")
            return None

        datos = response.json() # Convierte la respuesta JSON a un diccionario
        print(f"[DEBUG] JSON recibido: {datos}")

        for entry in datos[1]:  # Itera sobre los datos recibidos
            valor = entry.get('value') # Obtiene el valor del índice de GINI
            if valor is not None:
                print(f"[INFO] GINI encontrado: {valor}")
                return valor

        print("[WARNING] No hay valores GINI disponibles")
        return None

    except Exception as e:
        print("[EXCEPTION] Error en obtener_gini_pais:", e)
        return None

