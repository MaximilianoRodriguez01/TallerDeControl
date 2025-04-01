#include <NewPing.h>

#define TRIGGER_PIN  6  // Pin conectado al trigger
#define ECHO_PIN     7  // Pin conectado al echo
#define MAX_DISTANCE 100 // Maxima distancia de medicion para probar en el aula

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);
}

// EN CLASE: 
// con delay de 50 tarda aprox 53 milisegundos
// sin prints de distancia necesita un delay de 17 para llegar a los 50 Hz
// con prints tambien, no influyen demasiado


// EN CASA:
// Ping: 3.81
// El tiempo de ejecucion es: 3036
// Ping: 24.64
// El tiempo de ejecucion es: 4296

// El tiempo de ejecucion es: 21096
// Ping: 5.17
// El tiempo de ejecucion es: 21116
// Ping: 7.43
// El tiempo de ejecucion es: 19384

void loop() {
  delay(50);
  unsigned long contador_inicial = micros();
  Serial.print("Ping: ");
  Serial.println(sonar.ping() / (2*29.287));
  
  // 
  unsigned long tiempo_de_Tareas = (micros() - contador_inicial);
  delay((20000 - tiempo_de_Tareas) / 1000);
  delayMicroseconds(tiempo_de_Tareas % 1000);
  Serial.print("El tiempo de ejecucion es: ");
  Serial.println(micros() - contador_inicial);
}
