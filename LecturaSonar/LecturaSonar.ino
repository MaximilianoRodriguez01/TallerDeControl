#include <NewPing.h>

#define TRIGGER_PIN  6  // Pin conectado al trigger
#define ECHO_PIN     7  // Pin conectado al echo
#define MAX_DISTANCE 100 // Maxima distancia de medicion para probar en el aula

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);
}

// con delay de 50 tarda aprox 53 milisegundos
// sin prints de distancia necesita un delay de 17 para llegar a los 50 Hz
// con prints tambien, no influyen demasiado

void loop() {
  unsigned long contador_inicial = micros();          
  Serial.print("Ping: ");
  Serial.print(sonar.ping() / (2*29.287));
  Serial.println("cm");
  // unsigned long contador_resta = (micros() - contador_micro);
  // Serial.println(contador_resta);
  unsigned long tiempo_de_Tareas = (micros() - contador_inicial);
  delay((20000 - tiempo_de_Tareas) / 1000);
  delayMicroseconds(tiempo_de_Tareas % 1000);
  Serial.println(micros() - contador_inicial);
}
