import matplotlib.pyplot as plt
from matplotlib.widgets import Button
import matplotlib.animation as animation
import time

DEVICE_PATH = "/dev/cdd_signal"
x_data = []
y_data = []
start_time = time.time()
senal_actual = 0
senal_nueva = 0
reiniciar = False

def enviar_senal_al_driver(señal):
    global senal_actual, senal_nueva, reiniciar
    try:
        with open(DEVICE_PATH, "w") as f:
            f.write(str(señal))
        if señal != senal_actual:
            senal_nueva = señal
            reiniciar = True
    except Exception as e:
        print(f"Error escribiendo al driver: {e}")

def leer_valor():
    try:
        with open(DEVICE_PATH, "r") as f:
            contenido = f.read().strip()
            partes = contenido.split(":")
            if len(partes) == 2:
                señal = int(partes[0].strip()[-1])
                valor = int(partes[1].strip())
                return señal, valor
    except Exception as e:
        print(f"Error leyendo: {e}")
    return None, None

def actualizar(frame):
    global senal_actual, senal_nueva, reiniciar, start_time

    if reiniciar:
        x_data.clear()
        y_data.clear()
        start_time = time.time()
        senal_actual = senal_nueva
        reiniciar = False
        print(f"🔁 Reiniciando gráfico para señal {senal_actual}")

    señal, valor = leer_valor()
    if señal is None:
        return

    tiempo = time.time() - start_time
    x_data.append(tiempo)
    y_data.append(valor)

    if len(x_data) > 50:
        x_data.pop(0)
        y_data.pop(0)

    ax.clear()
    ax.plot(x_data, y_data, marker='o')
    ax.set_ylim(-0.2, 1.2)
    ax.set_title(f"Señal {senal_actual}")
    ax.set_xlabel("Tiempo (s)")
    ax.set_ylabel("Valor lógico (0 o 1)")
    ax.grid(True)

# 🎛️ Interfaz
fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.2)

ax_b0 = plt.axes([0.25, 0.05, 0.1, 0.075])
ax_b1 = plt.axes([0.65, 0.05, 0.1, 0.075])
btn_0 = Button(ax_b0, 'Señal 0')
btn_1 = Button(ax_b1, 'Señal 1')

btn_0.on_clicked(lambda event: enviar_senal_al_driver(0))
btn_1.on_clicked(lambda event: enviar_senal_al_driver(1))

ani = animation.FuncAnimation(fig, actualizar, interval=1000)
plt.show()
