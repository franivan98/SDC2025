import requests
import pycountry

def obtener_gini_pais(nombre_pais):
    try:
        print(f"[INFO] Buscando código ISO para: {nombre_pais}")
        country = pycountry.countries.get(name=nombre_pais)
#        print("[DEBUG] Objeto country:", country)


        if not country:
            print("[ERROR] País no encontrado con pycountry.")
            return None

        iso_code = country.alpha_3

        print(f"[INFO] Código ISO encontrado: {iso_code}")

        url = f"https://api.worldbank.org/v2/en/country/{iso_code}/indicator/SI.POV.GINI?format=json&date=2011:2020"
        print(f"[INFO] Consultando API del Banco Mundial: {url}")
        response = requests.get(url)

        print(f"[DEBUG] Código de estado HTTP: {response.status_code}")
        if response.status_code != 200:
            print("[ERROR] Falló la solicitud HTTP")
            return None

        datos = response.json()
        print(f"[DEBUG] JSON recibido: {datos}")

        for entry in datos[1]:
            valor = entry.get('value')
            if valor is not None:
                print(f"[INFO] GINI encontrado: {valor}")
                return valor

        print("[WARNING] No hay valores GINI disponibles")
        return None

    except Exception as e:
        print("[EXCEPTION] Error en obtener_gini_pais:", e)
        return None

