/******************************************************************************
MinimalistExample.ino

Marshall Taylor @ SparkFun Electronics
May 20, 2015
https://github.com/sparkfun/LSM6DS3_Breakout
https://github.com/sparkfun/SparkFun_LSM6DS3_Arduino_Library

Description:
Most basic example of use.

Example using the LSM6DS3 with basic settings.  This sketch collects Gyro and
Accelerometer data every second, then presents it on the serial monitor.

Resources:
Uses Wire.h for i2c operation
Uses SPI.h for SPI operation
Either can be omitted if not used

Development environment specifics:
Arduino IDE 1.6.4
Teensy loader 1.23

Hardware connections:
Connect I2C SDA line to A4
Connect I2C SCL line to A5
Connect GND and 3.3v power to the IMU

This code is released under the [MIT License](http://opensource.org/licenses/MIT).

Please review the LICENSE.md file included with this example. If you have any questions 
or concerns with licensing, please contact techsupport@sparkfun.com.

Distributed as-is; no warranty is given.
******************************************************************************/

#include "SparkFunLSM6DS3.h"
#include "SparkFunLSM6DS3.cpp"
//#include "Wire.h"
//#include "SPI.h"

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <softPwm.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


LSM6DS3 myIMU; //Default constructor is I2C, addr 0x6B

void setup() {
  // put your setup code here, to run once:
 // Serial.begin(9600);
 // delay(1000); //relax...
//  Serial.println("Processor came out of reset.\n");
  
  //Call .begin() to configure the IMU
 
  
}
/*

void loop()
{
  //Get all parameters
  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatAccelX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readFloatAccelY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readFloatAccelZ(), 4);

  Serial.print("\nGyroscope:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatGyroX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readFloatGyroY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readFloatGyroZ(), 4);

  Serial.print("\nThermometer:\n");
  Serial.print(" Degrees C = ");
  Serial.println(myIMU.readTempC(), 4);
  Serial.print(" Degrees F = ");
  Serial.println(myIMU.readTempF(), 4);
  
  delay(1000);
}


*/

 
int main (void)
{
	
	

int fd, dane, idDev=0x6b;
//FILE *fp;
 
 //if(wiringPiSetup() == -1)
//exit(1);
 /*
if((fd=wiringPiI2CSetup(0x6b)) == -1){
printf("error initialize I2C");
exit(1);
}
printf(" I2C modul ADXL 345");
 
 //read X
dane = wiringPiI2CReadReg8(fd,0x0f);
printf("jestem x: %d",dane);
 
 
//Start measure
wiringPiI2CWriteReg8(fd, 0x10, 0x85);
 */
 
 	myIMU.begin();
  

 while(1)
 {


  printf(" X = ");
 // printf("x: %f",myIMU.readFloatAccelX());
  printf("x: %f",myIMU.readFloatGyroX());
printf("\n");
delay (100) ;
}
return 0 ;
}
