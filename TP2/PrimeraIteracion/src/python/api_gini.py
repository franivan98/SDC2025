import requests

def obtener_gini_argentina():
    #URL de la API del Banco Mundial con formato JSON para 2011–2020.
    url = "https://api.worldbank.org/v2/en/country/ARG/indicator/SI.POV.GINI?format=json&date=2011:2020"
    #Petición HTTP GET.
    response = requests.get(url)
    #Verificar si la respuesta fue exitosa.
    if response.status_code == 200:
        # Convertir la respuesta a JSON.
        # Basicamente convierte el JSON a un diccionario de Python.
        datos = response.json()
        # Extraer el último valor de GINI disponible (ejemplo simplificado)
        # Índice 0: Metadatos de la consulta (página, total de resultados, etc.).
        # Índice 1: Datos reales, es decir, una lista de registros de datos, donde cada registro es un diccionario con campos.
        valor_gini = datos[1][0]['value']
        return valor_gini  # Retorna un float (ej: 42.3)
    else:
        print('Response Failed')
        return None