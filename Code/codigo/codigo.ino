
#include <Wire.h>

#define LED 2



const int MPU_addr = 0x68 ; // MPU-6050 I2C address
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

float angle = 0;
float maxangle = 0, minangle = 18000;
void setup() {
  // Set pin mode
  Serial. begin ( 9600 ) ;
  Wire. begin (23, 22, 100000) ;
  Wire. beginTransmission ( MPU_addr ) ;
  Wire. write ( 0x6B ) ; // Register PWR_MGMT_1
  Wire. write ( 0 ) ;    // set to zero (enables MPU-6050)
  Wire. endTransmission ( true ) ;
}

void loop() {
  angle = mpu_read () ;
  if (angle>maxangle){
    maxangle = angle;
    }
   if (angle<minangle){
    minangle = angle;
    }
   
  Serial.print("Max angle:");
  Serial.println(maxangle);
  Serial.print("Min angle:");
  Serial.println(minangle);
  
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
  return(AcY);
  
}
