#include "stm32f1xx_hal.h"
#include "i2c.h"

#ifndef MPU6050_H
#define MPU6050_H

typedef enum {
	MPU6050_OK   = 1,
	MPU6050_ERROR = 0
} MPU6050_STATUS;

/*Register Address Defines */
#define MPU6050_I2C_ADDRESS		       (0x68 << 1)      /*AD0 pin to GND*/
#define MPU6050_WHO_AM_I_REG            0x75
#define MPU6050_PWR_MGMT_1_REG          0x6B
#define MPU6050_SMPRT_DIV_REG           0x19
#define MPU6050_GYRO_CONFIG_REG 		0x1B
#define MPU6050_ACCEL_CONFIG_REG 		0x1C
#define MPU6050_ACCEL_DATA_REG			0x3B      /*Starting address of Accel data, total 6 registers long (word size 1 byte) */
#define MPU6050_GYRO_DATA_REG			0x43      /*Starting address of Gyro data,  total 6 registers long (word size 1 byte) */
#define MPU6050_TEMP_DATA_REG			0x41	  /*Starting address of Temp data,  total 2 registers long (word size 1 byte) */

/*Register Bit Position Defines*/
#define MPU6050_GYRO_FSEL_POS   		3U
#define MPU6050_ACCEL_FSEL_POS  		3U

/*Register Value Defines */
#define MPU6050_GYRO_RANGE_0   			0U        /* Gyroscope Range: -250 to +250   */
#define MPU6050_GYRO_RANGE_1   			1U        /* Gyroscope Range: -500 to +500   */
#define MPU6050_GYRO_RANGE_2   			2U    	  /* Gyroscope Range: -1000 to +1000 */
#define MPU6050_GYRO_RANGE_3   			3U		  /* Gyroscope Range: -2000 to +2000 */

#define MPU6050_ACCEL_RANGE_0   		0U        /* Accelerometer Range: -2g  to  +2g   */
#define MPU6050_ACCEL_RANGE_1   		1U        /* Accelerometer Range: -4g  to  +4g   */
#define MPU6050_ACCEL_RANGE_2   		2U    	  /* Accelerometer Range: -8g  to  +8g   */
#define MPU6050_ACCEL_RANGE_3   		3U	      /* Accelerometer Range: -16g to  +16g  */


/*Library Functions*/

/**
 * @brief Initialize the MPU6050 sensor
 * @param uint8_t gyroRange
 * @param uint8_t accelRange
 * @retval MPU6050_STATUS
 */
MPU6050_STATUS MPU6050_init(uint8_t gyroRange, uint8_t accelRange);

/**
 * @brief Read gyroscope data form sensor
 * @param uint8_t gyroRange
 * @param float *pGyroX
 * @param float *pGyroY
 * @param float *pGyroZ
 * @retval MPU6050_STATUS
 */
MPU6050_STATUS MPU6050_readGyroscope(uint8_t gyroRange, float *pGyroX, float *pGyroY, float *pGyroZ);

/**
 * @brief Read accelerometer data form sensor
 * @param uint8_t accelRange
 * @param float *pAccelX
 * @param float *pAccelY
 * @param float *pAccelZ
 * @retval MPU6050_STATUS
 */
MPU6050_STATUS MPU6050_readAccelerometer(uint8_t accelRange, float *pAccelX, float *pAccelY, float *pAccelZ);

/**
 * @brief Read roll, pitch angles
 * @param uint8_t accelRange
 * @param float *pRollAngle
 * @param float *pPitchAngle
 * @retval MPU6050_STATUS
 */
MPU6050_STATUS MPU6050_readAngles(uint8_t accelRange, float *pRollAngle, float *pPitchAngle);

/**
 * @brief Read temparature data from sensor
 * @param float *pTemparature
 * @retval MPU6050_STATUS
 */
MPU6050_STATUS MPU6050_readTemparature(float *pTemparature);


#endif // MPU6050_H
