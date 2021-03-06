/*
 * MPU9255.h
 *
 *  Created on: Jan, 01 2017
 *      Author: korr237i
 */

#ifndef MPU9255_H_
#define MPU9255_H_

#include "state.h"

extern I2C_HandleTypeDef* i2c_mpu9255;

#define G_VECTOR	9.80665


//	Scale factors
#define MPU9255_GYRO_SCALE_FACTOR	M_PI / (180 * 131)
#define MPU9255_ACCEL_SCALE_FACTOR	9.80665 / 16384


//	Mangetometer registers/modes
#define AK8963_MODE_POWER_DOWN	(0x00)
#define AK8963_MODE_ONCE		(0x01)
#define AK8963_MODE_8HZ 		(0x02)
#define AK8963_MODE_100HZ		(0x06)
#define AK8963_MODE_FUSE_ROM	0b00001111//(0x0F)

#define AK8963_DATA_READY		(0x01)
#define AK8963_DATA_OVERRUN		(0x02)
#define AK8963_DATA_OVERFLOW	(0x08)

#define AK8963_BIT_14_BIT		(0x00)
#define AK8963_BIT_16_BIT		(0x10)


//	Ranges
#define ACCEL_RANGE			1			//2g - 00, 4g - 01, 8g - 10, 16g - 11
#define GYRO_RANGE			2			//250degps - 00, 500degps - 01, 1000degps - 10, 2000degps - 11


/////////////////////////////////////////////////////
//////////////////// STRUCTURES /////////////////////
/////////////////////////////////////////////////////

typedef enum {

	//	full-scale range (FS_SEL_X, +- x = range in g)
	FS_SEL_2G = 0,
	FS_SEL_4G = 1,
	FS_SEL_8G = 2,
	FS_SEL_16G = 3

} accel_params_t;

typedef enum {

	//	full-scale range (FS_SEL_X, +- x = range in dps)
	FS_SEL_250DPS = 0,
	FS_SEL_500DPS = 1,
	FS_SEL_1000DPS = 2,
	FS_SEL_2000DPS = 3

} gyro_params_t;


//	Addresses of mpu9255
typedef enum {
	GYRO_AND_ACCEL = 0b11010000,
//	GYRO_AND_ACCEL = 0b11010010,
	COMPASS = 0b00011000
} mpu9255_address_t;



/////////////////////////////////////////////////////
//////////////////// FUNCTIONS //////////////////////
/////////////////////////////////////////////////////

int mpu9255_readRegister(mpu9255_address_t address, uint8_t reg_address, uint8_t* dataRead, uint8_t count);
int mpu9255_writeRegister(mpu9255_address_t address, uint8_t reg_address, uint8_t dataWrite);
int mpu9255_rewriteRegister(mpu9255_address_t address, uint8_t reg_address, uint8_t dataWrite);
int mpu9255_init(I2C_HandleTypeDef* hi2c);

int mpu9255_readIMU(int16_t * raw_accelData, int16_t * raw_gyroData);
int mpu9255_readCompass(int16_t * raw_compassData);
void mpu9255_recalcAccel(const int16_t * raw_accelData, float * accelData);
void mpu9255_recalcGyro(const int16_t * raw_gyroData, float * gyroData);
void mpu9255_recalcMagn(const int16_t * raw_magnData, float * magnData);


#endif /* MPU9255_H_ */
