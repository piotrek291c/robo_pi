/*
* i2c.c:
* Simple test I2C. Device ADXL 345
***********************************************************************
*/
 
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <softPwm.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
 
 #define LSM6DS3_ACC_GYRO_OUTX_L_XL  			0X28
#define LSM6DS3_ACC_GYRO_OUTX_H_XL  			0X29
#define LSM6DS3_ACC_GYRO_OUTY_L_XL  			0X2A
#define LSM6DS3_ACC_GYRO_OUTY_H_XL  			0X2B
#define LSM6DS3_ACC_GYRO_OUTZ_L_XL  			0X2C
#define LSM6DS3_ACC_GYRO_OUTZ_H_XL  			0X2D
 
#define LSM6DS3_ACC_GYRO_STATUS_REG  			0X1E
#define LSM6DS3_ACC_GYRO_OUT_TEMP_L  			0X20
#define LSM6DS3_ACC_GYRO_OUT_TEMP_H  			0X21
#define LSM6DS3_ACC_GYRO_OUTX_L_G  			0X22
#define LSM6DS3_ACC_GYRO_OUTX_H_G  			0X23
#define LSM6DS3_ACC_GYRO_OUTY_L_G  			0X24
#define LSM6DS3_ACC_GYRO_OUTY_H_G  			0X25
#define LSM6DS3_ACC_GYRO_OUTZ_L_G  			0X26
#define LSM6DS3_ACC_GYRO_OUTZ_H_G  			0X27
#define LSM6DS3_ACC_GYRO_OUTX_L_XL  			0X28
#define LSM6DS3_ACC_GYRO_OUTX_H_XL  			0X29
#define LSM6DS3_ACC_GYRO_OUTY_L_XL  			0X2A
#define LSM6DS3_ACC_GYRO_OUTY_H_XL  			0X2B
#define LSM6DS3_ACC_GYRO_OUTZ_L_XL  			0X2C
#define LSM6DS3_ACC_GYRO_OUTZ_H_XL  			0X2D 
 
 
 int fd, dane, idDev=0x6b;
 
 int gyroRange = 2000;   //Max deg/s.  Can be: 125, 245, 500, 1000, 2000
 int accelRange=4;
 //****************************************************************************//
//
//  Accelerometer section
//
//****************************************************************************//
 
 
 
 float calcAccel( int16_t input )
{
	float output = (float)input * 0.061 * (accelRange >> 1) / 1000;
	return output;
}
 
 
 
 int16_t readRawAccelX( void )
{
	int16_t output;
	//status_t errorLevel = readRegisterInt16( &output, LSM6DS3_ACC_GYRO_OUTX_L_XL );
	output= wiringPiI2CReadReg16(fd ,LSM6DS3_ACC_GYRO_OUTX_L_XL);
	
	return output;
}
float readFloatAccelX( void )
{
	float output = calcAccel(readRawAccelX());
	return output;
}
 
 
  int16_t readRawAccelY( void )
{
	int16_t output;
	//status_t errorLevel = readRegisterInt16( &output, LSM6DS3_ACC_GYRO_OUTX_L_XL );
	output= wiringPiI2CReadReg16(fd ,LSM6DS3_ACC_GYRO_OUTY_L_XL );
	
	return output;
}
float readFloatAccelY( void )
{
	float output = calcAccel(readRawAccelY());
	return output;
}
 
 
 
 
  int16_t readRawAccelZ( void )
{
	int16_t output;
	//status_t errorLevel = readRegisterInt16( &output, LSM6DS3_ACC_GYRO_OUTX_L_XL );
	output= wiringPiI2CReadReg16(fd ,LSM6DS3_ACC_GYRO_OUTZ_L_XL);
	
	return output;
}
float readFloatAccelZ( void )
{
	float output = calcAccel(readRawAccelZ());
	return output;
}
 
 
 
 //****************************************************************************//
//
//  Gyroscope section
//
//****************************************************************************//


float calcGyro( int16_t input )
{
	int gyroRangeDivisor = gyroRange / 125;
	if ( gyroRange == 245 ) {
		gyroRangeDivisor = 2;
	}

	float output = (float)input * 4.375 * (gyroRangeDivisor) / 1000;
	return output;
}


int16_t readRawGyroX( void )
{
	int16_t output;
	//status_t errorLevel = readRegisterInt16( &output, LSM6DS3_ACC_GYRO_OUTX_L_G );
	output= wiringPiI2CReadReg16(fd ,LSM6DS3_ACC_GYRO_OUTX_L_G);

	return output;
}
float readFloatGyroX( void )
{
	float output = calcGyro(readRawGyroX());
	return output;
}

