#include <Servo.h>

Servo myservo;
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float delta_T = 0.020;

int contador1 = 0;
int contador2 = 0;
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

  // Serial.println("");
  delay(100);
  // Serial.begin(115200);
}

// float calcularAnguloFusion(float delta_T) {
//   static float angulo_g = 0;
//   static float angulo_g_sesgado = 0;
//   static float angulo_a = 0;
//   static float angulo_total = 0;

//   sensors_event_t a, g, temp;
//   mpu.getEvent(&a, &g, &temp);

//   float acel_y = a.acceleration.y;
//   float acel_z = a.acceleration.z;
//   float gyro_x = g.gyro.x;

//   angulo_g = angulo_total + (180.0 / PI) * (gyro_x * delta_T);
//   angulo_g_sesgado += (180.0 / PI) * (gyro_x * delta_T);
//   angulo_a = (180.0 / PI) * atan2(acel_y, acel_z);

//   float alpha = 0.1;
//   angulo_total = alpha * angulo_a + (1 - alpha) * angulo_g;

//   return angulo_total;
// }

void matlab_send(float* datos, size_t cantidad) {
  Serial.write("abcd");  // header fijo para identificar el inicio del paquete

  for (size_t i = 0; i < cantidad; i++) {
    byte* b = (byte*) &datos[i];
    Serial.write(b, 4);  // Cada float ocupa 4 bytes
  }

}

void loop() {

  contador1++;

  static float angulo_g = 0;
  static float angulo_g_sesgado = 0;
  static float angulo_a = 0;
  static float angulo_total = 0;

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float acel_y = a.acceleration.y;
  float acel_z = a.acceleration.z;
  float gyro_x = g.gyro.x;

  angulo_g = angulo_total + (180.0 / PI) * (gyro_x * delta_T);
  angulo_g_sesgado += (180.0 / PI) * (gyro_x * delta_T);
  
  // if (abs(acel_z) < 0.005 && abs(acel_y) < 0.005) {
  //   angulo_a = angulo_a;
  // } else {
  //   // el angulo negativo por como colocamos la imu
  //   angulo_a = ((180 / PI) * atan2(acel_y, acel_z));
  // }

  angulo_a = ((180 / PI) * atan2(acel_y, acel_z));


  float alpha = 0.1;
  angulo_total = alpha * angulo_a + (1 - alpha) * angulo_g;

  // float angulo_medido[] = {calcularAnguloFusion(delta_T)};

  int angulo_default = 90;

  int angulos[] = {0, 30};


  unsigned long contador_inicial = micros();

  if (contador1 == 100) {
    if (contador2 == 0){
        myservo.write(angulos[0] + angulo_default);
        contador2++;
        contador1 = 0;
        medicion1 = 0;

    }
    else if (contador2 == 1){
        myservo.write(angulos[1] + angulo_default);
        contador2 = 0;
        contador1 = 0;
        medicion1 = 30;
    }
  }



  float mediciones[2] = {angulo_total, medicion1};
  
  matlab_send(mediciones, 2);

  unsigned long tiempo_de_Tareas = micros() - contador_inicial;
  long tiempo_restante = 20000L - tiempo_de_Tareas;

  if (tiempo_restante > 1000) {
    delay(tiempo_restante / 1000); // en milisegundos
    delayMicroseconds(tiempo_restante % 1000); // el resto en microsegundos
  } else if (tiempo_restante > 0) {
    delayMicroseconds(tiempo_restante);
  }

}




