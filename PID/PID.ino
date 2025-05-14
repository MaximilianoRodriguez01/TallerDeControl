#include <Servo.h>
Servo myservo;
#include <Wire.h>
#include <NewPing.h>

#define TRIGGER_PIN  6  // Pin conectado al trigger
#define ECHO_PIN     7  // Pin conectado al echo
#define MAX_DISTANCE 100 // Maxima distancia de medicion para probar en el aula

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object

  // Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  delay(100);
}

// void matlab_send(float* datos, size_t cantidad) {
//   Serial.write("abcd");  // header fijo para identificar el inicio del paquete

//   for (size_t i = 0; i < cantidad; i++) {
//     byte* b = (byte*) &datos[i];
//     Serial.write(b, 4);  // Cada float ocupa 4 bytes
//   }
// }

void loop() {
  unsigned long t0 = micros();

  static float error_anterior = 0;
  static float u_anterior = 0;
  float referencia = 15.8;
  const float alpha = 0.08;  // Peso del acelerómetro

  // Valor kp medido que oscila = 6

  float k0 = 0;
  float kp = 3;
  float ki = 0;
  float kd = 0;

  unsigned int duracion = sonar.ping();
  float distancia_carro;

  if (duracion == 0) {
    // Lectura inválida, mantener último valor o saltar la iteración
    distancia_carro = referencia; // o alguna estimación razonable
  } else {
    distancia_carro = duracion / (2.0 * 29.287);
  }


  
  // 2) Calcular dt real en segundos
  unsigned long dt_us = micros() - t0;
  float dt = 0.020;


  // PID Discreto 
  static float I = 0;
  static float D = 0;
  static float u = 0;

  float error = referencia - distancia_carro;
  Serial.print("El error es: ");
  Serial.println(error);

  // Integral trapezoidal
  I = I + (dt / 2) * (error + error_anterior);

  // Derivada filtrada
  float derivada = 2 * (error - error_anterior) / dt - D;
  D = 2 * (error - error_anterior) / dt;

  // PID discreto
  u = kp * error + ki * I + kd * derivada;

  // Actualizar estados
  error_anterior = error;

  int angulo_default = 90;
  myservo.write(u + angulo_default);

  Serial.print("La accion de control es: ");
  Serial.println(u);



  u_anterior = u;
  error_anterior = error;

  unsigned long tiempo_de_Tareas = micros() - t0;
  long tiempo_restante = 20000L - tiempo_de_Tareas;

  delay(20 - (tiempo_de_Tareas)/1000.0);
}
