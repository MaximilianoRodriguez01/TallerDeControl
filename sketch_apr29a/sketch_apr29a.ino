#include <Servo.h>
Servo myservo;

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float delta_T = 0.020;
int angulo_cero = 0;
float medicion1 = 0;



void setup() {
  Serial.begin(115200);
  Wire.setWireTimeout(20000, true);
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  while (!Serial)
     delay(10); // will pause Zero, Leonardo, etc until serial console opens

  // Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    // Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  // Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  delay(100);
}

void matlab_send(float* datos, size_t cantidad) {
  Serial.write("abcd");  // header fijo para identificar el inicio del paquete

  for (size_t i = 0; i < cantidad; i++) {
    byte* b = (byte*) &datos[i];
    Serial.write(b, 4);  // Cada float ocupa 4 bytes
  }
}

void loop() {
  unsigned long contador_inicial = micros();

  static float ang_tot = 0.0;
  static float error_anterior = 0;
  static float u_anterior = 0;
  float referencia = 0;
  const float alpha = 0.08;  // Peso del acelerómetro

  unsigned long t0 = micros();  // <- ahora antes de leer el sensor

  // 1) Leer IMU
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // 2) Calcular dt real en segundos
  unsigned long dt_us = micros() - t0;
  float dt = dt_us * 1e-6;

  // 3) Integrar giroscopio (convierto rad/s a grados/s)
  float gyro_rate = g.gyro.x * 180.0 / PI;
  float ang_g = ang_tot + gyro_rate * dt;

  // 4) Calcular ángulo por acelerómetro
  float ang_a = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;

  // 5) Filtro complementario (según imagen)
  if (!isnan(ang_a) && abs(ang_a) <= 90) {
    ang_tot = (1 - alpha) * ang_g + alpha * ang_a;
  }

  Serial.println(ang_tot);
  // Si la lectura es inválida o extrema, no se actualiza ang_tot

  float error = referencia - ang_tot;


  float u = 0.03 * (error - error_anterior) + u_anterior;

  int angulo_default = 90;
  myservo.write(u + angulo_default);

  u_anterior = u;
  error_anterior = error;

  unsigned long tiempo_de_Tareas = micros() - contador_inicial;
  long tiempo_restante = 20000L - tiempo_de_Tareas;

  if (tiempo_restante > 1000) {
    delay(tiempo_restante / 1000); // en milisegundos
    delayMicroseconds(tiempo_restante % 1000); // el resto en microsegundos
  } else if (tiempo_restante > 0) {
    delayMicroseconds(tiempo_restante);
  }
}
