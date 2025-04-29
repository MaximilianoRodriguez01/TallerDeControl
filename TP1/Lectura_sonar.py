import serial
import matplotlib.pyplot as plt
import time

# Configurar puerto serie (ajustá el nombre del puerto, en MacOS suele ser algo como /dev/tty.usbmodemXXXX)
puerto = '/dev/tty.usbserial-130'  # <-- CAMBIAR por tu puerto correcto
baudrate = 115200

# Abrir puerto serie
ser = serial.Serial(puerto, baudrate, timeout=1)

# Esperar a que Arduino se reinicie
time.sleep(2)

# Configurar gráfico
plt.ion()
fig, ax = plt.subplots()
xdata, ydata = [], []
line, = ax.plot(xdata, ydata, 'b-')
ax.set_xlabel('Muestras')
ax.set_ylabel('Distancia (cm)')
ax.set_title('Lectura de sensor ultrasonido en tiempo real')

# Leer y graficar en tiempo real
try:
    while True:
        if ser.in_waiting > 0:
            linea = ser.readline().decode('utf-8').strip()
            try:
                distancia = float(linea)
                print(f"Distancia: {distancia} cm")
                xdata.append(len(xdata))
                ydata.append(distancia)

                # # Mantener solo los últimos 100 datos en la gráfica
                # if len(xdata) > 100:
                #     xdata = xdata[-100:]
                #     ydata = ydata[-100:]

                line.set_xdata(xdata)
                line.set_ydata(ydata)
                ax.relim()
                ax.autoscale_view()
                plt.pause(0.001)
            except ValueError:
                # Ignorar líneas que no sean números
                pass

except KeyboardInterrupt:
    print("\nTerminando lectura...")
    ser.close()
    plt.ioff()
    plt.show()
