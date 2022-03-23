#include <Arduino.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Filters/Butterworth.hpp>
#include <Filters.h>

// Sampling frequency
const double f_s = 25; // Hz
// Cut-off frequency (-3 dB)
const double f_c = 7.84; // Hz
// Normalized cut-off frequency
const double f_n = 2 * f_c / f_s;

const float Ta = 40; // Tempo de Amostragem em ms

unsigned long lastMillis;
// Sixth-order Butterworth filter
auto filter = butter<6>(f_n);

#define MPU_SDA 23
#define MPU_SCL 22
MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(115200);
  Wire.begin(MPU_SDA, MPU_SCL, f_s);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}
float filtrado, sem_filtro;
void loop() {

  if(millis() - lastMillis > Ta){
    mpu6050.update();
    sem_filtro = mpu6050.getAngleX();
    filtrado = filter(sem_filtro);

    Serial.print("sem_filtro: ");
    Serial.print(sem_filtro);
    Serial.print(" , ");
    Serial.print("com_filtro: ");
    Serial.println(filtrado);
    
    lastMillis = millis();
  }  
}