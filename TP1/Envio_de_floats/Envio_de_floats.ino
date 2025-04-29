unsigned long t_inicio, t_fin;
float datos[100];

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 100; i++) {
    datos[i] = (float)i;
  }
  
  t_inicio = micros();
  for (int i = 0; i < 100; i++) {
    Serial.write((uint8_t *)&datos[i], sizeof(float));
  }
  Serial.flush();
  t_fin = micros();
  Serial.println();
  Serial.println(t_fin - t_inicio);
}

void loop() {}