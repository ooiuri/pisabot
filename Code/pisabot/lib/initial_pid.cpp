#include <Arduino.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Filters/Butterworth.hpp>
#include <Filters.h>
// Include the AccelStepper library:
#include <AccelStepper.h>

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPinA 26
#define stepPinA 27
#define dirPinB 12
#define stepPinB 14
#define motorInterfaceType 1
#define MAX_SPEED 400
// Create a new instance of the AccelStepper class:
AccelStepper stepperA = AccelStepper(motorInterfaceType, stepPinA, dirPinA);
AccelStepper stepperB = AccelStepper(motorInterfaceType, stepPinB, dirPinB);
int speed;

// Sampling frequency
const double f_s = 25; // Hz
// Cut-off frequency (-3 dB)
const double f_c = 7.84; // Hz
// Normalized cut-off frequency
const double f_n = 2 * f_c / f_s;
// Sixth-order Butterworth filter
auto filter = butter<6>(f_n);

const float Ta = 40; // Tempo de Amostragem em ms
unsigned long now, lastMillis;

// referencia do sistema
double rss = 0;
// double Kp = 33.25;
// double Ki = 0.15;
// double Kd = 8.50; 
double Kp = 0;
double Ki = 0.0;
double Kd = 0; 
double I, lastAngle, lastError;
double Output;

#define kOut 28000
#define outMin -100
#define outMax 100

#define MPU_SDA 23
#define MPU_SCL 22
MPU6050 mpu6050(Wire);
float angulofiltrado;

double calculo_pid(){
  double error = rss - angulofiltrado;
  I += (Ki * error);
  I = constrain(I, outMin, outMax);
  double D = (error - lastError)/Ta;
  Output = Kp * error + Kd * D + I*Ta;

  lastAngle = angulofiltrado;
  lastError = error;
  return Output;
}

void mover(){
  // stepperA.move(steps);
  // stepperB.move(steps);
  
  stepperA.setSpeed(speed);
  stepperB.setSpeed(-speed);
  
}

void setup() {
  // Set the maximum speed in steps per second:
  stepperA.setMaxSpeed(MAX_SPEED);
  stepperB.setMaxSpeed(MAX_SPEED);
  stepperA.setSpeed(MAX_SPEED);
  stepperB.setSpeed(-MAX_SPEED);

  Serial.begin(115200);
  Wire.begin(MPU_SDA, MPU_SCL, f_s);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  if(Serial.available()){
    char message = Serial.read();
    if(message == 'a') {
      Kp += .1;
    }
    if(message == 's') {
      Kp -= .1;
    }
    if(message == 'd') {
      Kd += .1;
    }
    if(message == 'f') {
      Kd -= .1;
    }
    if(message == 'g') {
      Ki += .1;
    }
    if(message == 'h') {
      Ki += .1;
    }

    if(message == 'q') {
      Kp += 1;
    }
    if(message == 'w') {
      Kp -= 1;
    }
    if(message == 'e') {
      Kd += 1;
    }
    if(message == 'r') {
      Kd -= 1;
    }
    if(message == 't') {
      Ki += 1;
    }
    if(message == 'y') {
      Ki += 1;
    }

    Serial.println("Valores atualizados: ");
    Serial.print("Kp: ");
    Serial.print(Kp);
    Serial.print("\tKd: ");
    Serial.print(Kd);
    Serial.print("\tKi: ");
    Serial.print(Ki);
    delay(1500);
  }
  now = millis();
  stepperA.runSpeed();
  stepperB.runSpeed();
  
  mpu6050.update();
  // angulofiltrado = filter(mpu6050.getAngleX());
  angulofiltrado = mpu6050.getAngleX();
  if(now - lastMillis > Ta){
    calculo_pid();
    speed = round(Output);
    speed = constrain(speed, -MAX_SPEED, MAX_SPEED);
    Serial.print("AnguloFiltrado: ");
    Serial.print(angulofiltrado);
    Serial.print("\tOutput: ");
    Serial.print(Output,8);
    Serial.print("\tspeed: ");
    Serial.println(speed);
    mover();
    lastMillis = now;
  }  
}


