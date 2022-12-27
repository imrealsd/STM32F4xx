#include "mpu6050.h"
#include "stdint.h"
#include "math.h"



MPU6050_STATUS MPU6050_init(uint8_t gyroRange, uint8_t accelRange)
{	
	uint8_t buff;

	/**
	 * Initialisation Sequence :
	 * step0: Initialise I2C Peripheral : 400khz
	 * step1: Check for I2C Address Match
	 * step2: Power Reset & Clk Source Select
	 * step3: Set Sample Rate
	 * step4: Config Gyro Range
	 * Step5: Config Accel Range 
	*/
	
	/*Read the WHO_AM_I register to verify module address*/
	if (HAL_I2C_Mem_Read(&hi2c1, MPU6050_I2C_ADDRESS, MPU6050_WHO_AM_I_REG, 1, &buff, 1, 100) != HAL_OK ){
		return MPU6050_ERROR;
	}
	
	/*if address is correct*/
	if (buff == 0x68) {
		
		/*wake up the sensor & select internal 8mz crystal as clk source*/
		buff = 0x00;
		if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2C_ADDRESS, MPU6050_PWR_MGMT_1_REG, 1, &buff, 1, 100) != HAL_OK) {
			return MPU6050_ERROR;
		}
		
		/**
		 * set gyroscope sample rate to 1mhz
		 * sampleRate = clk_speed / (1 + saplrt_reg_val) 
		 */
		buff = 0x07;
		if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2C_ADDRESS, MPU6050_SMPRT_DIV_REG, 1, &buff, 1, 100) != HAL_OK) {
			return MPU6050_ERROR;
		}
		
		/*set gyro range as per user choic */
		buff = gyroRange << MPU6050_GYRO_FSEL_POS;
		if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2C_ADDRESS, MPU6050_GYRO_CONFIG_REG, 1, &buff, 1, 100) != HAL_OK) {
			return MPU6050_ERROR;
		}
		
		/*set accel range as per user choic */
		buff = accelRange << MPU6050_ACCEL_FSEL_POS;
		if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2C_ADDRESS, MPU6050_ACCEL_CONFIG_REG, 1, &buff, 1, 100) != HAL_OK) {
			return MPU6050_ERROR;
		}

		return MPU6050_OK;
	}
	
	return MPU6050_ERROR;
}


MPU6050_STATUS MPU6050_readGyroscope(uint8_t gyroRange, float *pGyroX, float *pGyroY, float *pGyroZ)
{
	uint8_t buff[6];
	int8_t index = 0;
	int16_t gyroRaw;
	float 	gyroDivider;
	
	/*determine gyro_scale_factor form the gyro range*/
	switch (gyroRange) {

	case MPU6050_GYRO_RANGE_0:
		gyroDivider = 131;
		break;
	case MPU6050_GYRO_RANGE_1:
		gyroDivider = 65.5;
		break;
	case MPU6050_GYRO_RANGE_2:
		gyroDivider = 32.8;
		break;
	case MPU6050_GYRO_RANGE_3:
		gyroDivider = 16.4;
		break;
	}
	
	/*read 6 bytes of raw gyroscope data * arrange them into int16_t size*/
	while (index < 6) {
		if (HAL_I2C_Mem_Read(&hi2c1, MPU6050_I2C_ADDRESS, (MPU6050_GYRO_DATA_REG + index), 1, &buff[index], 1, 100) != HAL_OK) {
			return MPU6050_ERROR;
		}
		index++;
		HAL_Delay(2);
	}

	/*Converting RAW Gyro Value to DPS (degree per sec)*/

	gyroRaw = (int16_t) (buff[0] << 8 | buff[1]);
	*pGyroX = (float) gyroRaw / gyroDivider;

	gyroRaw = (int16_t) (buff[2] << 8 | buff[3]);
	*pGyroY = (float) gyroRaw / gyroDivider;

	gyroRaw = (int16_t) (buff[4] << 8 | buff[5]);
	*pGyroZ = (float) gyroRaw / gyroDivider;

	/*Calibration*/
	*pGyroX = *pGyroX + 4.45;
	*pGyroY = *pGyroY + 0.2;
	*pGyroZ = *pGyroZ - 0.9;

	return MPU6050_OK;
}


MPU6050_STATUS MPU6050_readAccelerometer(uint8_t accelRange, float *pAccelX, float *pAccelY, float *pAccelZ)
{
	uint8_t buff[6];
	int16_t accelRaw;
	int8_t  index = 0;
	float 	accelDivider;
	
	/*determine accel_scale_factor form the accel range*/
	switch (accelRange) {

	case MPU6050_ACCEL_RANGE_0:
		accelDivider = 16384;
		break;
	case MPU6050_ACCEL_RANGE_1:
		accelDivider = 8192;
		break;
	case MPU6050_ACCEL_RANGE_2:
		accelDivider = 4096;
		break;
	case MPU6050_ACCEL_RANGE_3:
		accelDivider = 2048;
		break;
	}
	
	/*read 6 bytes of raw accelerometer data * arrange them into int16_t size*/
	while (index < 6) {
		if (HAL_I2C_Mem_Read(&hi2c1,MPU6050_I2C_ADDRESS, (MPU6050_ACCEL_DATA_REG + index), 1, &buff[index], 1, 100) != HAL_OK) {
			return MPU6050_ERROR;
		}
		index++;
		HAL_Delay(2);
	}

	/*Converting RAW Accel Value to g */

	accelRaw = (int16_t) ((buff[0] << 8) | buff[1]);
	*pAccelX = (float) accelRaw / accelDivider;

	accelRaw = (int16_t) ((buff[2] << 8) | buff[3]);
	*pAccelY = (float) accelRaw / accelDivider;

	accelRaw = (int16_t) ((buff[4] << 8) | buff[5]);
	*pAccelZ = (float) accelRaw / accelDivider;

	/*Calibration*/
	*pAccelX = *pAccelX - 0.1;
	*pAccelY = *pAccelY - 0.01;
	*pAccelZ = *pAccelZ - 0.2;

	return MPU6050_OK;
}


MPU6050_STATUS MPU6050_readTemparature(float *pTemparature)
{
	uint8_t buff[2];
	int16_t tempRaw;
	float 	tempDivider = 340;
	int8_t  index = 0;
	
	/*reading 2 bytes of raw temparature value*/

	while (index < 2) {
		if (HAL_I2C_Mem_Read(&hi2c1,MPU6050_I2C_ADDRESS, (MPU6050_TEMP_DATA_REG + index) , 1, &buff[index], 1, 100) != HAL_OK) {
			return MPU6050_ERROR;
		}
		index++;
		HAL_Delay(2);
	}

	/*Converting RAW temp Value to degree C */

	tempRaw = (int16_t) ((buff[0] << 8) | buff[1]);
	*pTemparature = ((float) tempRaw / tempDivider) + (float) 33.53;

	return MPU6050_OK;
}

MPU6050_STATUS MPU6050_readAngles(uint8_t accelRange, float *pRollAngle, float *pPitchAngle)
{
	float accelx;
	float accely;
	float accelz;

	if (MPU6050_readAccelerometer(accelRange, &accelx, &accely, &accelz) != MPU6050_OK) {
		return MPU6050_ERROR;
	}

	*pRollAngle  = atan(accely/ sqrt(accelx * accelx + accelz * accelz)) * (float) 1/ ((float)3.142  / (float)180);
	*pPitchAngle = atan(accelx/ sqrt(accely * accely + accelz * accelz)) * (float) 1/ ((float)3.142  / (float)180);

	return MPU6050_OK;
}

