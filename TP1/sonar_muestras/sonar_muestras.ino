#include <NewPing.h>

#define TRIGGER_PIN  6
#define ECHO_PIN     7
#define MAX_DISTANCE 100

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);
}

void loop() {
  unsigned long contador_inicial = micros();

  // Medir distancia (en cm)
  float distancia = sonar.ping() / (2.0 * 29.1); // constante ajustada para mejor precisión
  
  Serial.println(distancia);  // <-- Solo esto imprime la distancia, ideal para graficar

  // Esperar hasta completar 20 ms por ciclo (50 Hz)
  unsigned long tiempo_de_Tareas = micros() - contador_inicial;
  delay((20000 - tiempo_de_Tareas) / 1000);
  delayMicroseconds(20000 - tiempo_de_Tareas % 1000);
}
