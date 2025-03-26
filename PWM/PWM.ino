#include <Servo.h>

Servo myservo;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
}

void loop() {

  // delay(1000);
  // myservo.writeMicroseconds(1000);
  // delay(1000);
  // myservo.writeMicroseconds(1500);
  // delay(1000);
  // myservo.writeMicroseconds(2000);
  // delay(1000);
  delay(1000);
  int angulo_default = 90;
  myservo.write(-90 + angulo_default);

    delay(1000);
  myservo.write(0 + angulo_default);

    delay(1000);

  myservo.write(90 + angulo_default);
}
