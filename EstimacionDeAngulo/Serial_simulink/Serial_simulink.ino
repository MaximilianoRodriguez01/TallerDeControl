
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float delta_T = 0.020;
// float angulo;

void setup(void) {
  Serial.begin(115200);
  Wire.setWireTimeout(20000, true);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
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
  Serial.print("Gyro range set to: ");
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

  float angulo = 0;

  Serial.println("");
  delay(100);
}
void loop() {
  static float angulo_g = 0;
  static float angulo_g_sesgado = 0;
  static float angulo_a = 0;
  static float angulo_total = 0;
  unsigned long t0 = micros();
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float acel_x = a.acceleration.x;
  float acel_y = a.acceleration.y;
  float acel_z = a.acceleration.z;

  float gyro_x = g.gyro.x;
  float gyro_y = g.gyro.y;
  float gyro_z = g.gyro.z;

  angulo_g = angulo_total + (180 / PI)*(gyro_x * delta_T);

  angulo_g_sesgado = angulo_g_sesgado + (180 / PI)*(gyro_x * delta_T);

  if (abs(acel_z) < 0.005 && abs(acel_y) < 0.005) {
    // Datos poco confiables, conservar valor anterior
    angulo_a = angulo_a; // opcionalmente no actualices
  } else {
    angulo_a =  ((180 / PI) * atan2(acel_y, acel_z));
  }
  
  float alpha = 0.1;

  angulo_total = alpha*angulo_a + (1-alpha)*angulo_g;


  Serial.print(angulo_g_sesgado); Serial.print(",");
  Serial.print(angulo_a); Serial.print(",");
  Serial.println(angulo_total);

  unsigned long dt = micros() - t0;
  if (dt < 20000) {
    unsigned long rem = 20000 - dt;       // µs
    delay(rem / 1000);                    // ms
    delayMicroseconds(rem % 1000);        // µs
  }
}

void matlab_send(float dato1, float dato2, float dato3){
  Serial.write("abcd");
  byte *b = (byte *) &dato1;
  Serial.write(b,4);
  b = (byte *) &dato2;
  Serial.write(b,4); 
  b = (byte *) &dato3;
  Serial.write(b,4);
  //etc con mas datos tipo float. Tambien podría pasarse como parámetro a esta funcion un array de floats.
}




