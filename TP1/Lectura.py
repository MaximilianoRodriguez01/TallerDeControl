import serial
import matplotlib.pyplot as plt
import time

# Configuración del puerto serie (ajusta aquí tu puerto)
puerto = '/dev/tty.usbserial-130'
baudrate = 115200

ser = serial.Serial(puerto, baudrate, timeout=1)
time.sleep(2)  # Esperar reinicio de Arduino

# Configurar gráfico en modo interactivo
plt.ion()
fig, ax = plt.subplots()
xdata, ydata = [], []
line_plot, = ax.plot(xdata, ydata, 'b-')
ax.set_xlabel('Muestra')
ax.set_ylabel('Ángulo total (°)')
ax.set_title('Ángulo estimado en tiempo real (MPU6050)')

i = 0
try:
    while True:
        raw = ser.readline().decode('utf-8').strip()
        if not raw:
            continue

        # DEBUG: ver qué línea llegó
        # print("RAW:", raw)

        try:
            ang_total = float(raw)
        except ValueError:
            # ignora cualquier texto distinto de un número
            continue

        # Mostrar por consola
        print(f"[{i}] Ángulo total: {ang_total:.2f}°")

        # Acumular dato
        xdata.append(i)
        ydata.append(ang_total)
        i += 1

        # Actualizar gráfica
        line_plot.set_data(xdata, ydata)
        ax.relim()
        ax.autoscale_view()
        plt.pause(0.001)

except KeyboardInterrupt:
    print("\nInterrupción por teclado, cerrando puerto y mostrando gráfico final.")
    ser.close()
    plt.ioff()
    plt.show()