int16_t readRawGyroY( void )
{
	int16_t output;
	//status_t errorLevel = readRegisterInt16( &output, LSM6DS3_ACC_GYRO_OUTY_L_G );
	output= wiringPiI2CReadReg16(fd ,LSM6DS3_ACC_GYRO_OUTY_L_G);
	
	return output;
}
float readFloatGyroY( void )
{
	float output = calcGyro(readRawGyroY());
	return output;
}

int16_t readRawGyroZ( void )
{
	int16_t output;
	//status_t errorLevel = readRegisterInt16( &output, LSM6DS3_ACC_GYRO_OUTZ_L_G );
	output= wiringPiI2CReadReg16(fd ,LSM6DS3_ACC_GYRO_OUTZ_L_G);
	
	return output;
}
float readFloatGyroZ( void )
{
	float output = calcGyro(readRawGyroZ());
	return output;
}



//****************************************************************************//
//
//  Temperature section
//
//****************************************************************************//
int16_t readRawTemp( void )
{
	int16_t output;
	//readRegisterInt16( &output, LSM6DS3_ACC_GYRO_OUT_TEMP_L );
	output= wiringPiI2CReadReg16(fd ,LSM6DS3_ACC_GYRO_OUT_TEMP_L);
	return output;
}  

float readTempC( void )
{
	float output = (float)readRawTemp() / 16; //divide by 16 to scale
	output += 25; //Add 25 degrees to remove offset

	return output;

}

float readTempF( void )
{
	float output = (float)readRawTemp() / 16; //divide by 16 to scale
	output += 25; //Add 25 degrees to remove offset
	output = (output * 9) / 5 + 32;

	return output;

}

//****************************************************************************//
 
 
 
 
 
 
 
 
int main (void)
{

//FILE *fp;
 
 
if(wiringPiSetup() == -1)
exit(1);
 
if((fd=wiringPiI2CSetup(0x6b)) == -1){
printf("error initialize I2C");
exit(1);
}
printf(" I2C modul ADXL 345");
 
 //read X
dane = wiringPiI2CReadReg8(fd,0x0f);
printf("jestem x: %d",dane);
 
 float pomiar_x;
 float pomiar_y;
 float pomiar_z;
  float pomiar_gyro_x;
 float pomiar_gyro_y;
 float pomiar_gyro_z;
//Start measure
wiringPiI2CWriteReg8(fd, 0x10, 0x44); // on accelerometr
wiringPiI2CWriteReg8(fd, 0x11, 0x4E); // on gyro
 while(1)
 {
//read X
//dane = wiringPiI2CReadReg16(fd,0x28);
//printf("\ndane x: %d\n",dane);
pomiar_x=readFloatAccelX();
if(pomiar_x<=0.000 or pomiar_x >=0.015)
{
 // printf("x: %f ",pomiar_x);
 // printf("\n");
}
pomiar_y=readFloatAccelY();
if(pomiar_y>=-0.0000 or pomiar_y <=-0.015)
{
 // printf("y: %f ",pomiar_y);
 // printf("\n");
}
pomiar_z=readFloatAccelZ();
if(pomiar_z<=0.23 or pomiar_z >=0.25)
{
 // printf("z: %f ",pomiar_z);
 // printf("\n");
}


pomiar_gyro_x=readFloatGyroX();
if(pomiar_gyro_x<=0.000 or pomiar_gyro_x >=0.015)
{
 // printf("x: %f ",pomiar_gyro_x);
 // printf("\n");
}
pomiar_gyro_y=readFloatGyroY();
if(pomiar_gyro_y>=-10.0000 or pomiar_gyro_y <=-40)
{
  printf("y: %f ",pomiar_gyro_y);
  printf("\n");
}
pomiar_gyro_z=readFloatGyroZ();
if(pomiar_gyro_z<=0.23 or pomiar_gyro_z >=0.25)
{
 // printf("z: %f ",pomiar_gyro_z);
 // printf("\n");
}

 // printf(" Degrees C =  %f \n" , readTempC());



//read Y
//dane = wiringPiI2CReadReg8(fd,0x2a);
//printf("\ndane y: %d",dane);
//read Z
//dane = wiringPiI2CReadReg8(fd,0x2c);
//printf("\ndane z: %d",dane);
delay (10) ;
}
return 0 ;
}
