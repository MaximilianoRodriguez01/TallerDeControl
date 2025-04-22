#include <Servo.h>

Servo myservo;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  Serial.begin(9600);
}

void loop() {

  // delay(1000);
  // myservo.writeMicroseconds(1000);
  // delay(1000);
  // myservo.writeMicroseconds(1500);
  // delay(1000);
  // myservo.writeMicroseconds(2000);
  // delay(1000);

  // EJERCICIO 2:
  delay(1000);
  int angulo_default = 90;
  // myservo.write(-90 + angulo_default);  

  int angulos[] = {-30, 0, 30};

  delay(1000);
  myservo.write(angulos[0] + angulo_default);

  delay(1000);
  myservo.write(angulos[1] + angulo_default);

  delay(1000);

  myservo.write(angulos[2] + angulo_default);

  // EJERCICIO 3

  // unsigned long contador_inicial = micros();
  
  // int sensorValue = analogRead(A0);
  // // Serial.println(sensorValue);

  // // int angulo = (sensorValue * 300.0) / 1023.0;  

  // int angulo = map(sensorValue, 0, 1023, 0, 180);

  // angulo = angulo % 180;

  // // Serial.println(angulo);
  // myservo.write(angulo); 

  // unsigned long tiempo_de_Tareas = (micros() - contador_inicial);
  // delay((1000000 - tiempo_de_Tareas) / 1000);
  // delayMicroseconds(tiempo_de_Tareas % 1000);
  // Serial.println(micros() - contador_inicial);
}
