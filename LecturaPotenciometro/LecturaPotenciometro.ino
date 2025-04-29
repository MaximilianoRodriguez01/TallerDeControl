void setup() {
  Serial.begin(9600);
}

// con el delay en 1, la tarea tarda aproximadamente 6204 microsegundos
// con el codigo actual, tenemos 19932 microsegundos

void loop() {
  unsigned long contador_inicial = micros();
  int sensorValue = analogRead(A0);
  float angulo = (sensorValue * 300.0) / 1023.0;
  // Serial.println((sensorValue * 300.0) / 1023.0);
  Serial.println(micros() - contador_inicial);

  // unsigned long tiempo_de_Tareas = (micros() - contador_inicial);
  // delay((20000 - tiempo_de_Tareas) / 1000);
  // delayMicroseconds(tiempo_de_Tareas % 1000);
  // Serial.println(micros() - contador_inicial);
}
