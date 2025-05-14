#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(115200);
  Wire.setWireTimeout(20000, true);
  while (!Serial) delay(10);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  delay(100);
}

void loop() {
  static float ang_tot = 0.0;
  const float alpha = 0.05;  // Peso del acelerómetro

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
// Si la lectura es inválida o extrema, no se actualiza ang_tot
  unsigned long tf = micros() - t0;

  // 7) Esperar para mantener ~50 Hz
  if (dt_us < 20000) {
    delayMicroseconds(20000 - dt_us);
  }
}

