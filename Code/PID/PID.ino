/*Example sketch to control a stepper motor with A4988 stepper motor driver, AccelStepper library and Arduino: continuous rotation. More info: https://www.makerguides.com */
// Include the AccelStepper library:
#include <AccelStepper.h>

//include I2c library to receive MPU data
#include <Wire.h>


// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPinA 26
#define stepPinA 27
#define dirPinB 12
#define stepPinB 14
#define motorInterfaceType 1

//medium filter size
#define tam 10

float angles[tam]; //last MPU reading
float medangle = 0, lastmedangle = 0, ref = 0; //median
int pos; //array current positon
int range = 16384;//reference value for 180 degrees
float error = 0, lasterror = 0;
float vel = 0;
float kp = 10;
float kd = 4;
float ki = 1;
float p = 0, d = 0, i = 0;


//mpu data
const int MPU_addr = 0x68 ; // MPU-6050 I2C address
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; //receive mpu data



// Create a new instance of the AccelStepper class:
AccelStepper stepperA = AccelStepper(motorInterfaceType, stepPinA, dirPinA);
AccelStepper stepperB = AccelStepper(motorInterfaceType, stepPinB, dirPinB);
void setup() {
  /*-motor setup-*/
  // Set the maximum speed in steps per second:
  stepperA.setMaxSpeed(1000);
  stepperB.setMaxSpeed(1000);
  stepperA.setSpeed(400);
  stepperB.setSpeed(-400);
  /*-mpu setup-*/
  Serial. begin ( 9600 ) ;
  Wire. begin (23, 22, 100000) ;
  Wire. beginTransmission ( MPU_addr ) ;
  Wire. write ( 0x6B ) ; // Register PWR_MGMT_1
  Wire. write ( 0 ) ;    // set to zero (enables MPU-6050)
  Wire. endTransmission ( true ) ;

}
void loop() {

  medangle -= angles[pos] / (tam * 1.0);
  angles[pos] = mpu_read ();
  angles[pos] = angles[pos] * 180 / (range * 1.0); //transforma para graus
  medangle += angles[pos] / (tam * 1.0);

  error = ref - medangle;
  
  p = kp*error;
  d = kd*(lasterror - error);
  i = ki*(error + lasterror);
  vel = p + d + i;

  Serial.print("medangle: ");
  Serial.print(medangle);
  Serial.print("\t error: ");
  Serial.print(error);
  Serial.print("\t p: ");
  Serial.print(p);
  Serial.print("\t d: ");
  Serial.print(d);
  Serial.print("\t i: ");
  Serial.print(i);
  Serial.print("\t vel: ");
  Serial.println(vel);
  
  //Serial.print(" ");
  if (pos == tam - 1) {
    pos = 0;
  } else {
    pos++;
  }

  // Set the speed in steps per second:
//  stepperA.setSpeed(400);
//  stepperB.setSpeed(-400);
  // Step the motor with a constant speed as set by setSpeed():
  stepperA.runSpeed();
  stepperB.runSpeed();
  
  lasterror = error;
}


int16_t mpu_read () {
  Wire. beginTransmission ( MPU_addr ) ;
  Wire. write ( 0x3B ) ; // starting with register 0x3B (ACCEL_XOUT_H)
  Wire. endTransmission ( false ) ;
  Wire. requestFrom ( MPU_addr, 4, true ) ; // request a total of 14 records
  AcX = Wire. read () << 8 | Wire. read () ; // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire. read () << 8 | Wire. read () ; // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  //AcZ = Wire. read () << 8 | Wire. read () ; // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  //Tmp=Wire.read()<<8|Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  //GyX = Wire. read () << 8 | Wire. read () ; // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  //GyY = Wire. read () << 8 | Wire. read () ; // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  //GyZ = Wire. read () << 8 | Wire. read () ; // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  /*Serial. print ( "Accelerometer values: \n" ) ;
    Serial. print ( "AcX: " ) ;
    Serial. print ( AcX ) ;
    Serial. print ( "\nAcY: " ) ;*/
  //Serial. println ( AcY ) ;
  /*
    Serial. print ( "\nAcZ: " ) ;
    Serial. print ( AcZ ) ;
    //Serial.print("\nTemperature: " );
    //Serial.print(Tmp);
    Serial. print ( "\nGyroscope values:\n" ) ;
    Serial. print ( "GyX: " ) ;
    Serial. print ( GyX ) ;
    Serial. print ( "\nGyY: " ) ;
    Serial. print ( GyY ) ;
    Serial. print ( "\nGyZ: " ) ;
    Serial. print ( GyZ ) ;
    Serial. print ( "\n" ) ;
    delay ( 1000 ) ;*/
  return (AcY);

}


/*_______________________________________

      MAPA DA PISA - ESP
      _____________________________

  MOTOR A TA COM A ETIQUETA PRA CIMA E B PRA BAIXO

  P0       -
  P1       -
  P2       -
  P3       -
  P4       -
  P5       -
  P6       -
  P7       -
  P8       -
  P9       -
  P10      -
  P11      -
  P12      -  DIR B
  P13      -
  P14      -  STP B
  P15      -
  P16      -
  P17      -
  P18      -
  P19      -
  P20      -
  P21      -
  P22      -
  P23      - SCL MPU
  P24      - SDA MPU
  P25      -
  P26      - DIR A
  P27      - STP A
  P28      -
  P29      -
  P30      -
  P31      -
  P32      -
  P33      -
  P34      -
  P35      -


  ______________________________________ */
